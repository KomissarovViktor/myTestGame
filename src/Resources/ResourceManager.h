#pragma once;
#include <string>
#include <shared_mutex>
#include <map>

namespace Renderer {
	class ShaderProgram;
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
	void loadTexture(const std::string& textureName, const std::string& texturePath);
	
private:
	std::string getFileString(const std::string& relativeFilePath) const;

	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramMap;
	ShaderProgramMap m_shaderPrograms;

	std::string m_path;
};