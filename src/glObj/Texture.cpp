#include "Texture.h"

#include <SFML/Graphics.hpp>

Texture::Texture(const char* image, GLenum textureType, GLenum slot, GLenum format, GLenum pixelType)
{
    type = textureType;
    int widthTex, heightTex, numColCh;

    sf::Image* img = new sf::Image;
    img->loadFromFile(image);

    widthTex = img->getSize().x;
    heightTex = img->getSize().y;

    glGenTextures(1, &ID);

    glActiveTexture(slot);
    glBindTexture(textureType, ID);

    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(textureType, 0, GL_RGBA, widthTex, heightTex, 0, format, GL_UNSIGNED_BYTE, img->getPixelsPtr());
    glGenerateMipmap(textureType);

    glBindTexture(textureType, 0);
}


void Texture::TextureUniform(Shader& shader, const char* uniform, GLuint unit)
{
    GLuint texUniform = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();
    glUniform1i(texUniform, unit);
}

void Texture::Bind()
{
    glBindTexture(type, ID);
}

void Texture::UnBind()
{
    glBindTexture(type, 0);
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);
}
