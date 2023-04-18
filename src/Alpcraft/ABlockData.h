#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include <string>
#include <unordered_map>

enum Block : GLubyte {
	Air,
	Grass,
	Dirt,
	Stone,
	CottonStone,
	BlackRock,
	WoodLog,
	GlowStone,
	Water,
};

enum BasicSides {
	TopTexture,
	BottomTexture,
	SideTexture
};

constexpr glm::ivec2 ReturnBlockSide(GLubyte side, glm::ivec2 t1, glm::ivec2  t2, glm::ivec2  t3)
{ return side == 0 ? t1 : side == 1 ? t2 : side == 2 ? t3 : glm::ivec2(7, 7); }

constexpr glm::ivec2 ReturnBlockTexture(Block _Block, GLubyte side)
{
	switch (_Block)
	{
	case Grass:
		return ReturnBlockSide(side, { 0, 0 }, { 2, 0 }, { 1, 0 });
		break;
	case Dirt:
		return { 2, 0 };
		break;
	case Stone:
		return { 3, 0 };
		break;
	case CottonStone:
		return { 4, 0 };
		break;
	case BlackRock:
		return { 5,0 };
		break;
	case WoodLog:
		return ReturnBlockSide(side, { 7, 0 }, { 7, 0 }, { 6, 0 });
		break;
	case GlowStone:
		return { 0, 1 };
		break;
	case Water:
		return { 1, 1 };
		break;
	}

	return { 7, 7 }; // Missing Texture
}

constexpr static glm::ivec3 DirectionArray[6]
{
	{0, 0, 1},
	{0, 0, -1},
	{1, 0, 0},
	{-1, 0, 0},
	{0, 1, 0},
	{0, -1, 0}
};

constexpr glm::ivec3 Dir2Vec(int dir)
{
	return DirectionArray[dir];
}

constexpr bool IsInBound(glm::ivec3 block)
{
	if (block.x >= 0 && block.x < 32 &&
		block.y >= 0 && block.y < 32 &&
		block.z >= 0 && block.z < 32)
	{
		return true;
	}

	return false;
}