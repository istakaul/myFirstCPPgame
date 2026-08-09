#include <raylib.h>
#include "gameMain.h"
#include <iostream>
#include <fstream>
#include <asserts.h>
#include <assetManager.h>
#include <gameMap.h>
#include <helpers.h>

struct GameData {
	GameMap gameMap;
	Camera2D camera;
} gameData;

AssetManager assetManager;

bool initGame()
{
	assetManager.loadAll();

	gameData.gameMap.create(30, 10);

	gameData.gameMap.getBlockUnsafe(0, 0).type = Block::dirt;
	gameData.gameMap.getBlockUnsafe(1, 1).type = Block::grass;
	gameData.gameMap.getBlockUnsafe(2, 2).type = Block::goldBlock;
	gameData.gameMap.getBlockUnsafe(3, 3).type = Block::glass;
	gameData.gameMap.getBlockUnsafe(4, 4).type = Block::platform;

	gameData.camera.target = { 0.f, 0.f }; // world-space center of view
	gameData.camera.rotation = 0.f; // no rotation
	gameData.camera.zoom = 100.f; // no zoom

	return true;
}

bool updateGame()
{
	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5) { deltaTime = 1.f / 5; } // limit deltaTime to a maximum of 1/5 seconds (200ms) to avoid large jumps

	gameData.camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }; // center of the screen

	ClearBackground({ 75, 75, 150, 255 }); // clear the screen with a dark green color

#pragma region camera movement

	if(IsKeyDown(KEY_LEFT)) gameData.camera.target.x -= 7.f * deltaTime;
	if(IsKeyDown(KEY_RIGHT)) gameData.camera.target.x += 7.f * deltaTime;
	if(IsKeyDown(KEY_UP)) gameData.camera.target.y -= 7.f * deltaTime;
	if(IsKeyDown(KEY_DOWN)) gameData.camera.target.y += 7.f * deltaTime;

#pragma endregion

	BeginMode2D(gameData.camera);
	
	for (int y = 0; y < gameData.gameMap.h; y++) {
		for (int x = 0; x < gameData.gameMap.w; x++) {

			auto &b = gameData.gameMap.getBlockUnsafe(x, y);

			if (b.type != Block::air) {

				DrawTexturePro(
					assetManager.textures,
					getTextureAtlas(b.type, 0, 32, 32), // source
					{ (float)x, (float)y, 1, 1}, // destination
					{ 0, 0 }, // origin (top-left corner)
					0.f, // rotation
					WHITE // tint
				);
			}
		}
	}

	EndMode2D();

	return true;
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";

	std::ofstream f(RESOURCES_PATH "f.txt");
	f << "\nCLOSED\n";
	f.close();
}
