#include "AWorld.h"

#include "ABlockData.h"

World::World()
{
	INIT_Blocks(); // Initializes every defined block in Blocks.cpp && obv Blocks.h
	memset(WorldChunks, NULL, sizeof(WorldChunks));

}

World::~World()
{

}

void World::CreateChunk(glm::ivec3 _ChunkPosition)
{
	if (IsChunkInBound(_ChunkPosition))
	{
		Chunk* chunk = new Chunk(_ChunkPosition, this);

		for (int x = 0; x < CHUNK_SIZE; x++)
			for (int y = 0; y < CHUNK_SIZE / 2; y++)
				for (int z = 0; z < CHUNK_SIZE; z++)
				{
					chunk->SetBlock({ x, y, z }, Stone);
				}

		glm::ivec3 poswoffset = _ChunkPosition + WorldOffset;

		WorldChunks[WIndexV(poswoffset)] = chunk;
	}
}

void World::Update(glm::vec3 _PlayerPosition)
{
	PlayerCurrentPos = (glm::ivec3)floor(_PlayerPosition) / CHUNK_SIZE;

	if (PlayerCurrentPos == PlayerOlderPos) return;

}

void World::RenderChunks(GLuint& _ShaderID)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	 //This works for glasses as this is only like a mask but for blending for stuff like water we need to use the stuff above.
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);


	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
}

Chunk* World::GetChunk(glm::ivec3 pos)
{
	if (IsChunkInBound(pos))
	{
		glm::ivec3 poswoffset = pos + WorldOffset;
		return WorldChunks[WIndexV(poswoffset)];
	}

	return nullptr;
}

Block World::GetWorldBlock()
{
	return Air;
}
