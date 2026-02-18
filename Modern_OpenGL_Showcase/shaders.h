#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <glad/glad.h>

class shaders
{
public:
	shaders();
	std::string LoadShaderProgram(const std::string& filename);
	static unsigned int CreateLinkShader();
};

