#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "shaders.h"

const int SCR_WIDTH = 750;
const int SCR_HEIGHT = 750;

// ===| Exit on pressing ESC key |==================================================================

static void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// ===| Set Viewport |==================================================================

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// ===| OpenGL Version Info |==================================================================

static void getOpenGLVerInfo() {
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}

// ===| Init GLFW, GLAD and Create new window |=================================================

static GLFWwindow* Initialize() {
	//Initialize glfw
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(650, 650, "OpenGL Renderer", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create new window";
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	return window;
}

// ===| Generate and Bind VAO, VBO |=============================================================

static unsigned int GenerateBindArrayBuffer(unsigned int* VBO) {
	//Stores in CPU
	const std::vector<float> vertexCoordsColors = { 
		// vertex 0
		-0.5f, -0.5f, 0.0f, // lower-left coordinates
		1.0f, 0.0f, 0.0f,   // color

		//vetex 1
		0.5f, -0.5f, 0.0f,  // lower-right coordinates
		0.0f, 1.0f, 0.0f,   // color

		// vertex 2
		-0.5f, 0.5f, 0.0f,   // top-left coordinates
		1.0f, 0.0f, 0.0f,    // color

		//vertex 3
		0.5f, 0.5f, 0.0f,    // top-right coordinates
		0.0f, 1.0f, 0.0f     // color
	};

	const std::vector<unsigned int> indices = {
		2, 0, 1,  // triangle 1
		3, 2, 1   // triangle 2
	};

	//Transfer memory to GPU

	// Generate and bind VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate, bind, enable vertex atrribute and sending data for VBO
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertexCoordsColors.size() * sizeof(float),
		vertexCoordsColors.data(),
		GL_STATIC_DRAW
	);

	// Setup Index buffer object (ie.EBO)
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(unsigned int),
		indices.data(),
		GL_STATIC_DRAW
	);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, 
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(GL_FLOAT)*6,  // Stride
		(void*)0
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, 
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(GL_FLOAT)*6,  // Stride
		(void*)(sizeof(GL_FLOAT)*3) // Offset
	);
	

	//bind and disable after completion
	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	return VAO;
}

// ===| Main Loop |===========================================================================

static void RenderLoop(GLFWwindow* window, unsigned int shaderProgram, unsigned int VAO) {

	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		// render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6);  only for drawing without index buffers

		// For drawing with index buffers, we need to use the below
		glDrawElements(
			GL_TRIANGLES,     // Mode
			6,                // No. of indices to be drawn
			GL_UNSIGNED_INT,  // Data type
			0                 // Offset
		);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

// =================================================================================================

int main() {

	GLFWwindow* window = Initialize();

	// Get version details here
	getOpenGLVerInfo();

	//Create object from shader class
	shaders* shader = new shaders();
	unsigned int shaderProgram = shader->LinkShaders();

	unsigned int VBO;
	unsigned int VAO = GenerateBindArrayBuffer(&VBO); // Pass VBO by pointer

	RenderLoop(window, shaderProgram, VAO);

	//Cleanup

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	delete(shader);

	glfwTerminate();

	return 0;
}