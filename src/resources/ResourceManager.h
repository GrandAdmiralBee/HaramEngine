#pragma once
#include <string>
#include <memory>
#include <map>

namespace renderer {
	class ShaderProgram;
}

class ResourceManager {
public:
	ResourceManager(const std::string& executablePath);
	~ResourceManager() = default;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	std::shared_ptr<renderer::ShaderProgram> loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
    std::shared_ptr<renderer::ShaderProgram> getShaderProgram(const std::string& shadername);


private:
	typedef std::map<const std::string, std::shared_ptr<renderer::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap m_shaderPrograms;

	std::string getFileString(const std::string& relativeFilePath) const;

	std::string m_path;

};