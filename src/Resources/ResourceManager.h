#pragma once
#include <string>
#include <memory>
#include <map>
#include <vector>
namespace Renderer {
	class ShaderProgram;
	class Texture2D;
	class Sprite;
	class AnimatedSprite;
}

class ResourceManager
{
public:
	ResourceManager(const std::string& executablePath);
	~ResourceManager() = default;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	std::shared_ptr<Renderer::ShaderProgram> loadShaderProgram(const std::string& shaderProgramName, const std::string& vertexPath, const std::string& fragmentPath);
	std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string& shaderProgramName);

	std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
	std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName);

	std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName,
													const std::string& textureName,
													const std::string& shaderProgramName,
													const unsigned int spriteWidth,
													const unsigned int spriteHeight,
													const std::string& subTextureName = "default");
	std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName);

	std::shared_ptr<Renderer::AnimatedSprite> loadAnimatedSprite(const std::string& spriteName,
																 const std::string& textureName,
																 const std::string& shaderProgramName,
																 const unsigned int spriteWidth,
																 const unsigned int spriteHeight,
																 const std::string& subTextureName = "default");
	std::shared_ptr<Renderer::AnimatedSprite> getAnimatedSprite(const std::string& spriteName);

	std::shared_ptr<Renderer::Texture2D> loadTextureAtlas(std::string textureName,
														  std::string texturePath,
														  std::vector<std::string> subTextureNames,
														  const unsigned int subTextureWidth,
														  const unsigned int subTextureHeight);
private:
	std::string getFileString(const std::string& relativeFilePath) const;

	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramMap;
	ShaderProgramMap m_shaderPrograms;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	TexturesMap m_textures;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpritesMap;
	SpritesMap m_sprites;

	typedef std::map<const std::string, std::shared_ptr<Renderer::AnimatedSprite>> animatedSpritesMap;
	animatedSpritesMap m_animatedSprites;
	std::string m_path;
};