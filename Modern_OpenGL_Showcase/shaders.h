#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <glad/glad.h>

class shaders
{
public:
	static std::string LoadShaderProgram(const std::string& filename);
	static unsigned int CreateVertexShader();
	static unsigned int CreateFragShader();
	static unsigned int LinkShaders();
};

