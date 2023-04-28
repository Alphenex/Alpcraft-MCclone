#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"

#include <vector>

#include "AChunkMesh.h"
#include "ABlockData.h"
#include <queue>

#define CHUNK_SIZE 32 // Cubic Chunk Size

#define MAX_BLOCK_AMOUNT 32768

#define CIndex(x, y, z) ((x * CHUNK_SIZE) * CHUNK_SIZE) + (y * CHUNK_SIZE) + z // Index to get array position with individual position
#define CIndexV(vpos) ((vpos.x * CHUNK_SIZE) * CHUNK_SIZE) + (vpos.y * CHUNK_SIZE) + vpos.z // Index to get array position with vector

constexpr bool IsBlockInBound(glm::ivec3 blockpos)
{
	if (blockpos.x >= 0 && blockpos.x < CHUNK_SIZE &&
		blockpos.y >= 0 && blockpos.y < CHUNK_SIZE &&
		blockpos.z >= 0 && blockpos.z < CHUNK_SIZE)
	{
		return true;
	}

	return false;
}

enum BlockFace : GLubyte {
	FrontFace,	// Z+
	BackFace,	// Z-
	RightFace,	// X+
	LeftFace,	// X-
	UpFace,		// Y+
	DownFace,	// Y-
};

class World;

class Chunk {
public:
	Chunk(glm::ivec3 _ChunkNormalPos, World* _GlobalWorld);
	~Chunk();

	// Chunk Positions
	glm::ivec3 GetNormalPos() { return ChunkNormalPosition; }
	glm::ivec3 GetWorldPos() { return ChunkWorldPosition; }

	// Block related methods //

	Block GetBlock(glm::ivec3 _BlockPosition);
	Block GetNeighbourBlock(glm::ivec3 _BlockPosition, glm::ivec3 _BlockDirection);
	void SetBlock(glm::ivec3 _BlockPosition, Block _TypeID);
	void RemoveBlock(glm::ivec3 _BlockPosition);

	GLubyte GetTorchLightLevel(glm::ivec3 _BlockPosition);
	GLubyte GetNeighbourTorchLightLevel(glm::ivec3 _BlockPosition, glm::ivec3 _BlockDirection);
	GLubyte GetSunLightLevel(glm::ivec3 _BlockPosition);
	GLubyte GetNeighbourSunLightLevel(glm::ivec3 _BlockPosition, glm::ivec3 _BlockDirection);

	void SetTorchLightLevel(glm::ivec3 _BlockPosition, GLubyte _LightLevel);
	void SetSunLightLevel(glm::ivec3 _BlockPosition, GLubyte _LightLevel);

	void AddTorchlight(glm::ivec3 _LightPosition, GLubyte _LightLevel);
	void RemoveTorchLight(glm::ivec3 _LightPosition);

	void PropagateLights();

	// Mesh creation and rendering.
	void MeshCreate();

	void Render(GLuint& _ShaderID);
	void RenderTransparent(GLuint& _ShaderID);

	ChunkMesh* Mesh;
	ChunkMesh* TMesh;
private:
	Block ChunkBlocks[MAX_BLOCK_AMOUNT]; // 32768 because a chunk is 32^3, thus 32x32x32.
	GLubyte LightMap[MAX_BLOCK_AMOUNT];

	std::queue<LightNode> LightNodeQueue;
	std::queue<LightNode> LightRemovalList;

	// We store both of them so we deal with less problems of negative positions.
	glm::ivec3 ChunkWorldPosition;
	glm::ivec3 ChunkNormalPosition;

	World* GlobalWorld;
};