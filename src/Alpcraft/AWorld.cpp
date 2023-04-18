#include "AWorld.h"

#include "ABlockData.h"

World::World()
{

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
	glm::ivec3 ChunkPos;

	glm::ivec3 FWorldPos = floor(_ChunkWorldPosition);

	ChunkPos.x = FWorldPos.x < 0 ? (FWorldPos.x - 32) / 32 : FWorldPos.x / 32;
	ChunkPos.y = FWorldPos.y < 0 ? (FWorldPos.y - 32) / 32 : FWorldPos.y / 32;
	ChunkPos.z = FWorldPos.z < 0 ? (FWorldPos.z - 32) / 32 : FWorldPos.z / 32;

	return GetChunk(ChunkPos);
}

Block World::GetWorldBlock(glm::vec3 _WorldPosition)
{
	glm::ivec3 BlockPos;
	glm::ivec3 ChunkPos;

	BlockPos.x = _WorldPosition.x < 0 ? round((CHUNK_SIZE - 1 + ((int)_WorldPosition.x % CHUNK_SIZE)) % CHUNK_SIZE) : (int)_WorldPosition.x % CHUNK_SIZE;
	BlockPos.y = _WorldPosition.y < 0 ? round((CHUNK_SIZE - 1 + ((int)_WorldPosition.y % CHUNK_SIZE)) % CHUNK_SIZE) : (int)_WorldPosition.y % CHUNK_SIZE;
	BlockPos.z = _WorldPosition.z < 0 ? round((CHUNK_SIZE - 1 + ((int)_WorldPosition.z % CHUNK_SIZE)) % CHUNK_SIZE) : (int)_WorldPosition.z % CHUNK_SIZE;

	ChunkPos.x = _WorldPosition.x < 0 ? (_WorldPosition.x - CHUNK_SIZE) / CHUNK_SIZE : _WorldPosition.x / CHUNK_SIZE;
	ChunkPos.y = _WorldPosition.y < 0 ? (_WorldPosition.y - CHUNK_SIZE) / CHUNK_SIZE : _WorldPosition.y / CHUNK_SIZE;
	ChunkPos.z = _WorldPosition.z < 0 ? (_WorldPosition.z - CHUNK_SIZE) / CHUNK_SIZE : _WorldPosition.z / CHUNK_SIZE;

	if (GetChunk(ChunkPos) != nullptr)
	{
		return GetChunk(ChunkPos)->GetBlock(BlockPos);
	}

	return Air;
}

void World::SetWorldBlock(glm::vec3 _WorldPosition, Block _TypeID)
{
	glm::ivec3 BlockPos;
	glm::ivec3 ChunkPos;

	BlockPos.x = _WorldPosition.x < 0 ? round((CHUNK_SIZE - 1 + ((int)_WorldPosition.x % CHUNK_SIZE)) % CHUNK_SIZE) : (int)_WorldPosition.x % CHUNK_SIZE;
	BlockPos.y = _WorldPosition.y < 0 ? round((CHUNK_SIZE - 1 + ((int)_WorldPosition.y % CHUNK_SIZE)) % CHUNK_SIZE) : (int)_WorldPosition.y % CHUNK_SIZE;
	BlockPos.z = _WorldPosition.z < 0 ? round((CHUNK_SIZE - 1 + ((int)_WorldPosition.z % CHUNK_SIZE)) % CHUNK_SIZE) : (int)_WorldPosition.z % CHUNK_SIZE;

	ChunkPos.x = _WorldPosition.x < 0 ? (_WorldPosition.x - CHUNK_SIZE) / CHUNK_SIZE : _WorldPosition.x / CHUNK_SIZE;
	ChunkPos.y = _WorldPosition.y < 0 ? (_WorldPosition.y - CHUNK_SIZE) / CHUNK_SIZE : _WorldPosition.y / CHUNK_SIZE;
	ChunkPos.z = _WorldPosition.z < 0 ? (_WorldPosition.z - CHUNK_SIZE) / CHUNK_SIZE : _WorldPosition.z / CHUNK_SIZE;

	if (GetChunk(ChunkPos) != nullptr)
	{
		GetChunk(ChunkPos)->SetBlock(BlockPos, _TypeID);
	}
}

void World::RemoveWorldBlock(glm::vec3 _WorldPosition)
{
	glm::ivec3 BlockPos;
	glm::ivec3 ChunkPos;

	BlockPos.x = _WorldPosition.x < 0 ? round((CHUNK_SIZE - 1 + ((int)_WorldPosition.x % CHUNK_SIZE)) % CHUNK_SIZE) : (int)_WorldPosition.x % CHUNK_SIZE;
	BlockPos.y = _WorldPosition.y < 0 ? round((CHUNK_SIZE - 1 + ((int)_WorldPosition.y % CHUNK_SIZE)) % CHUNK_SIZE) : (int)_WorldPosition.y % CHUNK_SIZE;
	BlockPos.z = _WorldPosition.z < 0 ? round((CHUNK_SIZE - 1 + ((int)_WorldPosition.z % CHUNK_SIZE)) % CHUNK_SIZE) : (int)_WorldPosition.z % CHUNK_SIZE;

	ChunkPos.x = _WorldPosition.x < 0 ? (_WorldPosition.x - CHUNK_SIZE) / CHUNK_SIZE : _WorldPosition.x / CHUNK_SIZE;
	ChunkPos.y = _WorldPosition.y < 0 ? (_WorldPosition.y - CHUNK_SIZE) / CHUNK_SIZE : _WorldPosition.y / CHUNK_SIZE;
	ChunkPos.z = _WorldPosition.z < 0 ? (_WorldPosition.z - CHUNK_SIZE) / CHUNK_SIZE : _WorldPosition.z / CHUNK_SIZE;

	if (GetChunk(ChunkPos) != nullptr && GetChunk(ChunkPos)->GetBlock(BlockPos) != Air)
	{
		GetChunk(ChunkPos)->RemoveBlock(BlockPos);
	}
}

void World::SetWorldLightInfo(glm::vec3 _WorldPosition, GLubyte _LightLevel)
{
	glm::ivec3 BlockPos;
	glm::ivec3 ChunkPos;

	BlockPos.x = _WorldPosition.x < 0 ? round((CHUNK_SIZE - 1 + ((int)_WorldPosition.x % CHUNK_SIZE)) % CHUNK_SIZE) : (int)_WorldPosition.x % CHUNK_SIZE;
	BlockPos.y = _WorldPosition.y < 0 ? round((CHUNK_SIZE - 1 + ((int)_WorldPosition.y % CHUNK_SIZE)) % CHUNK_SIZE) : (int)_WorldPosition.y % CHUNK_SIZE;
	BlockPos.z = _WorldPosition.z < 0 ? round((CHUNK_SIZE - 1 + ((int)_WorldPosition.z % CHUNK_SIZE)) % CHUNK_SIZE) : (int)_WorldPosition.z % CHUNK_SIZE;

	ChunkPos.x = _WorldPosition.x < 0 ? (_WorldPosition.x - CHUNK_SIZE) / CHUNK_SIZE : _WorldPosition.x / CHUNK_SIZE;
	ChunkPos.y = _WorldPosition.y < 0 ? (_WorldPosition.y - CHUNK_SIZE) / CHUNK_SIZE : _WorldPosition.y / CHUNK_SIZE;
	ChunkPos.z = _WorldPosition.z < 0 ? (_WorldPosition.z - CHUNK_SIZE) / CHUNK_SIZE : _WorldPosition.z / CHUNK_SIZE;

	//if (GetChunk(ChunkPos) != nullptr)
	//{
	//	GetChunk(ChunkPos)->SetLightLevel(BlockPos, _LightLevel);
	//	GetChunk(ChunkPos)->LightNodeQueue.push(BlockPos);
	//}
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
				chunk->SetBlock({ x, y-4, z }, CottonStone);

				if (glm::linearRand(0, 2) == 2)
				{
					chunk->SetBlock({ x, y + 1, z }, Grass);
					chunk->SetBlock({ x, y, z }, Dirt);
					if (glm::linearRand(0, 2) == 2)
					{
						chunk->SetBlock({ x, y + 2, z }, Grass);
						chunk->SetBlock({ x, y + 1, z}, Dirt);
					}
				}
			}

		} else if (_Position.y < 0)
		{
			for (int x = 0; x < CHUNK_SIZE; x++)
			for (int y = 0; y < CHUNK_SIZE; y++)
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				chunk->SetBlock({ x, y, z }, Stone);
			}
		}

		if (this->GetChunk(_Position + glm::ivec3(0, 0, 1)))
			this->GetChunk(_Position + glm::ivec3(0, 0, 1))->State = Outdated;
		if (this->GetChunk(_Position + glm::ivec3(0, 0, -1)))
			this->GetChunk(_Position + glm::ivec3(0, 0, -1))->State = Outdated;
		if (this->GetChunk(_Position + glm::ivec3(1, 0, 0)))
			this->GetChunk(_Position + glm::ivec3(1, 0, 0))->State = Outdated;
		if (this->GetChunk(_Position + glm::ivec3(-1, 0, 0)))
			this->GetChunk(_Position + glm::ivec3(-1, 0, 0))->State = Outdated;
		if (this->GetChunk(_Position + glm::ivec3(0, 1, 0)))
			this->GetChunk(_Position + glm::ivec3(0, 1, 0))->State = Outdated;
		if (this->GetChunk(_Position + glm::ivec3(0, -1, 0)))
			this->GetChunk(_Position + glm::ivec3(0, -1, 0))->State = Outdated;

		this->WorldChunks[_Position] = chunk;
		this->ChunkContainer.push_back(chunk);
		this->OutdatedChunks.push_front(chunk);
	}
}

void World::UpdateChunkWNeighbours(glm::ivec3 _Position)
{
	if (GetChunk(_Position))
		GetChunk(_Position)->MeshCreate();

	if (GetWorldChunk(_Position + glm::ivec3(0, 0, 1)))
		GetWorldChunk(_Position + glm::ivec3(0, 0, 1))->MeshCreate();
	if (GetWorldChunk(_Position + glm::ivec3(0, 0, -1)))
		GetWorldChunk(_Position + glm::ivec3(0, 0, -1))->MeshCreate();
	if (GetWorldChunk(_Position + glm::ivec3(1, 0, 0)))
		GetWorldChunk(_Position + glm::ivec3(1, 0, 0))->MeshCreate();
	if (GetWorldChunk(_Position + glm::ivec3(-1, 0, 0)))
		GetWorldChunk(_Position + glm::ivec3(-1, 0, 0))->MeshCreate();
	if (GetWorldChunk(_Position + glm::ivec3(0, 1, 0)))
		GetWorldChunk(_Position + glm::ivec3(0, 1, 0))->MeshCreate();
	if (GetWorldChunk(_Position + glm::ivec3(0, -1, 0)))
		GetWorldChunk(_Position + glm::ivec3(0, -1, 0))->MeshCreate();

}

void World::Update(glm::vec3 _PlayerPosition)
{
	PlayerCurrentPos.x = floor(_PlayerPosition.x) / CHUNK_SIZE;
	PlayerCurrentPos.y = floor(_PlayerPosition.y) / CHUNK_SIZE;
	PlayerCurrentPos.z = floor(_PlayerPosition.z) / CHUNK_SIZE;

	if (OutdatedChunks.size() > 0)
	{
		if (OutdatedChunks.at(0) != nullptr)
		{
			OutdatedChunks.at(0)->MeshCreate();
			OutdatedChunks.pop_front();
		}
		else
		{
			OutdatedChunks.pop_front();
		}
	}

	if (PlayerCurrentPos != PlayerOlderPos)
	{
		for (int x = PlayerCurrentPos.x - RenderDistanceH; x < PlayerCurrentPos.x + RenderDistanceH; x++)
		for (int y = PlayerCurrentPos.y - RenderDistanceV; y < PlayerCurrentPos.y + RenderDistanceV; y++)
		for (int z = PlayerCurrentPos.z - RenderDistanceH; z < PlayerCurrentPos.z + RenderDistanceH; z++)
		{
			PutChunk({ x, y, z });
		}

		for (int i = 0; i < ChunkContainer.size(); i++)
		{
			if (ChunkContainer.at(i)->State == Outdated)
			{
				OutdatedChunks.push_front(ChunkContainer.at(i));
				break;
			}
		}
	}
}

void World::RenderChunksInbound(GLuint& _ShaderID)
{
	for (int x = PlayerCurrentPos.x - RenderDistanceH; x < PlayerCurrentPos.x + RenderDistanceH; x++)
	for (int y = PlayerCurrentPos.y - RenderDistanceV; y < PlayerCurrentPos.y + RenderDistanceV; y++)
	for (int z = PlayerCurrentPos.z - RenderDistanceH; z < PlayerCurrentPos.z + RenderDistanceH; z++)
	{
		if (GetChunk({x, y, z}) != nullptr)
		{
			GetChunk({ x, y, z })->Render(_ShaderID);
		}
	}
}

void World::RenderChunks(GLuint& _ShaderID)
{
	for (int c = 0; c < ChunkContainer.size(); c++)
	{
		ChunkContainer.at(c)->Render(_ShaderID);
	}
}
