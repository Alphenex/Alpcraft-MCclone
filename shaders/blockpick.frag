#version 330 core

out vec4 FragColor;

in vec4 BlockColor;

void main()
{
	FragColor = BlockColor;
}