#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

const int SCR_WIDTH = 750;
const int SCR_HEIGHT = 750;

// ===| Load Shader Programs |==================================================================

std::string LoadShaderProgram(const std::string& filename) {
	std::string shaderStr = "";
	std::string line = "";

	std::ifstream myFile(filename.c_str());

	if (myFile.is_open()) {
		while (std::getline(myFile, line)) {      // Storing each line of a file until EOF
			shaderStr += line + '\n';             // Append each line of program (fix: use +=)
		}
		myFile.close();
	}
	return shaderStr;
}

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

	GLFWwindow* window = glfwCreateWindow(650, 650, "OpenGL Triangle Renderer", NULL, NULL);
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

// ===| Creating and linking Linker, Fragment Shaders to a Shader program |======================

static unsigned int CreateLinkShader() {
	std::string vertexShaderSourceStr = LoadShaderProgram("./shaders/vertexShader.glsl");
	std::string fragmentShaderSourceStr = LoadShaderProgram("./shaders/fragmentShader.glsl");

	const char* vertexShaderSource = vertexShaderSourceStr.c_str();
	const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();

	// Vertex shader 
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	// Fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::SHADER::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	// Linking the shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);     // Attaching vertex shader with shader program
	glAttachShader(shaderProgram, fragmentShader);   // Attaching fragment shader with shader program
	glLinkProgram(shaderProgram);                    // Linking the above attached shaders with the program

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::SHADER::LINKING_FAILED\n" << infoLog << "\n";
	}

	// Deleting individual shaders after linking
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

// ===| Generate and Bind VAO, VBO |=============================================================

static unsigned int GenerateBindArrayBuffer(unsigned int* VBO1, unsigned int* VBO2) {
	//Stores in CPU
	const std::vector<float> vertexCoords = {
		// x     y     z  
		-0.5f, -0.2f, 0.0f,
		0.5f, -0.2f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	const std::vector<float> vertexColors = {
		// r    g    b
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	//Transfer memory to GPU

	// Generate and bind VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate, bind, enable vertex atrribute and sending data for VBOs
	// For VBO1 : Coordinates
	glGenBuffers(1, VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO1);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertexCoords.size() * sizeof(float),
		vertexCoords.data(),
		GL_STATIC_DRAW
	);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// For VBO2 : Colors
	glGenBuffers(1, VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO2);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertexColors.size() * sizeof(float),
		vertexColors.data(),
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	

	//Unbind and disable after completion
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

		// draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

// =================================================================================================

int main() {

	GLFWwindow* window = Initialize();

	unsigned int shaderProgram = CreateLinkShader();

	std::vector<unsigned int> VBOs(2);
	unsigned int VAO = GenerateBindArrayBuffer(&VBOs[0], &VBOs[1]); // Pass VBO by pointer

	RenderLoop(window, shaderProgram, VAO);

	//Cleanup

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBOs[0]);
	glDeleteBuffers(1, &VBOs[1]);
	glDeleteProgram(shaderProgram);
	glfwTerminate();

	return 0;
}