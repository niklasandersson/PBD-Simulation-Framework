#ifndef GLFW_CONTROLS_H
#define GLFW_CONTROLS_H

#include <iostream>
#include <math.h>

#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS 0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



void InitializeControls(GLFWwindow* window);
glm::vec3 getCameraPosition();
void computeMatricesFromInputs(GLFWwindow* window);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();


#endif // GLFW_CONTROLS_H