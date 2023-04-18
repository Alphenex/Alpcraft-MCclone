#pragma once

#include <GL/glew.h>

#include <string>
#include <fstream>

std::string GetFileContents(const char* filename);

class Shader {
public:
	Shader(const char* vertShader, const char* fragShader);

	void Activate();

	void Delete();

	GLuint ID;
};


