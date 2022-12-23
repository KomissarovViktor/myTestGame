#pragma once
#include "Sprite.h"
#include <map>
#include <vector>
namespace Renderer {
	
	class AnimatedSprite: public Sprite
	{
	public:
		AnimatedSprite(std::shared_ptr<Texture2D> pTexture,
				  	   std::string subTextureName,
					   std::shared_ptr<ShaderProgram> pShaderProgram,
					   const glm::vec2& position = glm::vec2(0.f), 
					   const glm::vec2& size = glm::vec2(1.f),
					   const float rotation = 0.f);

		void insertState(std::string stateName, std::vector<std::pair<std::string, uint64_t>> subTexturesDuration);
		void render() const override;

		void update(const uint64_t delta);
		void setState(std::string stateName);
	private:
		std::map<std::string, std::vector<std::pair<std::string, uint64_t>>> m_statesMap;
		size_t m_currentFrame = 0;
		uint64_t m_currentAnimationTime = 0;
		std::map<std::string, std::vector<std::pair<std::string, uint64_t>>>::const_iterator m_pCurrentAnimationDurations = m_statesMap.end();

		mutable bool m_dirty = false;
	};
}