#include "shaders.h"

int success;
char infoLog[512];

std::string shaders::LoadShaderProgram(const std::string& filename) {
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

unsigned int shaders::CreateVertexShader() {
	std::string vertexShaderSourceStr = LoadShaderProgram("./shaders/vertexShader.glsl");
	const char* vertexShaderSource = vertexShaderSourceStr.c_str();

	// Vertex shader 
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check for errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	return vertexShader;
}

unsigned int shaders::CreateFragShader() {
	std::string fragmentShaderSourceStr = LoadShaderProgram("./shaders/fragmentShader.glsl");
	const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();

	// Fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Check for errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::SHADER::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	return fragmentShader;
}

unsigned int shaders::LinkShaders() {
	// Vertex shader 
	unsigned int vertexShader = CreateVertexShader();

	// Fragment shader
	unsigned int fragmentShader = CreateFragShader();

	// Linking the shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);     // Attaching vertex shader with shader program
	glAttachShader(shaderProgram, fragmentShader);   // Attaching fragment shader with shader program
	glLinkProgram(shaderProgram);                    // Linking the above attached shaders with the program

	// Check for errors
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

