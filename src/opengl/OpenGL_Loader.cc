#include "opengl/OpenGL_Loader.h"


OpenGL_Loader::~OpenGL_Loader() {
	clearProgramsAndShaders();
}


void OpenGL_Loader::clearProgramsAndShaders() {
	for(auto shader : shaders_)
	{
		if(shader.second) {
			delete shader.second;
		}
	}

	shaders_.clear();

	for(auto program : programs_)
	{
		if(program.second) {
			delete program.second;
		}
	}

	programs_.clear();
}


GLenum OpenGL_Loader::getType(const std::string name) const {
	GLenum type = 0;

	std::string ending = name.substr(name.find('.'), name.length());  
	
	if(ending == VERTEX_SHADER_TYPE)
		type = GL_VERTEX_SHADER;
	else if(ending == FRAGMENT_SHADER_TYPE)
		type = GL_FRAGMENT_SHADER;
	else if(ending == GEOMETRY_SHADER_TYPE)
		type = GL_GEOMETRY_SHADER; 
	else if(ending == TESSELATION_CONTROL_SHADER_TYPE)
		type = GL_TESS_CONTROL_SHADER;
	else if(ending == TESSELATION_EVALUATION_SHADER_TYPE)
		type = GL_TESS_EVALUATION_SHADER;
	else 
		throw std::invalid_argument{ report_error("The type was not defined for '" << name << "'.") };	

	return type;
}


void OpenGL_Loader::loadShaders(const std::string file)
{
	std::ifstream input(file.c_str(), std::ifstream::in);

	if(input.good()) 
	{
		std::string line;
		std::string type;
		std::string dir;

		try
		{
			dir = file.substr(0, file.rfind('/')+1);   
		}
		catch(...)
		{
			dir = "";
		}

		while(getline(input, line))
		{
			loadShader(dir + line, getType(line));
		}
		
		input.close();
	} 
	else
	{
		std::cout << "Error (LINE: " << __LINE__ << ") loading: " << file << std::endl;
	}
}


void OpenGL_Loader::loadShader(const std::string file, const GLenum shaderType)
{
	std::string name;
	try
	{
		 name = file.substr(file.rfind('/')+1);   
	}
	catch(...)
	{
		name = file;
	}

	auto it = shaders_.find(name);

	if(it != shaders_.end()) 
	{
		return;
	}
	
	// Create shader
	GLuint shader = glCreateShader(shaderType);

	// Load code from file
	std::string code = loadCode(file);
	
	// Set source code
	char const* source = code.c_str();
  glShaderSource(shader, 1, &source, NULL);
	
	// Compile shader
	glCompileShader(shader);

	// Check shader compile status
#if SHADERS_CHECK_COMPILE_STATUS
	checkShaderCompileStatus(file, shader);
#endif // SHADERS_CHECK_COMPILE_STATUS

	// Add to DB
	shaders_.insert(std::make_pair(name, new Shader(name, shader)));
}


GLuint OpenGL_Loader::accessShader(const std::string name) const
{
	GLuint shader;
	auto it = shaders_.find(name);

	if(it == shaders_.end()) 
	{
		assert(false && "Shader not loaded.");
	}
	else 
	{
		shader = it->second->getCompiledShader();
	}

	return shader;
}


std::string OpenGL_Loader::loadCode(const std::string file) {
	return code_parser_.parseFile(file);
}


void OpenGL_Loader::checkShaderCompileStatus(const std::string file, const GLuint shader) const
{
	GLint params = GL_FALSE;
  GLint max_message_length;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_message_length);

  if(max_message_length <= 1) 
  {
  	std::cout << "Shader: " << '\'' << file << '\'' << " compile status: \'OK\'" << std::endl;
  } 
  else 
  {
  	char message[max_message_length];
    GLsizei actual_message_length;
		glGetShaderInfoLog(shader, max_message_length, &actual_message_length, message);

    std::cerr << "Shader: " << '\'' << file << '\'' << " compile status: \'" << message << "\'" << std::endl;
    std::exit(EXIT_FAILURE);
  }
}


void OpenGL_Loader::checkProgramCompileStatus(std::string name, const GLuint program) const
{
	GLint params = GL_FALSE;
  GLint max_message_length;
  
	glGetProgramiv(program, GL_LINK_STATUS, &params);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_message_length);
	
  if(max_message_length <= 1) 
  {
    std::cout << "Program: " << '\'' << name << '\'' << " link status: \'OK\'" << std::endl;
  } 
  else 
  {
  	char message[max_message_length];
    GLsizei actual_message_length;
		glGetProgramInfoLog(program, max_message_length, &actual_message_length, message);

    std::cerr << "Program: " << '\'' << name << '\'' << " link status: \'" << message << "\'" << std::endl;
    std::exit(EXIT_FAILURE);
  }
}


void OpenGL_Loader::loadProgram(const std::string name, const std::string path, const std::string files)
{
	assert(!name.empty() && !files.empty() && "Files corrupt");

	auto it = programs_.find(name);

	if(it != programs_.end()) 
	{
		assert(false && "A program was already loaded.");
	} 
	else 
	{
		GLuint program = glCreateProgram();

		std::istringstream is(files);
		std::string shader;

		while(is >> shader && !shader.empty())
		{
			loadShader(path + shader, getType(shader));
			glAttachShader(program, accessShader(shader));
		}

	  glLinkProgram(program);

		// Check program compile status
#if PROGRAMS_CHECK_COMPILE_STATUS
		checkProgramCompileStatus(name, program);
#endif // PROGRAMS_CHECK_COMPILE_STATUS

		programs_.insert(std::make_pair(name, new Program(name, program, files)));
	}

}


void OpenGL_Loader::loadPrograms(const std::string file)
{
	std::string path;

	try
	{
		path = file.substr(0, file.rfind('/')+1);   
	}
	catch(...)
	{
		path = "";
	}

	std::istringstream input{programs_parser_.parseFile(file)};
	std::string line;

	while(getline(input, line))
	{
		std::istringstream is(line);
		std::string name;
		std::string files;
		
		is >> name >> std::ws;
		getline(is, files);

		if(name == "" || (name[0] == '/' && name[1] == '/')) {
			continue;
		}

		loadProgram(name, path, files);
	}

}


GLuint OpenGL_Loader::accessProgram(std::string name) const
{
	GLuint program;

	auto it = programs_.find(name);

	if(it == programs_.end()) 
	{
		assert(false && "A program was not found.");
	} 
	else 
	{
		program = it->second->getProgram();
	}	

	return program;
}


void OpenGL_Loader::printAvailableShaders() const
{
	for(auto shader : shaders_)
	{
		std::cout << "\t" << shader.first << std::endl;
	}
}


void OpenGL_Loader::printAvailablePrograms() const
{
	for(auto program : programs_)
	{
		std::cout << "\t" << program.first << " : " << program.second->getFiles() << std::endl;
	}
}
