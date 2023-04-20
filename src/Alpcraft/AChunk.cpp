#include "AChunk.h"
#include "AWorld.h"
#include <algorithm>
#include <SFML/Graphics.hpp>


Chunk::Chunk(glm::ivec3 _ChunkNormalPos, World* _GlobalWorld)
{
	ChunkNormalPosition = _ChunkNormalPos;
	ChunkWorldPosition = _ChunkNormalPos * CHUNK_SIZE;

	Mesh = new ChunkMesh(ChunkWorldPosition);

	GlobalWorld = _GlobalWorld;

	memset(LightMap, 0, sizeof(LightMap));
}

Chunk::~Chunk()
{
	State = UnInitialized;
	delete Mesh;
}

Block Chunk::GetBlock(glm::ivec3 _BlockPosition)
{
	if (_BlockPosition.x >= 0 && _BlockPosition.x < CHUNK_SIZE &&
		_BlockPosition.y >= 0 && _BlockPosition.y < CHUNK_SIZE &&
		_BlockPosition.z >= 0 && _BlockPosition.z < CHUNK_SIZE)
	{
		return ChunkBlocks[CIndexV(_BlockPosition)];
	}

	if (_BlockPosition.z == CHUNK_SIZE)
		return GetNeighbourBlock({ _BlockPosition.x , _BlockPosition.y, 0 }, {0, 0, 1});

	if (_BlockPosition.z == -1)
		return GetNeighbourBlock({ _BlockPosition.x , _BlockPosition.y, 31 }, { 0, 0, -1 });

	if (_BlockPosition.x == CHUNK_SIZE)
		return GetNeighbourBlock({ 0 , _BlockPosition.y, _BlockPosition.z }, { 1, 0, 0 });

	if (_BlockPosition.x == -1)
		return GetNeighbourBlock({ 31, _BlockPosition.y, _BlockPosition.z }, { -1, 0, 0 });

	if (_BlockPosition.y == CHUNK_SIZE)
		return GetNeighbourBlock({ _BlockPosition.x, 0, _BlockPosition.z }, { 0, 1, 0 });

	if (_BlockPosition.y == -1)
		return GetNeighbourBlock({ _BlockPosition.x, 31, _BlockPosition.z }, { 0, -1, 0 });

	return Grass;
}

GLubyte Chunk::GetLightLevel(glm::ivec3 _BlockPosition)
{
	if (_BlockPosition.x >= 0 && _BlockPosition.x < CHUNK_SIZE &&
		_BlockPosition.y >= 0 && _BlockPosition.y < CHUNK_SIZE &&
		_BlockPosition.z >= 0 && _BlockPosition.z < CHUNK_SIZE)
	{
		return LightMap[CIndexV(_BlockPosition)];
	}

	if (_BlockPosition.z == CHUNK_SIZE)
		return GetNeighbourLightLevel({ _BlockPosition.x , _BlockPosition.y, 0 }, { 0, 0, 1 });

	if (_BlockPosition.z == -1)
		return GetNeighbourLightLevel({ _BlockPosition.x , _BlockPosition.y, 31 }, { 0, 0, -1 });

	if (_BlockPosition.x == CHUNK_SIZE)
		return GetNeighbourLightLevel({ 0 , _BlockPosition.y, _BlockPosition.z }, { 1, 0, 0 });

	if (_BlockPosition.x == -1)
		return GetNeighbourLightLevel({ 31, _BlockPosition.y, _BlockPosition.z }, { -1, 0, 0 });

	if (_BlockPosition.y == CHUNK_SIZE)
		return GetNeighbourLightLevel({ _BlockPosition.x, 0, _BlockPosition.z }, { 0, 1, 0 });

	if (_BlockPosition.y == -1)
		return GetNeighbourLightLevel({ _BlockPosition.x, 31, _BlockPosition.z }, { 0, -1, 0 });

	return 0;
}

Block Chunk::GetNeighbourBlock(glm::ivec3 _BlockPosition, glm::ivec3 _BlockDirection)
{
	if (GlobalWorld->GetChunk(ChunkNormalPosition + _BlockDirection) == nullptr) return Grass;

	return GlobalWorld->GetChunk(ChunkNormalPosition + _BlockDirection)->GetBlock({_BlockPosition.x, _BlockPosition.y, _BlockPosition.z});

	return Grass;
}

GLubyte Chunk::GetNeighbourLightLevel(glm::ivec3 _BlockPosition, glm::ivec3 _BlockDirection)
{
	if (GlobalWorld->GetChunk(ChunkNormalPosition + _BlockDirection) == nullptr) return 0;

	return GlobalWorld->GetChunk(ChunkNormalPosition + _BlockDirection)->GetLightLevel({ _BlockPosition.x, _BlockPosition.y, _BlockPosition.z });

	return 0;
}

void Chunk::SetBlock(glm::ivec3 _BlockPosition, Block _TypeID)
{
	if (_BlockPosition.x >= 0 && _BlockPosition.x < CHUNK_SIZE &&
		_BlockPosition.y >= 0 && _BlockPosition.y < CHUNK_SIZE &&
		_BlockPosition.z >= 0 && _BlockPosition.z < CHUNK_SIZE)
	{
		ChunkBlocks[CIndexV(_BlockPosition)] = _TypeID;

		State = Outdated;
	}
}

void Chunk::RemoveBlock(glm::ivec3 _BlockPosition)
{
	if (_BlockPosition.x >= 0 && _BlockPosition.x < CHUNK_SIZE &&
		_BlockPosition.y >= 0 && _BlockPosition.y < CHUNK_SIZE &&
		_BlockPosition.z >= 0 && _BlockPosition.z < CHUNK_SIZE)
	{
		ChunkBlocks[CIndexV(_BlockPosition)] = Air;

		State = Outdated;
	}
}

void Chunk::SetLightLevel(glm::ivec3 _BlockPosition, GLubyte _LightLevel)
{
	if (_BlockPosition.x >= 0 && _BlockPosition.x < CHUNK_SIZE &&
		_BlockPosition.y >= 0 && _BlockPosition.y < CHUNK_SIZE &&
		_BlockPosition.z >= 0 && _BlockPosition.z < CHUNK_SIZE)
	{

		LightMap[CIndexV(_BlockPosition)] = _LightLevel;
	}

	if (_BlockPosition.z == CHUNK_SIZE)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ 0, 0, 1 }))->SetLightLevel({ _BlockPosition.x , _BlockPosition.y, 0 }, _LightLevel);

	if (_BlockPosition.z == -1)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ 0, 0, -1 }))->SetLightLevel({ _BlockPosition.x , _BlockPosition.y, 31 }, _LightLevel);

	if (_BlockPosition.x == CHUNK_SIZE)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ 1, 0, 0 }))->SetLightLevel({ 0 , _BlockPosition.y, _BlockPosition.z }, _LightLevel);

	if (_BlockPosition.x == -1)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ -1, 0, 0 }))->SetLightLevel({ 31, _BlockPosition.y, _BlockPosition.z }, _LightLevel);

	if (_BlockPosition.y == CHUNK_SIZE)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ 0, 1, 0 }))->SetLightLevel({ _BlockPosition.x, 0, _BlockPosition.z }, _LightLevel);

	if (_BlockPosition.y == -1)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ 0, -1, 0 }))->SetLightLevel({ _BlockPosition.x, 31, _BlockPosition.z }, _LightLevel);
}

void Chunk::RemoveLight(glm::ivec3 _BlockPosition)
{

}

void Chunk::PropagateLights()
{
	if (GetNormalPos().y >= 0)
	{
		for (int x = 0; x < CHUNK_SIZE; x++)
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				int SunLightLevel = 15;

				for (int y = CHUNK_SIZE - 1; y >= 0; y--)
				{
					if (GetBlock({ x, y, z }) != Air) { SunLightLevel = 10; SetLightLevel({ x, y, z }, 0); continue; }

					SetLightLevel({ x, y, z }, SunLightLevel);
				}
			}
	}
	else
	{
		for (int x = 0; x < CHUNK_SIZE; x++)
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				int SunLightLevel = GlobalWorld->GetChunk(GetNormalPos() + glm::ivec3(0, 1, 0))->GetLightLevel({x, 0, z});

				for (int y = CHUNK_SIZE - 1; y >= 0; y--)
				{
					if (GetBlock({ x, y, z }) != Air) { SunLightLevel = 10; SetLightLevel({ x, y, z }, 0); continue; }

					SetLightLevel({ x, y, z }, SunLightLevel);
				}
			}
	}
}

void Chunk::MeshCreate()
{
	Mesh->IndCount = 0;
	Mesh->PseudoIndCount = 0;

	State = Updated;

	PropagateLights();

	for (int x = 0; x < CHUNK_SIZE; x++)
	for (int y = 0; y < CHUNK_SIZE; y++)
	for (int z = 0; z < CHUNK_SIZE; z++)
	{
		if (GetBlock({x, y, z}) == Air) continue;

		Block block = GetBlock({ x, y, z });

		for (int d = 0; d < 6; d++)
		{
			glm::ivec3 NeighbourBlock = glm::ivec3(x, y, z) + Dir2Vec(d);

			BasicSides TextureFace = SideTexture;

			if (d == UpFace) TextureFace = TopTexture;
			else if (d == DownFace) TextureFace = BottomTexture;

			if (GetBlock(NeighbourBlock) == Air)
			{
				Mesh->AddFace(d, { x, y, z }, ReturnBlockTexture(block, TextureFace), GetLightLevel(NeighbourBlock));
			}
		}
	}
	Mesh->PrepareMesh();
}

void Chunk::Render(GLuint& _ShaderID)
{
	Mesh->RenderMesh(_ShaderID);
}