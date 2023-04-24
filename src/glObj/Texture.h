#pragma once

#include "GL/glew.h"
#include <SFML/Graphics.hpp>
#include "Shader.h"

class Texture {
public:
	Texture(const char* image, GLenum textureType, GLenum slot, GLenum format, GLenum pixelType);
	Texture(sf::Image image, GLenum textureType, GLenum slot, GLenum format, GLenum pixelType);

	void TextureUniform(Shader& shader, const char* uniform, GLuint unit);

	void Bind();
	void UnBind();

	void Delete();

	GLuint ID;
	GLenum type;
};