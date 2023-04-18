#version 330 core

layout (location = 0) in float vertexInfo;

out vec3 BlockColor;

uniform mat4 cameraMatrix;
uniform vec3 blockPosition;
uniform vec4 ublockColor;

void main()
{
	float x = blockPosition.x;
	float y = blockPosition.y;
	float z = blockPosition.z;

	gl_Position = cameraMatrix * vec4(x, y, z, 1.0f);
	BlockColor = ublockColor;
}