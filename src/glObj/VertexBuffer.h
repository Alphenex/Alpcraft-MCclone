#pragma once

#include <GL/glew.h>

class VBO {
public:
	VBO(GLfloat* vertices, GLsizeiptr size);
	VBO(GLuint* vertices, GLsizeiptr size);
	VBO(GLubyte* vertices, GLsizeiptr size);

	void Bind();
	void UnBind();

	void Delete();

	GLuint ID;
};

