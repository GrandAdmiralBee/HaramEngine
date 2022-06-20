#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>

namespace renderer {
	class ShaderProgram;
	class Texture2D;
	class Sprite;
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

	std::shared_ptr<renderer::Sprite> loadSprite(const std::string& spritename,
												const std::string& textureName,
												const std::string& shaderName, 
												const unsigned int spriteWidth, 
												const unsigned int spriteHeight,
												const std::string& subTextureName = "default");

	std::shared_ptr<renderer::Sprite> getSprite(const std::string& spriteName);

	std::shared_ptr<renderer::Texture2D> loadTextureAtlas(const std::string textureName,
														  const std::string texturePath,
		                                                  const std::vector<std::string> subTextures,
														  const unsigned int subTextureWidth,
														  const unsigned int subTextureHeight);


private:
	typedef std::map<const std::string, std::shared_ptr<renderer::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap m_shaderPrograms;

	typedef std::map<const std::string, std::shared_ptr<renderer::Texture2D>> TexturesMap;
	TexturesMap m_textures;

	typedef std::map<const std::string, std::shared_ptr<renderer::Sprite>> SpritesMap;
	SpritesMap m_sprites;

	std::string getFileString(const std::string& relativeFilePath) const;

	std::string m_path;



};