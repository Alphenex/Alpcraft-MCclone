#pragma once

#include <GL/glew.h>

class EBO {
public:
	EBO(GLuint* indices, GLsizeiptr size);
	EBO(GLubyte* indices, GLsizeiptr size);

	void Bind();
	void UnBind();

	void Delete();

	GLuint ID;
};

