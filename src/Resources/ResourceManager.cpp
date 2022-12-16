#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include <sstream>
#include <fstream>
#include <iostream>

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