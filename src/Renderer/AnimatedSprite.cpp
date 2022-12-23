#include "AnimatedSprite.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

#include <iostream>

namespace Renderer {

	AnimatedSprite::AnimatedSprite(std::shared_ptr<Texture2D> pTexture,
								   std::string subTextureName,
								   std::shared_ptr<ShaderProgram> pShaderProgram,
								   const glm::vec2& position,
								   const glm::vec2& size,
								   const float rotation) : Sprite(std::move(pTexture), std::move(subTextureName), std::move(pShaderProgram), position, size, rotation)
	{
	}
	void AnimatedSprite::insertState(std::string stateName, std::vector<std::pair<std::string, uint64_t>> subTexturesDuration)
	{
		m_statesMap.emplace(std::move(stateName), std::move(subTexturesDuration));
	}
	void AnimatedSprite::render() const
	{
		if (m_dirty)
		{
			auto subTexture = m_pTexture->getSubTexture(m_pCurrentAnimationDurations->second[m_currentFrame].first);
			GLfloat textureCoords[] = {

			subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
			subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
			subTexture.rightTopUV.x, subTexture.rightTopUV.y,

			subTexture.rightTopUV.x, subTexture.rightTopUV.y,
			subTexture.rightTopUV.x, subTexture.leftBottomUV.y,
			subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
			};

			glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordsVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textureCoords), &textureCoords);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			m_dirty = false;
		}
		Sprite::render();
	}
	void AnimatedSprite::update(const uint64_t delta)
	{
		if (m_pCurrentAnimationDurations != m_statesMap.end())
		{
			m_currentAnimationTime += delta;
			while (m_currentAnimationTime >= m_pCurrentAnimationDurations->second[m_currentFrame].second)
			{
				m_currentAnimationTime -= m_pCurrentAnimationDurations->second[m_currentFrame].second;
				++m_currentFrame;
				if (m_currentFrame==m_pCurrentAnimationDurations->second.size())
				{
					m_currentFrame = 0;
				}
				m_dirty = true;
			}
		}
	}
	void AnimatedSprite::setState(std::string stateName)
	{
		auto it = m_statesMap.find(stateName);
		if (it == m_statesMap.end())
		{
			std::cout << "Can't find the state: " << stateName << std::endl;
			return;
		}
		if (m_pCurrentAnimationDurations != it)
		{
			m_currentFrame = 0;
			m_currentAnimationTime = 0;
			m_pCurrentAnimationDurations = it;
			m_dirty = true;
		}
	}
}
