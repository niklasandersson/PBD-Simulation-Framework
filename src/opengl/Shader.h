#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

class Shader
{

public:
	Shader(const std::string name, GLuint compiledShader);
	~Shader();	
	GLuint getCompiledShader(void) const;
	std::string getName(void) const;
 
private:
	std::string name_;
	GLuint compiledShader_;

};

#endif // SHADER_H