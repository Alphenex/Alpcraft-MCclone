#include "AWorld.h"

#include "ABlockData.h"

World::World()
{
	INIT_Blocks(); // Initializes every defined block in Blocks.cpp && obv Blocks.h
	memset(WorldChunks, NULL, sizeof(WorldChunks));

	WorldIterate([&](int cx, int cy, int cz) {
		Chunk* chunk = CreateChunk({ cx, cy, cz });

		if ( cy == 0 )
		{
			for (int bx = 0; bx < CHUNK_SIZE; bx++)
			for (int by = 0; by < CHUNK_SIZE / 2; by++)
			for (int bz = 0; bz < CHUNK_SIZE; bz++)
			{
				if (by == CHUNK_SIZE / 2 - 1)
				{
					chunk->SetBlock({ bx, by, bz }, Grass);
				}
				else if (by > CHUNK_SIZE / 2 - 5 && by < CHUNK_SIZE / 2 - 1)
				{
					chunk->SetBlock({ bx, by, bz }, Dirt);
				}
				else
				{
					chunk->SetBlock({ bx, by, bz }, Stone);
				}
			}
		}
		else if (cy < 0)
		{
			for (int bx = 0; bx < CHUNK_SIZE; bx++)
			for (int by = 0; by < CHUNK_SIZE; by++)
			for (int bz = 0; bz < CHUNK_SIZE; bz++)
			{
				chunk->SetBlock({ bx, by, bz }, Stone);
			}
		}

		}
	);

	WorldIterate([&](Chunk* chunk, int x, int y, int z) {
		if (chunk)
		{
			chunk->MeshCreate();
		}
		}
	);
}

World::~World()
{
	WorldIterate([&](Chunk* chunk, int x, int y, int z) {
		if (chunk)
		{
			delete chunk;
		}
		}
	);
}

void World::WorldIterate(WorldIterateFn1 fn)
{
	int __WorldOffsetMax = WorldOffset + 1;
	for (int x = -WorldOffset; x < __WorldOffsetMax; x++)
		for (int y = -WorldOffset; y < __WorldOffsetMax; y++)
			for (int z = -WorldOffset; z < __WorldOffsetMax; z++)
			{
				fn(GetChunk({ x, y, z }), x, y, z);
			}
}

void World::WorldIterate(WorldIterateFn2 fn)
{
	int __WorldOffsetMax = WorldOffset + 1;
	for (int x = -WorldOffset; x < __WorldOffsetMax; x++)
		for (int y = -WorldOffset; y < __WorldOffsetMax; y++)
			for (int z = -WorldOffset; z < __WorldOffsetMax; z++)
			{
				fn(x, y, z);
			}
}

Chunk* World::CreateChunk(glm::ivec3 _ChunkPosition)
{
	if (IsChunkInRenderDistance(_ChunkPosition, PlayerCurrentPos))
	{
		Chunk* chunk = new Chunk(_ChunkPosition, this);

		chunk->MeshCreate();

		glm::ivec3 poswoffset = _ChunkPosition + WorldOffset;
		WorldChunks[WIndexV(poswoffset)] = chunk;

		return chunk;
	}

	return nullptr;
}

void World::SetWorldBlock(glm::vec3 _BlockPos, Block _TypeID)
{
	glm::ivec3 blockpos = WorldToBlockPos(_BlockPos);
	glm::ivec3 chunkpos = WorldToChunkPos(_BlockPos);

	if (IsChunkInRenderDistance(chunkpos, PlayerCurrentPos))
	{
		GetChunk(chunkpos)->SetBlock(blockpos, _TypeID);
	}
}

void World::RemoveWorldBlock(glm::vec3 _BlockPos)
{
	glm::ivec3 blockpos = WorldToBlockPos(_BlockPos);
	glm::ivec3 chunkpos = WorldToChunkPos(_BlockPos);

	if (IsChunkInRenderDistance(chunkpos, PlayerCurrentPos))
	{
		GetChunk(chunkpos)->RemoveBlock(blockpos);
	}
}

void World::UpdateChunkNeighbour(glm::vec3 _ChunkPos)
{
	if (IsChunkInRenderDistance(_ChunkPos, PlayerCurrentPos))
	{
		Chunk* originchunk = GetChunk(_ChunkPos);
		originchunk->MeshCreate();

		for (int d = 0; d < 6; d++)
		{
			Chunk* chunk = GetChunk((glm::ivec3)_ChunkPos + Dir2Vec3(d));

			if (chunk)
			{
				chunk->PropagateLights();
				chunk->MeshCreate();
			}
		}
	}
}

void World::Update(glm::vec3 _PlayerPosition)
{
	PlayerCurrentPos = (glm::ivec3)floor(_PlayerPosition) / CHUNK_SIZE;

	if (PlayerCurrentPos == PlayerOlderPos) return;

	Chunk* WorldChunksOld[WorldArrayTotalSize];

	memcpy(WorldChunksOld, WorldChunks, sizeof(WorldChunksOld));
	memset(WorldChunks, NULL, sizeof(WorldChunks));


	for (int i = 0; i < WorldArrayTotalSize; i++)
	{
		Chunk* chunk = WorldChunksOld[i];

		if (chunk == NULL)
			continue;

		glm::ivec3 chunknormalpos = chunk->GetNormalPos();
		glm::ivec3 poswoffset = chunknormalpos + WorldOffset;

		if (IsChunkInRenderDistance(chunknormalpos, PlayerCurrentPos))
		{
			WorldChunks[WIndexV(poswoffset)] = chunk;
		}
		else
		{
			WorldChunks[WIndexV(poswoffset)] = NULL;
			delete chunk;
		}
	}

	PlayerOlderPos = PlayerCurrentPos;
}

void World::RenderChunks(GLuint& _ShaderID)
{
	WorldIterate([&](int x, int y, int z) {
		Chunk* chunk = GetChunk({ x, y, z });
		if (chunk)
		{
			chunk->Render(_ShaderID);
		}
		}
	);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	 //This works for glasses as this is only like a mask but for blending for stuff like water we need to use the stuff above.
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

	WorldIterate([&](int x, int y, int z) {
		Chunk* chunk = GetChunk({ x, y, z });
		if (chunk)
		{
			chunk->RenderTransparent(_ShaderID);
		}
		}
	);

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
}

Chunk* World::GetChunk(glm::ivec3 _ChunkPos)
{
	if (IsChunkInRenderDistance(_ChunkPos, PlayerCurrentPos))
	{
		glm::ivec3 poswoffset = _ChunkPos + WorldOffset;
		return WorldChunks[WIndexV(poswoffset)];
	}

	return nullptr;
}

Block World::GetWorldBlock(glm::vec3 _BlockPos)
{
	glm::ivec3 blockpos = WorldToBlockPos(_BlockPos);
	glm::ivec3 chunkpos = WorldToChunkPos(_BlockPos);

	Chunk* chunk = GetChunk(chunkpos);

	if (IsChunkInRenderDistance(chunkpos, PlayerCurrentPos) && chunk)
	{
		return chunk->GetBlock(blockpos);
	}

	return Grass;
}
