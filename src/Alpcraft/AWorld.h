#pragma once

#include <map>
#include <unordered_map>

#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtx/hash.hpp"
#include "AChunk.h"
#include <thread>

constexpr int WorldSize = 4;
constexpr int WorldOffset = WorldSize / 2;
constexpr int WorldArraySize = WorldSize + 1; // Because we need to take 0 into account.

constexpr glm::ivec3 WorldToBlockPos(glm::vec3 _WorldPosition)
{
	glm::ivec3 BlockPos = {0, 0, 0};
	BlockPos.x = _WorldPosition.x < 0 ? round((CHUNK_SIZE - 1 + ((int)_WorldPosition.x % CHUNK_SIZE)) % CHUNK_SIZE) : (int)_WorldPosition.x % CHUNK_SIZE;
	BlockPos.y = _WorldPosition.y < 0 ? round((CHUNK_SIZE - 1 + ((int)_WorldPosition.y % CHUNK_SIZE)) % CHUNK_SIZE) : (int)_WorldPosition.y % CHUNK_SIZE;
	BlockPos.z = _WorldPosition.z < 0 ? round((CHUNK_SIZE - 1 + ((int)_WorldPosition.z % CHUNK_SIZE)) % CHUNK_SIZE) : (int)_WorldPosition.z % CHUNK_SIZE;
	return BlockPos;
}

constexpr glm::ivec3 WorldToChunkPos(glm::vec3 _WorldPosition)
{
	glm::ivec3 ChunkPos = { 0, 0, 0 };
	ChunkPos.x = _WorldPosition.x < 0 ? (_WorldPosition.x - CHUNK_SIZE) / CHUNK_SIZE : _WorldPosition.x / CHUNK_SIZE;
	ChunkPos.y = _WorldPosition.y < 0 ? (_WorldPosition.y - CHUNK_SIZE) / CHUNK_SIZE : _WorldPosition.y / CHUNK_SIZE;
	ChunkPos.z = _WorldPosition.z < 0 ? (_WorldPosition.z - CHUNK_SIZE) / CHUNK_SIZE : _WorldPosition.z / CHUNK_SIZE;
	return ChunkPos;
}

constexpr bool IsChunkInBound(glm::ivec3 chunkpos)
{
	chunkpos += WorldOffset;

	if (chunkpos.x >= 0 && chunkpos.x < WorldArraySize &&
		chunkpos.y >= 0 && chunkpos.y < WorldArraySize &&
		chunkpos.z >= 0 && chunkpos.z < WorldArraySize)
	{
		return true;
	}

	return false;
}

constexpr bool IsWorldBlockInBound(glm::vec3 blockworldpos)
{
	int MaxSize = WorldSize * CHUNK_SIZE;

	if (blockworldpos.x >= 0 && blockworldpos.x < MaxSize - 1 && // - 1 because we need to make sure that
		blockworldpos.y >= 0 && blockworldpos.y < MaxSize - 1 && // it works with the BlockMap of chunk objects.
		blockworldpos.z >= 0 && blockworldpos.z < MaxSize - 1)
	{
		return true;
	}

	return false;
}

#define WIndex(x, y, z) ((x * WorldArraySize) * WorldArraySize) + (y * WorldArraySize) + z // Index to get array position with individual position
#define WIndexV(vpos) ((vpos.x * WorldArraySize) * WorldArraySize) + (vpos.y * WorldArraySize) + vpos.z // Index to get array position with vector

class World {
public:
	World();
	~World();


	void CreateChunk(glm::ivec3 _ChunkPosition);

	void Update(glm::vec3 _PlayerPosition);
	void RenderChunks(GLuint& _ShaderID);

	Chunk* GetChunk(glm::ivec3 pos);
	Block GetWorldBlock();

	glm::ivec3 PlayerCurrentPos;
private:
	Chunk* WorldChunks[WorldArraySize * WorldArraySize * WorldArraySize]; // shush

	glm::ivec3 PlayerOlderPos = { 0, -12322, 0 };
};