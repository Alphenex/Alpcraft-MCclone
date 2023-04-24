#include "Blocks.h"

int BLOCKAMOUNT = 0;

BlockInfo BLOCKINFOS[256];

extern const char* GetBlockName(Block block)
{
	return BLOCKINFOS[block].BlockName;
}

extern bool GetBlockTransparency(Block block)
{
	return BLOCKINFOS[block].Transparency;
}

extern bool GetBlockIsLightSource(Block block)
{
	return BLOCKINFOS[block].LightSource;
}

void INIT_Air()
{
	BlockInfo BlockType;
	BlockType.BlockName = "Air";
	BlockType.BlockID = (Block)0;
	BlockType.Transparency = true;
	BlockType.Register();
}

void INIT_Grass()
{
	BlockInfo BlockType;
	BlockType.BlockName = "Grass";
	BlockType.BlockID = (Block)1;
	BlockType.Register();
}

void INIT_Dirt()
{
	BlockInfo BlockType;
	BlockType.BlockName = "Dirt";
	BlockType.BlockID = (Block)2;
	BlockType.Register();
}

void INIT_Stone()
{
	BlockInfo BlockType;
	BlockType.BlockName = "Stone";
	BlockType.BlockID = (Block)3;
	BlockType.Register();
}

void INIT_Cottonstone()
{
	BlockInfo BlockType;
	BlockType.BlockName = "Cottonstone";
	BlockType.BlockID = (Block)4;
	BlockType.Register();
}

void INIT_Bedrock()
{
	BlockInfo BlockType;
	BlockType.BlockName = "Bedrock";
	BlockType.BlockID = (Block)5;
	BlockType.Register();
}

void INIT_WoodLog()
{
	BlockInfo BlockType;
	BlockType.BlockName = "WoodLog";
	BlockType.BlockID = (Block)6;
	BlockType.Register();
}

void INIT_Glowstone()
{
	BlockInfo BlockType;
	BlockType.BlockName = "Glowstone";
	BlockType.BlockID = (Block)7;
	BlockType.LightSource = true;
	BlockType.Register();
}

void INIT_Water()
{
	BlockInfo BlockType;
	BlockType.BlockName = "Water";
	BlockType.BlockID = (Block)8;
	BlockType.Transparency = true;
	BlockType.Register();
}

void INIT_Glass()
{
	BlockInfo BlockType;
	BlockType.BlockName = "Glass";
	BlockType.BlockID = (Block)9;
	BlockType.Transparency = true;
	BlockType.Register();
}

void INIT_TintedGlass()
{
	BlockInfo BlockType;
	BlockType.BlockName = "Tinted Glass";
	BlockType.BlockID = (Block)10;
	BlockType.Transparency = true;
	BlockType.Register();
}

void INIT_CraftingDesk()
{
	BlockInfo BlockType;
	BlockType.BlockName = "Crafting Desk";
	BlockType.BlockID = (Block)11;
	BlockType.Register();
}

void INIT_Lava()
{
	BlockInfo BlockType;
	BlockType.BlockName = "Lava";
	BlockType.BlockID = (Block)12;
	BlockType.LightSource = true;
	BlockType.Register();
}