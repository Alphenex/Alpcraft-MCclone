#include "AWorld.h"

#include "ABlockData.h"

World::World()
{
	INIT_Blocks();


}

World::~World()
{
	for (int c = 0; c < ChunkContainer.size(); c++)
	{
		delete ChunkContainer.at(c);
	}
}

Chunk* World::GetChunk(glm::ivec3 _ChunkPosition)
{
	return WorldChunks[_ChunkPosition];
}

Chunk* World::GetWorldChunk(glm::vec3 _ChunkWorldPosition)
{
	glm::ivec3 FWorldPos = floor(_ChunkWorldPosition);
	glm::ivec3 ChunkPos = WorldToChunkPos(FWorldPos);

	return GetChunk(ChunkPos);
}

Block World::GetWorldBlock(glm::vec3 _WorldPosition)
{
	glm::ivec3 BlockPos = WorldToBlockPos(_WorldPosition);
	glm::ivec3 ChunkPos = WorldToChunkPos(_WorldPosition);

	if (GetChunk(ChunkPos) != nullptr)
	{
		return GetChunk(ChunkPos)->GetBlock(BlockPos);
	}

	return Air;
}

int World::GetWorldTorchLight(glm::vec3 _WorldPosition)
{
	glm::ivec3 BlockPos = WorldToBlockPos(_WorldPosition);
	glm::ivec3 ChunkPos = WorldToChunkPos(_WorldPosition);

	if (GetChunk(ChunkPos) != nullptr)
	{
		return GetChunk(ChunkPos)->GetTorchLightLevel(BlockPos);
	}

	return 0;
}

void World::SetWorldBlock(glm::vec3 _WorldPosition, Block _TypeID)
{
	glm::ivec3 BlockPos = WorldToBlockPos(_WorldPosition);
	glm::ivec3 ChunkPos = WorldToChunkPos(_WorldPosition);

	if (GetChunk(ChunkPos) != nullptr)
	{
		GetChunk(ChunkPos)->SetBlock(BlockPos, _TypeID);
	}
}

void World::RemoveWorldBlock(glm::vec3 _WorldPosition)
{
	glm::ivec3 BlockPos = WorldToBlockPos(_WorldPosition);
	glm::ivec3 ChunkPos = WorldToChunkPos(_WorldPosition);

	if (GetChunk(ChunkPos) != nullptr && GetChunk(ChunkPos)->GetBlock(BlockPos) != Air)
	{
		GetChunk(ChunkPos)->RemoveBlock(BlockPos);
	}
}

void World::RemoveWorldTorchLight(glm::vec3 _WorldPosition)
{
	glm::ivec3 BlockPos = WorldToBlockPos(_WorldPosition);
	glm::ivec3 ChunkPos = WorldToChunkPos(_WorldPosition);

	if (GetChunk(ChunkPos) != nullptr)
	{
		GetChunk(ChunkPos)->RemoveTorchLight(BlockPos);
	}
}

void World::AddWorldTorchLight(glm::vec3 _WorldPosition, GLubyte _LightLevel)
{
	glm::ivec3 BlockPos = WorldToBlockPos(_WorldPosition);
	glm::ivec3 ChunkPos = WorldToChunkPos(_WorldPosition);

	if (GetChunk(ChunkPos) != nullptr)
	{
		GetChunk(ChunkPos)->AddTorchlight(BlockPos, _LightLevel);
	}
}

void World::PutChunk(glm::ivec3 _Position)
{
	if (GetChunk(_Position) == nullptr)
	{
		Chunk* chunk = new Chunk(_Position, this);

		if (_Position.y == 0)
		{
			for (int x = 0; x < CHUNK_SIZE; x++)
			for (int y = 0; y < CHUNK_SIZE - 16; y++)
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				chunk->SetBlock({ x, y, z }, Grass);
				chunk->SetBlock({ x, y-1, z }, Dirt);
				chunk->SetBlock({ x, y-4, z }, Cottonstone);
			}
		}
		else if (_Position.y < 0)
		{
			for (int x = 0; x < CHUNK_SIZE; x++)
				for (int y = 0; y < CHUNK_SIZE; y++)
					for (int z = 0; z < CHUNK_SIZE; z++)
					{
						chunk->SetBlock({ x, y, z }, Stone);
					}
		}

		WorldChunks[_Position] = chunk;
		ChunkContainer.push_back(chunk);
		OutdatedChunks.push(chunk);
	}
}

void World::UpdateChunkWNeighbours(glm::ivec3 _Position)
{
	glm::ivec3 Position = WorldToChunkPos(_Position);
	Chunk* chunk = GetChunk(Position);

	if (chunk)
	{
		chunk->PropagateLights();
		chunk->MeshCreate();
	}

	for (int d = 0; d < 6; d++)
	{
		Chunk* chunk = GetChunk(Position + Dir2Vec3(d));

		if (chunk)
		{
			chunk->PropagateLights();
			chunk->MeshCreate();
		}
	}
}

void World::Update(glm::vec3 _PlayerPosition)
{
	PlayerCurrentPos = (glm::ivec3)floor(_PlayerPosition) / CHUNK_SIZE;

	if (!OutdatedChunks.empty())
	{
		Chunk* chunk = OutdatedChunks.front();
		OutdatedChunks.pop();
		OutdatedChunks.push(chunk);

		chunk->MeshCreate();
	}

	if (PlayerCurrentPos == PlayerOlderPos) return;

	for (int x = PlayerCurrentPos.x - RenderDistanceH; x < PlayerCurrentPos.x + RenderDistanceH; x++)
	for (int y = PlayerCurrentPos.y - RenderDistanceV; y < PlayerCurrentPos.y + RenderDistanceV; y++)
	for (int z = PlayerCurrentPos.z - RenderDistanceH; z < PlayerCurrentPos.z + RenderDistanceH; z++)
	{
		PutChunk({ x, y, z });
	}
}

void World::RenderChunks(GLuint& _ShaderID)
{
	for (int c = 0; c < ChunkContainer.size(); c++)
	{
		ChunkContainer.at(c)->Render(_ShaderID);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	 //This works for glasses as this is only like a mask but for blending for stuff like water we need to use the stuff above.
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	for (int c = 0; c < ChunkContainer.size(); c++)
	{
		ChunkContainer.at(c)->RenderTransparent(_ShaderID);
	}
	glDisable(GL_ALPHA_TEST);

	glDisable(GL_BLEND);
}
