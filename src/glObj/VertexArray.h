#pragma once

#include <GL/glew.h>

#include "VertexBuffer.h"

class VAO {
public:
	VAO();

	void GenVertexArray();
	void LinkAttrib(VBO& vbo, GLuint layout, GLuint numComps, GLenum type, GLsizeiptr stride, void* offset);
	void LinkAttribI(VBO& vbo, GLuint layout, GLuint numComps, GLenum type, GLsizeiptr stride, void* offset);

	void Bind();
	void UnBind();

	void Delete();

	GLuint ID;
};


