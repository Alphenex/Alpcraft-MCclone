#include "VertexArray.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::GenVertexArray()
{
	glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO& vbo, GLuint layout, GLuint numComps, GLenum type, GLsizeiptr stride, void* offset)
{
	vbo.Bind();

	glVertexAttribPointer(layout, numComps, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);

	vbo.UnBind();
}

void VAO::LinkAttribI(VBO& vbo, GLuint layout, GLuint numComps, GLenum type, GLsizeiptr stride, void* offset)
{
	vbo.Bind();

	glVertexAttribIPointer(layout, numComps, type, stride, offset);
	glEnableVertexAttribArray(layout);

	vbo.UnBind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::UnBind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}