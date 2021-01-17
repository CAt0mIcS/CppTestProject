#include "Shader.h"
#include "Renderer.h"
#include <sstream>
#include <fstream>
#include <iostream>


Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}


Shader::~Shader() {
	GLCall(glDeleteProgram(m_RendererID));
}


ShaderProgramSource Shader::ParseShader(const std::string& filepath) {

	enum class ShaderType {
		None = -1, VertexShader = 0, FragmentShader = 1
	};

	std::fstream stream(filepath.c_str());

	std::stringstream ss[2];
	std::string line;
	ShaderType type = ShaderType::None;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VertexShader;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FragmentShader;
			}
		}
		else
			ss[(int)type] << line << '\n';
	}

	return { ss[0].str(), ss[1].str() };

}


unsigned int Shader::CompileShader(unsigned int type, const char* source) {

	unsigned int id = glCreateShader(type);
	GLCall(glShaderSource(id, 1, &source, nullptr));
	GLCall(glCompileShader(id));

	//Error handling
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE) {
		int length;
		//Gets length of log message which is to be allocated on memory later on
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		//Allocated memory on the stack
		char* message = (char*)alloca(length * sizeof(char));
		//Fills the message with the actual error message
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex " : "fragment ") << "shader!\n";
		std::cout << message << "\n";
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	unsigned int program = glCreateProgram();

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader.c_str());  //or &vertexShader[0] == vertexShader.c_str();
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader.c_str());

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));

	GLCall(glLinkProgram(program));

	GLCall(glDetachShader(program, vs)); //Unimplemented by TheCherno
	GLCall(glDetachShader(program, fs)); //Unimplemented by TheCherno

	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}


void Shader::Bind() const {
	GLCall(glUseProgram(m_RendererID));
}


void Shader::Unbind() const {
	GLCall(glUseProgram(0));
}


void Shader::SetUniform1i(const std::string& name, int value) {
	GLCall(glUniform1i(GetUniformLocation(name), value));
}


void Shader::SetUniform1f(const std::string& name, float value) {
	GLCall(glUniform1f(GetUniformLocation(name), value));
}


void Shader::SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4) {
	GLCall(glUniform4f(GetUniformLocation(name), v1, v2, v3, v4))
}


void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}


int Shader::GetUniformLocation(const std::string& name) {
	
	
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));

	if (location == -1) {
		std::cout << "Warning: uniform '" << name << "' doesn't exist!\n";
	}
	
	m_UniformLocationCache[name] = location;
	return location;
}
