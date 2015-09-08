#ifndef GLFW_CONTROLS_H
#define GLFW_CONTROLS_H

#include <iostream>
#include <math.h>

#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#define TOGGLE_COOL_DOWN 0.2

class GLFW_Controls {

public:
  GLFW_Controls(GLFWwindow* glfwWindow, 
                const unsigned int screenWidth = 800, 
                const unsigned int screenHeight = 600,
                const glm::vec3 defaultPosition = glm::vec3(0, 1, 0),
                const glm::vec3 defaultDirection = glm::vec3(0, 0, -1),
                const float defaultFov = 45.0f,
                const float nearPlaneDistance = 0.1f,
                const float farPlaneDistance = 100.0f,
                const float defaultMovementSpeed = 10.0f,
                const float defaultMouseSpeed = 0.002f);

  glm::vec3 getCameraPosition();
  glm::vec3 getCameraDirection();
  glm::vec3 getCameraRight();
  glm::vec3 getCameraUp();
  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();

  void computeMatricesFromInput();

protected:
  virtual void computeCamera(double& xpos, double& ypos, float& deltaTime);
  

  GLFWwindow* const glfwWindow_;
  const unsigned int screenWidth_;
  const unsigned int screenHeight_;
  const unsigned int halfScreenWidth_;
  const unsigned int halfScreenHeight_;
  const glm::vec3 defaultPosition_;
  const glm::vec3 defaultDirection_;
  const float defaultFov_;
  const float defaultMovementSpeed_;
  const float defaultMouseSpeed_;

  glm::vec3 position_;
  glm::vec3 direction_;
  glm::vec3 up_;
  glm::vec3 right_;

  glm::mat4 viewMatrix_;
  glm::mat4 projectionMatrix_;

  float fov_;
  float screenRatio_;

  float defaultHorizontalAngle_;
  float defaultVerticalAngle_;

  float horizontalAngle_;
  float verticalAngle_;

  float movementSpeed_;
  float mouseSpeed_;

  float nearPlaneDistance_;
  float farPlaneDistance_;

  bool inGame_;
  double lastToggle_;
  bool enteringWindow_;
  bool reenteringWindow_;

  private:

};


#endif // GLFW_CONTROLS_H