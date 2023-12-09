#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "particle.h"
#include "util.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_click(GLFWwindow* window, ParticleGenerator&);

// settings
const GLint ScreenWidth = 800;
const GLint ScreenHeight = 600;
float curX = ScreenWidth / 2.0f;
float curY = ScreenHeight / 2.0f;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// mouse flag
bool dragging = false;

int main() {
	// glfw initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(ScreenWidth, ScreenHeight, "Particles", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_position_callback);

	// capture the mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// build and compile shaders
	Shader particleShader("particle.vs", "particle.fs");

	// load textures
	std::string dirPath = PROJECTDIR;
	auto texturePath = dirPath + "/resources/smoke.png";
	GLint textureId = loadTexture(texturePath);

	// init particle system
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(ScreenWidth),
		static_cast<GLfloat>(ScreenHeight), 0.0f, -1.0f, 1.0f);
	auto generator = ParticleGenerator(particleShader, textureId, projection);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// per-frame time
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// update particle system
		generator.update(deltaTime);

		// input
		mouse_click(window, generator);

		// clear buffers
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw particles
		generator.draw();

		// swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void mouse_click(GLFWwindow* window, ParticleGenerator& generator) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		return;
	}
	generator.create(glm::vec2(curX, curY), 3);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_position_callback(GLFWwindow* window, double xposIn, double yposIn) {
	curX = static_cast<float>(xposIn);
	curY = static_cast<float>(yposIn);
}