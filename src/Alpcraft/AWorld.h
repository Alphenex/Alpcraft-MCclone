#pragma once

#include <map>
#include <unordered_map>

#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtx/hash.hpp"
#include "AChunk.h"
#include <thread>

#define RenderDistanceH 4
#define RenderDistanceV 2

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

constexpr int WorldMaxChunkAmount = (RenderDistanceH * RenderDistanceH * RenderDistanceV);

#define WIndex(x, y, z) x + RenderDistanceV * (y + RenderDistanceH * z)
#define WIndexV(vpos) vpos.x + RenderDistanceV * (vpos.y + RenderDistanceH * vpos.z)

class World {
public:
	World();
	~World();

	Chunk* GetChunk(glm::ivec3 _ChunkPosition);
	Chunk* GetWorldChunk(glm::vec3 _ChunkWorldPosition);

	Block GetWorldBlock(glm::vec3 _WorldPosition);
	int GetWorldTorchLight(glm::vec3 _WorldPosition);

	void SetWorldBlock(glm::vec3 _WorldPosition, Block _TypeID);
	void RemoveWorldBlock(glm::vec3 _WorldPosition);

	void RemoveWorldTorchLight(glm::vec3 _WorldPosition);
	void AddWorldTorchLight(glm::vec3 _WorldPosition, GLubyte _LightLevel);

	void PutChunk(glm::ivec3 _Position);
	void UpdateChunkWNeighbours(glm::ivec3 _Position);

	void Update(glm::vec3 _PlayerPosition);

	void RenderChunks(GLuint& _ShaderID);

	glm::ivec3 PlayerCurrentPos;
	std::queue<Chunk*>	OutdatedChunks;
private:
	std::unordered_map<glm::ivec3, Chunk*> WorldChunks;
	std::vector<Chunk*> ChunkContainer;
	Chunk* Chunks[WorldMaxChunkAmount];

	glm::ivec3 PlayerOlderPos = { 0, -12322, 0 };
};