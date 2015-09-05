#ifndef OPENGL_LOADER_H
#define OPENGL_LOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>
#include <string>
#include <utility> 
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <GL/glew.h>

#include "parser/Parser.h"
#include "parser/CommentParser.h"
#include "parser/IncludeParser.h"
#include "parser/DefineParser.h"
#include "parser/ConfigParser.h"

#include "opengl/Shader.h"
#include "opengl/Program.h"

#include "exception/Error.h"

// To determine the shader filetype (string)
#ifndef VERTEX_SHADER_TYPE
	#define VERTEX_SHADER_TYPE ".vert"
#endif // VERTEX_SHADER_TYPE

#ifndef FRAGMENT_SHADER_TYPE
	#define FRAGMENT_SHADER_TYPE ".frag"
#endif // FRAGMENT_SHADER_TYPE

#ifndef GEOMETRY_SHADER_TYPE__
	#define GEOMETRY_SHADER_TYPE ".geom"
#endif // GEOMETRY_SHADER_TYPE

#ifndef TESSELATION_EVALUATION_SHADER_TYPE
	#define TESSELATION_EVALUATION_SHADER_TYPE ".tessev"
#endif // TESSELATION_EVALUATION_SHADER_TYPE

#ifndef TESSELATION_CONTROL_SHADER_TYPE
	#define TESSELATION_CONTROL_SHADER_TYPE ".tesscn"
#endif // TESSELATION_CONTROL_SHADER_TYPE


// For checking shader compile status (0 / 1)
#ifndef SHADERS_CHECK_COMPILE_STATUS
	#define SHADERS_CHECK_COMPILE_STATUS 1
#endif // SHADERS_CHECK_COMPILE_STATUS


// For checking program compile status (0 / 1)
#ifndef PROGRAMS_CHECK_COMPILE_STATUS
	#define PROGRAMS_CHECK_COMPILE_STATUS 1
#endif // PROGRAMS_CHECK_COMPILE_STATUS


class OpenGL_Loader {

public:

  static OpenGL_Loader& getInstance() {
    static OpenGL_Loader loader{};
    return loader;
  };

	OpenGL_Loader() = default;
	~OpenGL_Loader();
	void clearProgramsAndShaders();

	void loadShader(const std::string file, const GLenum shaderType);
	void loadShaders(const std::string file);
	GLuint accessShader(const std::string name) const; 

	void loadProgram(const std::string name, const std::string path, const std::string files);
	void loadPrograms(const std::string file);
	GLuint accessProgram(std::string name) const;

	void printAvailableShaders() const;
	void printAvailablePrograms() const;
	
private:
	std::string loadCode(const std::string file);
	GLenum getType(const std::string name) const;

	void checkShaderCompileStatus(const std::string file, const GLuint shader) const;
	void checkProgramCompileStatus(std::string name, const GLuint program) const;

	std::unordered_map<std::string, Shader*> shaders_;
	std::unordered_map<std::string, Program*> programs_;

	IncludeParser<DefineParser<CommentParser<Parser> > > code_parser_;
	IncludeParser<DefineParser<CommentParser<Parser> > > programs_parser_;
	
};

#endif // OPENGL_LOADER_H
