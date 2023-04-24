#include "AChunk.h"
#include "AWorld.h"
#include <algorithm>
#include <SFML/Graphics.hpp>


Chunk::Chunk(glm::ivec3 _ChunkNormalPos, World* _GlobalWorld)
{
	ChunkNormalPosition = _ChunkNormalPos;
	ChunkWorldPosition = _ChunkNormalPos * CHUNK_SIZE;

	Mesh = new ChunkMesh(ChunkWorldPosition);
	TMesh = new ChunkMesh(ChunkWorldPosition);

	GlobalWorld = _GlobalWorld;

	memset(LightMap, 0, sizeof(LightMap));
}

Chunk::~Chunk()
{
	State = UnInitialized;
	delete Mesh;
	delete TMesh;
}

Block Chunk::GetBlock(glm::ivec3 _BlockPosition)
{
	if (IsInBound(_BlockPosition))
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

Block Chunk::GetNeighbourBlock(glm::ivec3 _BlockPosition, glm::ivec3 _BlockDirection)
{
	if (GlobalWorld->GetChunk(ChunkNormalPosition + _BlockDirection) == nullptr) return Grass;

	return GlobalWorld->GetChunk(ChunkNormalPosition + _BlockDirection)->GetBlock({ _BlockPosition.x, _BlockPosition.y, _BlockPosition.z });

	return Grass;
}

void Chunk::SetBlock(glm::ivec3 _BlockPosition, Block _TypeID)
{
	if (IsInBound(_BlockPosition))
	{
		ChunkBlocks[CIndexV(_BlockPosition)] = _TypeID;

		State = Outdated;
	}
}

void Chunk::RemoveBlock(glm::ivec3 _BlockPosition)
{
	if (IsInBound(_BlockPosition))
	{
		if (GetBlockIsLightSource(GetBlock(_BlockPosition)))
		{
			RemoveTorchLight(_BlockPosition);
		}

		ChunkBlocks[CIndexV(_BlockPosition)] = Air;

		State = Outdated;
	}
}

GLubyte Chunk::GetTorchLightLevel(glm::ivec3 _BlockPosition)
{
	if (IsInBound(_BlockPosition))
	{
		return (LightMap[CIndexV(_BlockPosition)] & 0xF0) >> 4;
	}

	if (_BlockPosition.z == CHUNK_SIZE)
		return GetNeighbourTorchLightLevel({ _BlockPosition.x , _BlockPosition.y, 0 }, { 0, 0, 1 });

	if (_BlockPosition.z == -1)
		return GetNeighbourTorchLightLevel({ _BlockPosition.x , _BlockPosition.y, 31 }, { 0, 0, -1 });

	if (_BlockPosition.x == CHUNK_SIZE)
		return GetNeighbourTorchLightLevel({ 0 , _BlockPosition.y, _BlockPosition.z }, { 1, 0, 0 });

	if (_BlockPosition.x == -1)
		return GetNeighbourTorchLightLevel({ 31, _BlockPosition.y, _BlockPosition.z }, { -1, 0, 0 });

	if (_BlockPosition.y == CHUNK_SIZE)
		return GetNeighbourTorchLightLevel({ _BlockPosition.x, 0, _BlockPosition.z }, { 0, 1, 0 });

	if (_BlockPosition.y == -1)
		return GetNeighbourTorchLightLevel({ _BlockPosition.x, 31, _BlockPosition.z }, { 0, -1, 0 });

	return 0;
}

GLubyte Chunk::GetNeighbourTorchLightLevel(glm::ivec3 _BlockPosition, glm::ivec3 _BlockDirection)
{
	if (GlobalWorld->GetChunk(ChunkNormalPosition + _BlockDirection) == nullptr) return 0;

	return GlobalWorld->GetChunk(ChunkNormalPosition + _BlockDirection)->GetTorchLightLevel({ _BlockPosition.x, _BlockPosition.y, _BlockPosition.z });

	return 0;
}

GLubyte Chunk::GetSunLightLevel(glm::ivec3 _BlockPosition)
{
	if (IsInBound(_BlockPosition))
	{
		return LightMap[CIndexV(_BlockPosition)] & 0xF;
	}

	if (_BlockPosition.z == CHUNK_SIZE)
		return GetNeighbourSunLightLevel({ _BlockPosition.x , _BlockPosition.y, 0 }, { 0, 0, 1 });

	if (_BlockPosition.z == -1)
		return GetNeighbourSunLightLevel({ _BlockPosition.x , _BlockPosition.y, 31 }, { 0, 0, -1 });

	if (_BlockPosition.x == CHUNK_SIZE)
		return GetNeighbourSunLightLevel({ 0 , _BlockPosition.y, _BlockPosition.z }, { 1, 0, 0 });

	if (_BlockPosition.x == -1)
		return GetNeighbourSunLightLevel({ 31, _BlockPosition.y, _BlockPosition.z }, { -1, 0, 0 });

	if (_BlockPosition.y == CHUNK_SIZE)
		return GetNeighbourSunLightLevel({ _BlockPosition.x, 0, _BlockPosition.z }, { 0, 1, 0 });

	if (_BlockPosition.y == -1)
		return GetNeighbourSunLightLevel({ _BlockPosition.x, 31, _BlockPosition.z }, { 0, -1, 0 });

	return 0;
}

GLubyte Chunk::GetNeighbourSunLightLevel(glm::ivec3 _BlockPosition, glm::ivec3 _BlockDirection)
{
	if (GlobalWorld->GetChunk(ChunkNormalPosition + _BlockDirection) == nullptr) return 0;

	return GlobalWorld->GetChunk(ChunkNormalPosition + _BlockDirection)->GetSunLightLevel({ _BlockPosition.x, _BlockPosition.y, _BlockPosition.z });

	return 0;
}

void Chunk::SetTorchLightLevel(glm::ivec3 _BlockPosition, GLubyte _LightLevel)
{
	if (IsInBound(_BlockPosition))
	{

		LightMap[CIndexV(_BlockPosition)] = (LightMap[CIndexV(_BlockPosition)] & 0xF) | (_LightLevel << 4);
	}

	if (_BlockPosition.z == CHUNK_SIZE)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ 0, 0, 1 }))->SetTorchLightLevel({ _BlockPosition.x , _BlockPosition.y, 0 }, _LightLevel);

	if (_BlockPosition.z == -1)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ 0, 0, -1 }))->SetTorchLightLevel({ _BlockPosition.x , _BlockPosition.y, 31 }, _LightLevel);

	if (_BlockPosition.x == CHUNK_SIZE)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ 1, 0, 0 }))->SetTorchLightLevel({ 0 , _BlockPosition.y, _BlockPosition.z }, _LightLevel);

	if (_BlockPosition.x == -1)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ -1, 0, 0 }))->SetTorchLightLevel({ 31, _BlockPosition.y, _BlockPosition.z }, _LightLevel);

	if (_BlockPosition.y == CHUNK_SIZE)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ 0, 1, 0 }))->SetTorchLightLevel({ _BlockPosition.x, 0, _BlockPosition.z }, _LightLevel);

	if (_BlockPosition.y == -1)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ 0, -1, 0 }))->SetTorchLightLevel({ _BlockPosition.x, 31, _BlockPosition.z }, _LightLevel);
}

void Chunk::SetSunLightLevel(glm::ivec3 _BlockPosition, GLubyte _LightLevel)
{
	if (IsInBound(_BlockPosition))
	{

		LightMap[CIndexV(_BlockPosition)] = (LightMap[CIndexV(_BlockPosition)] & 0xF0) | _LightLevel;
	}

	if (_BlockPosition.z == CHUNK_SIZE)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ 0, 0, 1 }))->SetSunLightLevel({ _BlockPosition.x , _BlockPosition.y, 0 }, _LightLevel);

	if (_BlockPosition.z == -1)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ 0, 0, -1 }))->SetSunLightLevel({ _BlockPosition.x , _BlockPosition.y, 31 }, _LightLevel);

	if (_BlockPosition.x == CHUNK_SIZE)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ 1, 0, 0 }))->SetSunLightLevel({ 0 , _BlockPosition.y, _BlockPosition.z }, _LightLevel);

	if (_BlockPosition.x == -1)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ -1, 0, 0 }))->SetSunLightLevel({ 31, _BlockPosition.y, _BlockPosition.z }, _LightLevel);

	if (_BlockPosition.y == CHUNK_SIZE)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ 0, 1, 0 }))->SetSunLightLevel({ _BlockPosition.x, 0, _BlockPosition.z }, _LightLevel);

	if (_BlockPosition.y == -1)
		GlobalWorld->GetChunk(ChunkNormalPosition + glm::ivec3({ 0, -1, 0 }))->SetSunLightLevel({ _BlockPosition.x, 31, _BlockPosition.z }, _LightLevel);
}

void Chunk::AddTorchlight(glm::ivec3 _LightPosition, GLubyte _LightLevel)
{
	LightNode node(_LightPosition, _LightLevel);
	LightNodeQueue.push(node);
	SetTorchLightLevel(_LightPosition, _LightLevel);
	State = Outdated;
}

void Chunk::RemoveTorchLight(glm::ivec3 _LightPosition)
{
	LightNode node(_LightPosition, GetTorchLightLevel(_LightPosition));
	LightRemovalList.push(node);
	SetTorchLightLevel(_LightPosition, 0);
	State = Outdated;
}

void Chunk::PropagateLights()
{
	//if (GetNormalPos().y >= 0)
	//{
	//	for (int x = 0; x < CHUNK_SIZE; x++)
	//		for (int z = 0; z < CHUNK_SIZE; z++)
	//		{
	//			int SunLightLevel = 15;

	//			for (int y = CHUNK_SIZE - 1; y >= 0; y--)
	//			{
	//				Block block = GetBlock({ x, y, z });

	//				if (block != Air && !GetBlockTransparency(block)) { SunLightLevel = 10; SetTorchLightLevel({ x, y, z }, 0); continue; }

	//				SetTorchLightLevel({ x, y, z }, SunLightLevel);
	//			}
	//		}
	//}

	for (int x = 0; x < CHUNK_SIZE; x++)
	for (int y = 0; y < CHUNK_SIZE; y++)
	for (int z = 0; z < CHUNK_SIZE; z++)
	{
		if (GetBlockIsLightSource(GetBlock({x, y, z})))
		{
			RemoveTorchLight({ x, y ,z }); // We REMOVE the previous one and add a new one!
			AddTorchlight({x, y, z}, 15);
		}
	}

	while (!LightRemovalList.empty())
	{
		LightNode& Node = LightRemovalList.front();
		glm::ivec3 NodePosition = Node.NodePosition;
		int NodeLightLevel = Node.LightLevel;

		LightRemovalList.pop();

		for (int d = 0; d < 6; d++)
		{
			glm::ivec3 NeighbourPosition = NodePosition + Dir2Vec3(d);

			Block NeighbourBlock = GetBlock(NeighbourPosition);
			int NeighbourLightLevel = GetTorchLightLevel(NeighbourPosition);

			if (IsInBound(NeighbourPosition))
			{
				if (NeighbourLightLevel != 0 && NeighbourLightLevel < NodeLightLevel)
				{
					RemoveTorchLight(NeighbourPosition);
				}
				else if (NeighbourLightLevel >= NodeLightLevel)
				{
					AddTorchlight(NeighbourPosition, NeighbourLightLevel);
				}
			}

			// NEIGHBOUR CHUNK MODIFICATIONS

			glm::ivec3 NeighbourWorldPos = ChunkWorldPosition + NeighbourPosition;
			Chunk* ChunkNeighbour = GlobalWorld->GetChunk(ChunkNormalPosition + Dir2Vec3(d));

			if (!ChunkNeighbour) continue;

			{
				if (NeighbourPosition.x == -1)
				{
					NeighbourBlock = ChunkNeighbour->GetBlock({ 31, NeighbourPosition.y, NeighbourPosition.z });
					NeighbourLightLevel = ChunkNeighbour->GetTorchLightLevel({ 31, NeighbourPosition.y, NeighbourPosition.z });

					if (NeighbourLightLevel != 0 && NeighbourLightLevel < NodeLightLevel)
					{
						ChunkNeighbour->RemoveTorchLight({ 31, NeighbourPosition.y, NeighbourPosition.z });
					}
					else if (NeighbourLightLevel >= NodeLightLevel)
					{
						ChunkNeighbour->AddTorchlight({ 31, NeighbourPosition.y, NeighbourPosition.z }, NeighbourLightLevel);
					}

				}

				if (NeighbourPosition.x == 32)
				{
					NeighbourBlock = ChunkNeighbour->GetBlock({ 0, NeighbourPosition.y, NeighbourPosition.z });
					NeighbourLightLevel = ChunkNeighbour->GetTorchLightLevel({ 0, NeighbourPosition.y, NeighbourPosition.z });
					if (NeighbourLightLevel != 0 && NeighbourLightLevel < NodeLightLevel)
					{
						ChunkNeighbour->RemoveTorchLight({ 0, NeighbourPosition.y, NeighbourPosition.z });
					}
					else if (NeighbourLightLevel >= NodeLightLevel)
					{
						ChunkNeighbour->AddTorchlight({ 0, NeighbourPosition.y, NeighbourPosition.z }, NeighbourLightLevel);
					}

				}

				if (NeighbourPosition.y == -1)
				{
					NeighbourBlock = ChunkNeighbour->GetBlock({ NeighbourPosition.x, 31, NeighbourPosition.z });
					NeighbourLightLevel = ChunkNeighbour->GetTorchLightLevel({ NeighbourPosition.x, 31, NeighbourPosition.z });
					if (NeighbourLightLevel != 0 && NeighbourLightLevel < NodeLightLevel)
					{
						ChunkNeighbour->RemoveTorchLight({ NeighbourPosition.x, 31, NeighbourPosition.z });
					}
					else if (NeighbourLightLevel >= NodeLightLevel)
					{
						ChunkNeighbour->AddTorchlight({ NeighbourPosition.x, 31, NeighbourPosition.z }, NeighbourLightLevel);
					}

				}

				if (NeighbourPosition.y == 32)
				{
					NeighbourBlock = ChunkNeighbour->GetBlock({ NeighbourPosition.x, 0, NeighbourPosition.z });
					NeighbourLightLevel = ChunkNeighbour->GetTorchLightLevel({ NeighbourPosition.x, 0, NeighbourPosition.z });
					if (NeighbourLightLevel != 0 && NeighbourLightLevel < NodeLightLevel)
					{
						ChunkNeighbour->RemoveTorchLight({ NeighbourPosition.x, 0, NeighbourPosition.z });
					}
					else if (NeighbourLightLevel >= NodeLightLevel)
					{
						ChunkNeighbour->AddTorchlight({ NeighbourPosition.x, 0, NeighbourPosition.z }, NeighbourLightLevel);
					}

				}

				if (NeighbourPosition.z == -1)
				{
					NeighbourBlock = ChunkNeighbour->GetBlock({ NeighbourPosition.x, NeighbourPosition.y, 31 });
					NeighbourLightLevel = ChunkNeighbour->GetTorchLightLevel({ NeighbourPosition.x, NeighbourPosition.y, 31 });
					if (NeighbourLightLevel != 0 && NeighbourLightLevel < NodeLightLevel)
					{
						ChunkNeighbour->RemoveTorchLight({ NeighbourPosition.x, NeighbourPosition.y, 31 });
					}
					else if (NeighbourLightLevel >= NodeLightLevel)
					{
						ChunkNeighbour->AddTorchlight({ NeighbourPosition.x, NeighbourPosition.y, 31 }, NeighbourLightLevel);
					}

				}

				if (NeighbourPosition.z == 32)
				{
					NeighbourBlock = ChunkNeighbour->GetBlock({ NeighbourPosition.x, NeighbourPosition.y, 0 });
					NeighbourLightLevel = ChunkNeighbour->GetTorchLightLevel({ NeighbourPosition.x, NeighbourPosition.y, 0 });
					if (NeighbourLightLevel != 0 && NeighbourLightLevel < NodeLightLevel)
					{
						ChunkNeighbour->RemoveTorchLight({ NeighbourPosition.x, NeighbourPosition.y, 0 });
					}
					else if (NeighbourLightLevel >= NodeLightLevel)
					{
						ChunkNeighbour->AddTorchlight({ NeighbourPosition.x, NeighbourPosition.y, 0 }, NeighbourLightLevel);
					}

				}
			}
		}
	}

	while (!LightNodeQueue.empty())
	{
		LightNode& Node = LightNodeQueue.front();
		glm::ivec3 NodePosition = Node.NodePosition;
		int NodeLightLevel = GetTorchLightLevel(NodePosition);

		LightNodeQueue.pop();

		for (int d = 0; d < 6; d++)
		{
			glm::ivec3 NeighbourPosition = NodePosition + Dir2Vec3(d);

			Block NeighbourBlock = GetBlock(NeighbourPosition);
			int NeighbourLightLevel = GetTorchLightLevel(NeighbourPosition);

			if (IsInBound(NeighbourPosition))
			{
				if ((NeighbourBlock == Air || GetBlockTransparency(NeighbourBlock)) && //Check Block transparency.
					NeighbourLightLevel + 2 <= NodeLightLevel)
				{
					AddTorchlight(NeighbourPosition, NodeLightLevel - 1);
				}
				continue;
			}

			// NEIGHBOUR CHUNK MODIFICATIONS

			glm::ivec3 NeighbourWorldPos = ChunkWorldPosition + NeighbourPosition;
			Chunk* ChunkNeighbour = GlobalWorld->GetChunk(ChunkNormalPosition + Dir2Vec3(d));

			if (!ChunkNeighbour) continue;

			{
				if (NeighbourPosition.x == -1)
				{
					NeighbourBlock = ChunkNeighbour->GetBlock({ 31, NeighbourPosition.y, NeighbourPosition.z });
					NeighbourLightLevel = ChunkNeighbour->GetTorchLightLevel({ 31, NeighbourPosition.y, NeighbourPosition.z });

					if ((NeighbourBlock == Air || GetBlockTransparency(NeighbourBlock)) && //Check Block transparency.
						NeighbourLightLevel + 2 <= NodeLightLevel)
					{
						ChunkNeighbour->AddTorchlight({ 31, NeighbourPosition.y, NeighbourPosition.z }, NodeLightLevel - 1);
					}
				}

				if (NeighbourPosition.x == 32)
				{
					NeighbourBlock = ChunkNeighbour->GetBlock({ 0, NeighbourPosition.y, NeighbourPosition.z });
					NeighbourLightLevel = ChunkNeighbour->GetTorchLightLevel({ 0, NeighbourPosition.y, NeighbourPosition.z });
					if ((NeighbourBlock == Air || GetBlockTransparency(NeighbourBlock)) && //Check Block transparency.
						NeighbourLightLevel + 2 <= NodeLightLevel)
					{
						ChunkNeighbour->AddTorchlight({ 0, NeighbourPosition.y, NeighbourPosition.z }, NodeLightLevel - 1);
					}
				}

				if (NeighbourPosition.y == -1)
				{
					NeighbourBlock = ChunkNeighbour->GetBlock({ NeighbourPosition.x, 31, NeighbourPosition.z });
					NeighbourLightLevel = ChunkNeighbour->GetTorchLightLevel({ NeighbourPosition.x, 31, NeighbourPosition.z });
					if ((NeighbourBlock == Air || GetBlockTransparency(NeighbourBlock)) && //Check Block transparency.
						NeighbourLightLevel + 2 <= NodeLightLevel)
					{
						ChunkNeighbour->AddTorchlight({ NeighbourPosition.x, 31, NeighbourPosition.z }, NodeLightLevel - 1);
					}
				}

				if (NeighbourPosition.y == 32)
				{
					NeighbourBlock = ChunkNeighbour->GetBlock({ NeighbourPosition.x, 0, NeighbourPosition.z });
					NeighbourLightLevel = ChunkNeighbour->GetTorchLightLevel({ NeighbourPosition.x, 0, NeighbourPosition.z });
					if ((NeighbourBlock == Air || GetBlockTransparency(NeighbourBlock)) && //Check Block transparency.
						NeighbourLightLevel + 2 <= NodeLightLevel)
					{
						ChunkNeighbour->AddTorchlight({ NeighbourPosition.x, 0, NeighbourPosition.z }, NodeLightLevel - 1);
					}
				}

				if (NeighbourPosition.z == -1)
				{
					NeighbourBlock = ChunkNeighbour->GetBlock({ NeighbourPosition.x, NeighbourPosition.y, 31 });
					NeighbourLightLevel = ChunkNeighbour->GetTorchLightLevel({ NeighbourPosition.x, NeighbourPosition.y, 31 });
					if ((NeighbourBlock == Air || GetBlockTransparency(NeighbourBlock)) && //Check Block transparency.
						NeighbourLightLevel + 2 <= NodeLightLevel)
					{
						ChunkNeighbour->AddTorchlight({ NeighbourPosition.x, NeighbourPosition.y, 31 }, NodeLightLevel - 1);
					}
				}

				if (NeighbourPosition.z == 32)
				{
					NeighbourBlock = ChunkNeighbour->GetBlock({ NeighbourPosition.x, NeighbourPosition.y, 0 });
					NeighbourLightLevel = ChunkNeighbour->GetTorchLightLevel({ NeighbourPosition.x, NeighbourPosition.y, 0 });
					if ((NeighbourBlock == Air || GetBlockTransparency(NeighbourBlock)) && //Check Block transparency.
						NeighbourLightLevel + 2 <= NodeLightLevel)
					{
						ChunkNeighbour->AddTorchlight({ NeighbourPosition.x, NeighbourPosition.y, 0 }, NodeLightLevel - 1);
					}
				}
			}
		}
	}
}

void Chunk::MeshCreate()
{
	Mesh->IndCount = 0;
	Mesh->PseudoIndCount = 0;
	TMesh->IndCount = 0;
	TMesh->PseudoIndCount = 0;

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
			glm::ivec3 NeighbourPosition = glm::ivec3(x, y, z) + Dir2Vec3(d);
			Block NeighbourBlock = GetBlock(NeighbourPosition);

			BasicSides TextureFace = SideTexture;

			if (d == UpFace) TextureFace = TopTexture;
			else if (d == DownFace) TextureFace = BottomTexture;

			if (NeighbourBlock == Air || (!GetBlockTransparency(block) && GetBlockTransparency(NeighbourBlock)))
			{
				if (!GetBlockTransparency(block))
				{
					Mesh->AddFace(d, { x, y, z }, ReturnBlockTexture(block, TextureFace), GetTorchLightLevel(NeighbourPosition));
				}
				else
				{
					TMesh->AddFace(d, { x, y, z }, ReturnBlockTexture(block, TextureFace), GetTorchLightLevel(NeighbourPosition));
				}
			}
		}
	}
	Mesh->PrepareMesh();
	TMesh->PrepareMesh();
}

void Chunk::Render(GLuint& _ShaderID)
{
	Mesh->RenderMesh(_ShaderID);
}

void Chunk::RenderTransparent(GLuint& _ShaderID)
{
	TMesh->RenderMesh(_ShaderID);
	// This works for glasses as this is only like a mask but for blending for stuff like water we need to use the stuff above.
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0);
	//TMesh->RenderMesh(_ShaderID);
	//glDisable(GL_ALPHA_TEST);
}
