#pragma once

#include <GL/glew.h>
#include <GLM/glm.hpp>

#include "../AChunkMesh.h"

struct BlockInfo;
enum Block : GLubyte;

extern BlockInfo BLOCKINFOS[256];
extern int BLOCKAMOUNT;

const char* GetBlockName(Block block);
bool GetBlockTransparency(Block block);
bool GetBlockIsLightSource(Block block);
GLubyte GetBlockLightEmitLevel(Block block);
BlockMeshType GetBlockMeshType(Block block);

struct BlockInfo {
	const char* BlockName = "NULL";
	Block BlockID = (Block)255;

	bool Transparency = false;
	bool LightSource = false;

	GLubyte LightEmitLevel = 0;

	BlockMeshType MeshType = CubeMesh;

	void Register()
	{
		if (MeshType == CrossMesh)
			Transparency = true;

		BLOCKINFOS[BlockID] = *this;
		BLOCKAMOUNT++;
	}
};

#define INIT(name) \
	extern void INIT_##name(); \
	INIT_##name();

inline void INIT_Blocks()
{
	INIT(Air);
	INIT(Grass);
	INIT(Dirt);
	INIT(Stone);
	INIT(Cottonstone);
	INIT(Bedrock);
	INIT(WoodLog);
	INIT(Glowstone);
	INIT(Water);
	INIT(Glass);
	INIT(TintedGlass);
	INIT(CraftingDesk);
	INIT(Lava);
	INIT(BlueRose);
	INIT(Torch);
	INIT(Turf);
}