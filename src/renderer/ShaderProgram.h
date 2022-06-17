#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/mat4x4.hpp>

namespace renderer{
	class ShaderProgram {
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		~ShaderProgram();

		bool isCompiled() const { return m_isCompiled; }
		void use() const;
		void setInt(const std::string& name, const GLint value);
		void SetMatrix4(const std::string& name, const glm::mat4& matrix);

		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram() = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

	private:
		bool createShader(const std::string & source, const GLenum shaderType, GLuint& shaderID);
		bool m_isCompiled = false;
		GLuint m_ID = 0;
	};
}
