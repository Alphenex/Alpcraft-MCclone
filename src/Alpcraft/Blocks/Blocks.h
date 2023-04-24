#pragma once

#include <GL/glew.h>
#include <GLM/glm.hpp>

struct BlockInfo;
enum Block : GLubyte;

extern BlockInfo BLOCKINFOS[256];
extern int BLOCKAMOUNT;

const char* GetBlockName(Block block);
bool GetBlockTransparency(Block block);
bool GetBlockIsLightSource(Block block);
GLubyte GetBlockLightEmitLevel(Block block);

struct BlockInfo {
	const char* BlockName = "NULL";
	Block BlockID = (Block)255;

	bool Transparency = false;
	bool LightSource = false;

	GLubyte LightEmitLevel = 0;

	void Register()
	{
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
}