#pragma once;
#include <glad/glad.h>
#include <string>
#include <glm/vec2.hpp>
#include <map>

namespace Renderer {

	class Texture2D
	{
	public:
		struct SubTexture2D
		{
			SubTexture2D(const glm::vec2& _leftBottomUV, const glm::vec2& _rightTopUV) : leftBottomUV(_leftBottomUV), rightTopUV(_rightTopUV)
			{}
			SubTexture2D()
			{
				leftBottomUV = glm::vec2(0.f);
				rightTopUV = glm::vec2(1.f);
			}
			glm::vec2 leftBottomUV;
			glm::vec2 rightTopUV;
		};
		Texture2D(
			const GLuint width, const GLuint height,
			const unsigned char* data,
			const unsigned int channels,
			GLenum filter,
			GLenum wrapMode);

		Texture2D() = delete;
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;

		Texture2D& operator=(Texture2D&& texture2d) noexcept;
		Texture2D(Texture2D&& texture2d) noexcept;
		~Texture2D();
		
		void addSubTexture(std::string subTextureName, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV);
		const SubTexture2D& getSubTexture(const std::string subTextureName);

		unsigned int getWidth() const { return m_width; }
		unsigned int getHeight()const { return m_height; }
		void bind() const;
	private:
		GLuint m_ID;
		GLenum m_mode;
		unsigned int m_width;
		unsigned int m_height;
		std::map<std::string, SubTexture2D> m_subTexturesMap;
	};
}