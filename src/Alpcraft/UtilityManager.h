#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include <string>
#include <unordered_map>

#include "../glObj/Shader.h"
#include "../glObj/Texture.h"

#include "ABlockData.h"

struct UtilityManager {
	UtilityManager() {
		ChunkShader = new Shader("shaders//default.vert", "shaders//default.frag");

		TextureAtlas = new Texture("textures/blockatlas.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
		TextureAtlas->TextureUniform(*ChunkShader, "tex0", 0);
	};

	~UtilityManager() {
		ChunkShader->Delete();
		TextureAtlas->Delete();

		delete ChunkShader;
		delete TextureAtlas;
	};

	void BindBlockShader() { ChunkShader->Activate(); }
	Shader& GetShader() { return *ChunkShader; }

	void BindTextureAtlas() { TextureAtlas->Bind(); }
	void UnBindTextureAtlas() { TextureAtlas->UnBind(); }

private:
	Texture* TextureAtlas;
	Shader* ChunkShader;
};

class FPS
{
public:
	/// @brief Constructor with initialization.
	///
	FPS() : mFrame(0), mFps(0) {}

	/// @brief Update the frame count.
	///


	/// @brief Get the current FPS count.
	/// @return FPS count.
	const unsigned int getFPS() const { return mFps; }

private:
	unsigned int mFrame;
	unsigned int mFps;
	sf::Clock mClock;

public:
	void update()
	{
		if (mClock.getElapsedTime().asSeconds() >= .25f)
		{
			mFps = mFrame;
			mFrame = 0;
			mClock.restart();
		}

		mFrame+=4;
	}
};