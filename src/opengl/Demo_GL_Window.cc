#include "opengl/Demo_GL_Window.h"


Demo_GL_Window::Demo_GL_Window(const unsigned int window_width, 
                               const unsigned int window_height, 
                               const std::string& window_title,
                               const unsigned int multisampling_and_antialiasing,
                               const unsigned int opengl_major_version,
                               const unsigned int opengl_minor_version) 
: GLFW_Window(window_width, window_height, window_title)
, multisampling_and_antialiasing_{multisampling_and_antialiasing}
, opengl_major_version_{opengl_major_version}
, opengl_minor_version_{opengl_minor_version}
{
  Console::getInstance()->add("fps", dynamic_cast<GLFW_Window*>(this), &GLFW_Window::set_print_fps);
}


Demo_GL_Window::~Demo_GL_Window() {
  delete glfw_controls_;
  delete scene_;
  delete scene2_;
}
  

void Demo_GL_Window::initialize() {
  GLFW_Window::initialize();

  loadPrograms();

  glfw_controls_ = new GLFW_Controls{
    glfw_window_,
    get_window_width(),
    get_window_height(),
    glm::vec3(0, 1, 0),
    glm::vec3(0, -1, -1),
    45.0f,
    0.1f,
    100.0f,
    10.0f,
    0.002f
  };

  scene_ = new Scene();
  scene2_ = new Scene2();

}


void Demo_GL_Window::loadPrograms() {

  OpenGL_Loader& openglLoader = OpenGL_Loader::getInstance();

  openglLoader.loadPrograms("src/app/shader/programs.prog");

  std::cout << "Available Shaders:" << std::endl;
  openglLoader.printAvailableShaders();

  std::cout << "Available Programs:" << std::endl;
  openglLoader.printAvailablePrograms();
 
}


void Demo_GL_Window::render() {
  GLFW_Window::render();  

  glfw_controls_->computeMatricesFromInput();

  glm::mat4 view_matrix = glfw_controls_->getViewMatrix();
  glm::mat4 projection_matrix = glfw_controls_->getProjectionMatrix();

  scene2_->setViewMatrix(view_matrix);
  scene2_->setProjectionMatrix(projection_matrix);

  // Do rendering here
  // scene_->render();
  scene2_->render();

  glfwSwapBuffers(glfw_window_);
  glfwPollEvents();

}


void Demo_GL_Window::cleanup() {
  GLFW_Window::cleanup();
}


void Demo_GL_Window::set_glfw_window_hints() {
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, multisampling_and_antialiasing_);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_major_version_);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_minor_version_);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

