#include "opengl/Shader.h"	


Shader::Shader(const std::string name, GLuint compiledShader)
	: name_(name)
	, compiledShader_(compiledShader)
{
}


Shader::~Shader()
{
	glDeleteShader(compiledShader_);
}


GLuint Shader::getCompiledShader(void) const
{
	return compiledShader_;
}


std::string Shader::getName(void) const
{
	return name_;
}