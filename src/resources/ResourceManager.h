#pragma once
#include <string>
#include <memory>
#include <map>

namespace renderer {
	class ShaderProgram;
	class Texture2D;
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

	std::shared_ptr<renderer::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
	std::shared_ptr<renderer::Texture2D> getTexture(const std::string& texturename);

private:
	typedef std::map<const std::string, std::shared_ptr<renderer::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap m_shaderPrograms;

	std::string getFileString(const std::string& relativeFilePath) const;

	std::string m_path;

	typedef std::map<const std::string, std::shared_ptr<renderer::Texture2D>> TexturesMap;
	TexturesMap m_textures;


};