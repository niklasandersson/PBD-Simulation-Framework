#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <GL/glew.h>


class Program
{

public:
	Program(const std::string name, const GLuint program, const std::string files);
	~Program();

	std::string getName() const;
	std::string getFiles() const;
	GLuint getProgram() const;

private:
	std::string name_;
	std::string files_;
	GLuint program_;

};


#endif // PROGRAM_H