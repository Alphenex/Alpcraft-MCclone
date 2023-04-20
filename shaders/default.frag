#version 330 core

out vec4 FragColor;

in vec2 texCoord;

in float lightLevel;

uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, texCoord) * vec4(lightLevel, lightLevel, lightLevel, 1.0f);
}