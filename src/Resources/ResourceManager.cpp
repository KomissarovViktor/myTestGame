#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include <sstream>
#include <fstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ResourceManager(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath) const
{
	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		std::cerr << "Can't open file: " << std::endl;
		return std::string();
	}
	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaderProgram(const std::string& shaderProgramName, const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexSting = getFileString(vertexPath);
	if (vertexSting.empty())
	{
		std::cerr << "No Vertex Shader" << std::endl;
		return nullptr;
	}
	std::string fragmentSting = getFileString(fragmentPath);
	if (fragmentSting.empty())
	{
		std::cerr << "No Fragment Shader" << std::endl;
		return nullptr;
	}
	std::shared_ptr<Renderer::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderProgramName, std::make_shared<Renderer::ShaderProgram>(vertexSting, fragmentSting)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}
	std::cerr << "Can't load shader program\n"
		<< "Vertex Shader: " << vertexPath<< "\n"
		<< "Fragment Shader: " << fragmentPath << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderProgramName)
{
	ShaderProgramMap::const_iterator it = m_shaderPrograms.find(shaderProgramName);
	if (it != m_shaderPrograms.end())
	{
		return it->second;
		
	}
	std::cerr << "Can't find shader: " << shaderProgramName << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
	int channels = 0;
	int width = 0;
	int height = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);
	if (!pixels)
	{
		std::cerr << "Can't load image" << texturePath << std::endl;
		return nullptr;
	}
	std::shared_ptr<Renderer::Texture2D>& newTexture = m_textures.emplace(textureName, std::make_shared<Renderer::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);
	return newTexture;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::getTexture(const std::string& textureName)
{
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;

	}
	std::cerr << "Can't find texture: " << textureName << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::Sprite> ResourceManager::loadSprite(const std::string& spriteName,
																const std::string& textureName,
																const std::string& shaderProgramName,
																const unsigned int spriteWidth,
																const unsigned int spriteHeight,
																const std::string& subTextureName)
{
	auto pTexture = getTexture(textureName);
	if (!pTexture)
	{
		std::cerr << "Can't find texture: " << textureName << " for the sprite: " << spriteName << std::endl;
	}
	
	auto pShaderProgram = getShaderProgram(shaderProgramName);
	if (!pTexture)
	{
		std::cerr << "Can't find shader program: " << shaderProgramName << " for the sprite: " << spriteName << std::endl;
	}

	std::shared_ptr<Renderer::Sprite> newSprite = m_sprites.emplace(spriteName, std::make_shared<Renderer::Sprite>(pTexture, subTextureName, pShaderProgram, glm::vec2(0.f), glm::vec2(spriteWidth, spriteHeight))).first->second;
	
	return newSprite;
}

std::shared_ptr<Renderer::Sprite> ResourceManager::getSprite(const std::string& spriteName)
{
	SpritesMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
	{
		return it->second;

	}
	std::cerr << "Can't find texture: " << spriteName << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::AnimatedSprite> ResourceManager::loadAnimatedSprite(const std::string& spriteName,
																				const std::string& textureName,
																				const std::string& shaderProgramName,
																				const unsigned int spriteWidth,
																				const unsigned int spriteHeight,
																				const std::string& subTextureName)
{
	auto pTexture = getTexture(textureName);
	if (!pTexture)
	{
		std::cerr << "Can't find texture: " << textureName << " for the sprite: " << spriteName << std::endl;
	}

	auto pShaderProgram = getShaderProgram(shaderProgramName);
	if (!pTexture)
	{
		std::cerr << "Can't find shader program: " << shaderProgramName << " for the sprite: " << spriteName << std::endl;
	}

	std::shared_ptr<Renderer::AnimatedSprite> newSprite = m_animatedSprites.emplace(spriteName, std::make_shared<Renderer::AnimatedSprite>(pTexture, subTextureName, pShaderProgram, glm::vec2(0.f), glm::vec2(spriteWidth, spriteHeight))).first->second;

	return newSprite;
}

std::shared_ptr<Renderer::AnimatedSprite> ResourceManager::getAnimatedSprite(const std::string& spriteName)
{
	auto it = m_animatedSprites.find(spriteName);
	if (it != m_animatedSprites.end())
	{
		return it->second;

	}
	std::cerr << "Can't find texture: " << spriteName << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::loadTextureAtlas(std::string textureName,
														   			   std::string texturePath,
																	   std::vector<std::string> subTextureNames,
																	   const unsigned int subTextureWidth,
																	   const unsigned int subTextureHeight)
{
	auto pTexture = loadTexture(std::move(textureName), std::move(texturePath));

	if (pTexture)
	{
		auto textureWidth = pTexture->getWidth();
		auto textureHeight = pTexture->getHeight();
		unsigned int currentX = 0;
		unsigned int currentY = textureHeight;
		for (auto& currentSubTextureName : subTextureNames)
		{
			glm::vec2 leftBottom = glm::vec2(static_cast<float>(currentX) / textureWidth, static_cast<float>(currentY - subTextureHeight) / textureHeight);
			glm::vec2 rightTop = glm::vec2(static_cast<float>(currentX + subTextureWidth) / textureWidth, static_cast<float>(currentY) / textureHeight);
			pTexture->addSubTexture(std::move(currentSubTextureName), leftBottom, rightTop);
			currentX += subTextureWidth;
			if (currentX >= textureWidth)
			{
				currentX = 0;
				currentY -= subTextureHeight;
			}
		}
	}
	return pTexture;
}
