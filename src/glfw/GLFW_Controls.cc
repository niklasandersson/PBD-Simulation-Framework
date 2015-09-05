#include "glfw/GLFW_Controls.h"


#define MOVIE_CAMERA 		1
#define SCREEN_WIDTH 		800
#define SCREEN_HEIGHT 	600

// Initial position : on +Z
glm::vec3 position = glm::vec3(0.0f,1.0f,0.0f);
// Initial horizontal angle : toward -Z
float horizontalAngle = 0.0f;
// Initial vertical angle : none
float verticalAngle = - (M_PI / 2.0f);
// Initial Field of View
float initialFoV = 45.0f;

#define INITIAL_SPEED 10.0f
float speed = INITIAL_SPEED; // 3 units / second
float mouseSpeed = 0.002f;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
float FoV;


glm::mat4 getViewMatrix(){
	return ViewMatrix;
}


glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


glm::vec3 getCameraPosition() {
	return position;
}


#if MOVIE_CAMERA == 1
double Linear_Interpolation(double a, double b, double t) {
	return a + (b - a)*t;
}

float addSpeed[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
double oldHorizontalAngle = 0.0;
double oldVerticalAngle = 0.0;
double horizontalInterpolationValue = 1.0;
double verticalInterpolationValue = 1.0;
double newHorizontalAngle = 0.0;
double newVerticalAngle = 0.0;
#endif


void Scroll(GLFWwindow* window, double x, double y) {

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Events::scroll((unsigned short)xpos, (unsigned short)ypos, x, y);

	if(y == 1) {
		speed += 0.5f;
	} else if(y == -1) {
		speed -= 0.5f;
		if(speed < 0.0f) speed = 0.0f;
	}

}


void Click(GLFWwindow* window, int button, int action, int mods) {

	if(button == GLFW_MOUSE_BUTTON_3  && action == GLFW_PRESS) {
		speed = INITIAL_SPEED;
	}

}


void InitializeControls(GLFWwindow* window) {
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, Scroll);
	glfwSetMouseButtonCallback(window, Click);
	FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.
}


void computeMatricesFromInputs(GLFWwindow* window){
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

#if MOVIE_CAMERA == 1
	// Compute new orientation
	newHorizontalAngle += mouseSpeed * float( SCREEN_WIDTH/2 - xpos );
	newVerticalAngle += mouseSpeed * float( SCREEN_HEIGHT/2 - ypos );

	horizontalInterpolationValue += 0.08;
	verticalInterpolationValue += 0.08;

	horizontalAngle = Linear_Interpolation(oldHorizontalAngle, newHorizontalAngle, horizontalInterpolationValue);
	verticalAngle = Linear_Interpolation(oldVerticalAngle, newVerticalAngle, verticalInterpolationValue);

	if(horizontalAngle != newHorizontalAngle) {
		oldHorizontalAngle = horizontalAngle;
		horizontalInterpolationValue = 0.0;
	}

	if(verticalAngle != newVerticalAngle) {
		oldVerticalAngle = verticalAngle;
		verticalInterpolationValue = 0.0;
	}

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - M_PI/2.0f),
		0,
		cos(horizontalAngle - M_PI/2.0f)
	);

	// Keys:
	// http://www.glfw.org/docs/latest/group__keys.html

	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	#define SPEED_UP 0.1f
	#define SLOW_DOWN 0.03f
	#define MAX_SPEED 1.5f

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS ){
		// position += direction * deltaTime * speed;
		addSpeed[0] += SPEED_UP;
		addSpeed[0] = addSpeed[0] > MAX_SPEED ? MAX_SPEED : addSpeed[0];
	} else {
		addSpeed[0] -= SLOW_DOWN;
		addSpeed[0] = addSpeed[0] < 0 ? 0.0f : addSpeed[0];
	}

	position += addSpeed[0] * direction * deltaTime * speed;

	// Move backward
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS ){
		// position -= direction * deltaTime * speed;
		addSpeed[1] += SPEED_UP;
		addSpeed[1] = addSpeed[1] > MAX_SPEED ? MAX_SPEED : addSpeed[1];
	} else {
		addSpeed[1] -= SLOW_DOWN;
		addSpeed[1] = addSpeed[1] < 0 ? 0.0f : addSpeed[1];
	}

	position -= addSpeed[1] * direction * deltaTime * speed;

	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS ){
		// position += right * deltaTime * speed;
		addSpeed[2] += SPEED_UP;
		addSpeed[2] = addSpeed[2] > MAX_SPEED ? MAX_SPEED : addSpeed[2];
	} else {
		addSpeed[2] -= SLOW_DOWN;
		addSpeed[2] = addSpeed[2] < 0 ? 0.0f : addSpeed[2];
	}

	position += addSpeed[2] * right * deltaTime * speed;

	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS ){
		// position -= right * deltaTime * speed;
		addSpeed[3] += SPEED_UP;
		addSpeed[3] = addSpeed[3] > MAX_SPEED ? MAX_SPEED : addSpeed[3];
	} else {
		addSpeed[3] -= SLOW_DOWN;
		addSpeed[3] = addSpeed[3] < 0 ? 0.0f : addSpeed[3];
	}
	position -= addSpeed[3] * right * deltaTime * speed;


	if (glfwGetKey( window, GLFW_KEY_Q ) == GLFW_PRESS){
		// position -= right * deltaTime * speed;
		addSpeed[4] += SPEED_UP;
		addSpeed[4] = addSpeed[4] > MAX_SPEED ? MAX_SPEED : addSpeed[4];
	} else {
		addSpeed[4] -= SLOW_DOWN;
		addSpeed[4] = addSpeed[4] < 0 ? 0.0f : addSpeed[4];
	}

	position += 0.15f * addSpeed[4] * up * deltaTime * speed;


	if (glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS){
		// position -= right * deltaTime * speed;
		addSpeed[5] += SPEED_UP;
		addSpeed[5] = addSpeed[5] > MAX_SPEED ? MAX_SPEED : addSpeed[5];
	} else {
		addSpeed[5] -= SLOW_DOWN;
		addSpeed[5] = addSpeed[5] < 0 ? 0.0f : addSpeed[5];
	}


	position -= 0.10f * addSpeed[5] * up * deltaTime * speed;

		if (glfwGetKey( window, GLFW_KEY_TAB ) == GLFW_PRESS){
		// position -= right * deltaTime * speed;
		addSpeed[6] += SPEED_UP;
		addSpeed[6] = addSpeed[6] > MAX_SPEED ? MAX_SPEED : addSpeed[6];
	} else {
		addSpeed[6] -= SLOW_DOWN;
		addSpeed[6] = addSpeed[6] < 0 ? 0.0f : addSpeed[6];
	}

	position += addSpeed[6] * up * deltaTime * speed;


	if (glfwGetKey( window, GLFW_KEY_R ) == GLFW_PRESS){
		// position -= right * deltaTime * speed;
		addSpeed[7] += SPEED_UP;
		addSpeed[7] = addSpeed[7] > MAX_SPEED ? MAX_SPEED : addSpeed[7];
	} else {
		addSpeed[7] -= SLOW_DOWN;
		addSpeed[7] = addSpeed[7] < 0 ? 0.0f : addSpeed[7];
	}

	position -= 0.5f * addSpeed[7] * up * deltaTime * speed;

#else
	// MOVIE_CAMERA == 0

	// Compute new orientation
	horizontalAngle += mouseSpeed * float( SCREEN_WIDTH/2 - xpos );
	verticalAngle   += mouseSpeed * float( SCREEN_HEIGHT/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f),
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS ){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS ){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS ){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS ){
		position -= right * deltaTime * speed;
	}

#endif

	#define RATIO (float)(SCREEN_WIDTH / SCREEN_HEIGHT)
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, RATIO, 0.1f, 100.0f);

	// Camera matrix
	ViewMatrix = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}
