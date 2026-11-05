#pragma once
#include <raylib.h>

struct AssetManager;

struct DrawBackground {
	void draw(float deltaTime, AssetManager &assetManager, Camera2D camera, Vector2 mapSize);
};