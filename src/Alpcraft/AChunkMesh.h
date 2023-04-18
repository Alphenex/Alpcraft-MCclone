#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "../glObj/VertexArray.h"
#include "../glObj/VertexBuffer.h"
#include "../glObj/IndexBuffer.h"
#include <vector>

class ChunkMesh {
public:
	ChunkMesh(glm::ivec3 _ChunkWorldPos);
	~ChunkMesh();

	void AddFace(GLubyte _Side, glm::ivec3 _BlockPosition, glm::ivec2 _TexturePos, unsigned int LightLevel);
	void DeleteMesh();

	void PrepareMesh();
	void RenderMesh(GLuint& _ShaderID);

	// Pseudo Index Count is for creating the mesh,
	// meanwhile Index Count is the actual amount.
	int PseudoIndCount = 0;
	int IndCount = 0;

	std::vector<GLuint> MeshVertex;
	std::vector<GLuint> MeshIndex;
	bool IsArrayValid = false;
private:
	glm::ivec3 ParentChunkPosition;
	VAO MeshVAO;
};