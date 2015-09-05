#include "opengl/Program.h"


Program::Program(const std::string name, const GLuint program, const std::string files)
	: name_(name)
  , files_(files)
	, program_(program)
	
{

}


Program::~Program()
{
	glDeleteProgram(program_);
}


std::string Program::getName() const
{
	return name_;
}


std::string Program::getFiles() const
{
	return files_;
}


GLuint Program::getProgram() const
{
	return program_;
}