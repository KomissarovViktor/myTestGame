#include "ShaderProgram.h"
#include <iostream>
namespace Renderer {
	ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
	{
        GLuint vertexShaderID;
        if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
        {
            std::cerr << "VERTEX SHADER compile time error" << std::endl;
            return;
        }

        GLuint fragmentShaderID;
        if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
        {
            std::cerr << "FRAGMENT SHADER compile time error" << std::endl;
            glDeleteShader(vertexShaderID);
            return;
        }
        

        m_id = glCreateProgram();
        glAttachShader(m_id, vertexShaderID);
        glAttachShader(m_id, fragmentShaderID);
        glLinkProgram(m_id);

        GLint success;
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            GLchar infoLog[1024];
            glGetShaderInfoLog(m_id, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER; Link time error:\n" << infoLog << std::endl;
        }
        else
        {
            m_isCompiled = true;
        }
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
	}

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
    {
        glDeleteProgram(m_id);
        m_id = shaderProgram.m_id;
        m_isCompiled = shaderProgram.m_isCompiled;
        shaderProgram.m_id = 0;
        shaderProgram.m_isCompiled = false;
        return *this;
    }

    ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
    {
        m_id = shaderProgram.m_id;
        m_isCompiled = shaderProgram.m_isCompiled;
        shaderProgram.m_id = 0;
        shaderProgram.m_isCompiled = false;
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(m_id);
    }

    void ShaderProgram::setInt(const std::string& name, const GLint value)
    {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
    }

    void ShaderProgram::use() const
    {
        glUseProgram(m_id);
    }

    bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
    {
        shaderID = glCreateShader(shaderType);
        const char* code = source.c_str();
        glShaderSource(shaderID, 1, &code, nullptr);
        glCompileShader(shaderID);

        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER; Compile time error:\n" << infoLog << std::endl;
            return false;
        }
        return true;
    }

}
