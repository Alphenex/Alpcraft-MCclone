#pragma once

#include <map>
#include <unordered_map>

#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtx/hash.hpp"
#include "AChunk.h"

#include <thread>
#include <future>
#include <deque>

#define RenderDistanceH 8
#define RenderDistanceV 2

class World {
public:
	World();
	~World();

	Chunk* GetChunk(glm::ivec3 _ChunkPosition);
	Chunk* GetWorldChunk(glm::vec3 _ChunkWorldPosition);

	Block GetWorldBlock(glm::vec3 _WorldPosition);
	void SetWorldBlock(glm::vec3 _WorldPosition, Block _TypeID);
	void RemoveWorldBlock(glm::vec3 _WorldPosition);
	void SetWorldLightInfo(glm::vec3 _WorldPosition, GLubyte _LightLevel);

	void PutChunk(glm::ivec3 _Position);
	void UpdateChunkWNeighbours(glm::ivec3 _Position);

	void Update(glm::vec3 _PlayerPosition);

	void RenderChunksInbound(GLuint& _ShaderID);
	void RenderChunks(GLuint& _ShaderID);

	glm::ivec3 PlayerCurrentPos;
private:
	std::unordered_map<glm::ivec3, Chunk*> WorldChunks;
	std::vector<Chunk*> ChunkContainer;
	std::deque<Chunk*> OutdatedChunks;

	glm::ivec3 PlayerOlderPos = { 0, -12322, 0 };
};