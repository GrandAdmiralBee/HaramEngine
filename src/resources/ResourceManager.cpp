#include "ResourceManager.h"
#include "../renderer/ShaderProgram.h"
#include "../renderer/Texture2D.h"
#include "../renderer/Sprite.h"


#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#include "stb_image.h"


ResourceManager::ResourceManager(const std::string& executablePath) {
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath) const {
	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open()) {
		std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
		return std::string{};
	}
	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}

std::shared_ptr<renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath) {
	
	std::string vertexString = getFileString(vertexPath);
	if (vertexString.empty()) {
		std::cerr << "No vertex shader!" << std::endl;
		return nullptr;
	}

	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty()) {
		std::cerr << "No fragment shader!" << std::endl;
		return nullptr;
	}

	std::shared_ptr<renderer::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<renderer::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled()) {
		return newShader;
	}

	std::cerr << "Cannot load shader program:\n"
		<< "Vertex: " << vertexPath << "\n"
		<< "fragment: " << fragmentPath << "\n" << std::endl;
	return nullptr;
}

std::shared_ptr<renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName) {
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it != m_shaderPrograms.end()) {
		return it->second;
	}
	std::cerr << "Cannot find the shader program: " << shaderName << std::endl;
	return nullptr;
}

std::shared_ptr<renderer::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath) {
	int channels = 0;
	int width = 0;
	int height = 0;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

	if (!pixels) {
		std::cerr << "Cannot load image: " << texturePath << std::endl;
		return nullptr;
	}

	std::shared_ptr<renderer::Texture2D> newTexture = m_textures.emplace(textureName, std::make_shared<renderer::Texture2D>(width,
																															height,	
																															pixels,		
																															channels, 
																															GL_NEAREST, 
																															GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);
	return newTexture;
}

std::shared_ptr<renderer::Texture2D> ResourceManager::getTexture(const std::string& texturename) {
	TexturesMap::const_iterator it = m_textures.find(texturename);
	if (it != m_textures.end()) {
		return it->second;
	}
	std::cerr << "Cannot find the texture: " << texturename << std::endl;
	return nullptr;
}


std::shared_ptr<renderer::Sprite> ResourceManager::loadSprite(const std::string& spriteName,
																const std::string& textureName,
																const std::string& shaderName,
																const unsigned int spriteWidth,
																const unsigned int spriteHeight,
																const std::string& subTextureName) {

	auto pTexture = getTexture(textureName);
	if (!pTexture) {
		std::cerr << "Cannot find the texture: " << textureName << " for the sprite: " << spriteName << std::endl;
	}

	auto pShaderProgram = getShaderProgram(shaderName);
	if (!pTexture) {
		std::cerr << "Cannot find the shader: " << shaderName << " for the sprite: " << spriteName << std::endl;
	}

	std::shared_ptr<renderer::Sprite> newSprite = m_sprites.emplace(textureName, std::make_shared<renderer::Sprite>(pTexture,
																													subTextureName,
																													pShaderProgram,
																													glm::vec2(0.f, 0.f),
																													glm::vec2(spriteWidth, spriteHeight), 0)).first->second;


	return newSprite;

}

std::shared_ptr<renderer::Sprite> ResourceManager::getSprite(const std::string& spriteName) {
	SpritesMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end()) {
		return it->second;
	}
	std::cerr << "Cannot find the texture: " << spriteName << std::endl;
	return nullptr;
}

std::shared_ptr<renderer::Texture2D> ResourceManager::loadTextureAtlas(const std::string textureName,
																	   const std::string texturePath,
																	   const std::vector<std::string> subTextures,
																	   const unsigned int subTextureWidth,
																	   const unsigned int subTextureHeight) {

	auto pTexture = loadTexture(textureName, texturePath);

	if (pTexture) {
		const unsigned int textureWidth = pTexture->width();
		const unsigned int textureHeight = pTexture->height();
		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = textureHeight;

		for (const auto& currentSubTextureName : subTextures) {
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth,                 static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);
			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth, static_cast<float>(currentTextureOffsetY) / textureHeight);

			pTexture->addSubTexture(currentSubTextureName, leftBottomUV, rightTopUV);

			currentTextureOffsetX += subTextureWidth;
			if (currentTextureOffsetX >= textureWidth) {

				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureHeight;
			}
		}

	}
	return pTexture;
}

