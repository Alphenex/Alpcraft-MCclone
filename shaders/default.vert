#version 330 core

layout (location = 0) in uint compVertex;

out vec2 texCoord;

out float lightLevel;

uniform mat4 cameraMatrix;

uniform vec3 chunkPosition;

vec2 TexCoordList[4] = vec2[4](
	vec2(0.0, 0.0),
	vec2(0.125, 0.0),
	vec2(0.125, 0.125),
	vec2(0.0, 0.125)
);

void main()
{
	float x = float(compVertex & 0x3Fu) + chunkPosition.x;
    float y = float((compVertex & 0xFC0u) >> 6u) + chunkPosition.y;
    float z = float((compVertex & 0x3F000u) >> 12u) + chunkPosition.z;

	uint uvID = uint((compVertex & 0xC0000u) >> 18u);
	uint textureXID = uint((compVertex & 0x700000u) >> 20u);
	uint textureYID = uint((compVertex & 0x3800000u) >> 23u);

	float lightLVL = float((compVertex & 0x3C000000u) >> 26u);

	gl_Position = cameraMatrix * vec4(x, y, z, 1.0f);

	texCoord = TexCoordList[uvID] + vec2(textureXID, textureYID) * vec2(0.125, 0.125);
	lightLevel = clamp(lightLVL, 0, 15) / 15;
}