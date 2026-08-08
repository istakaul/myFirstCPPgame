#include <raylib.h>
#include "gameMain.h"
#include <iostream>
#include <fstream>
#include <asserts.h>
#include <assetManager.h>

struct GameData {

} gameData;

AssetManager assetManager;

bool initGame()
{
	assetManager.loadAll();

	return true;
}

bool updateGame()
{
/*
TODO: delete the following region
*/
#pragma region imgui Draw Example
	//DrawRectangle(75, 75, 100, 100, { 0,255,0,127 }); // green
	//DrawRectangle(50, 50, 100, 100, { 255,0,0,127 }); // red

	//DrawText("Congrats! You created your first window", 190, 200, 20, RED);
	//DrawText("Congrats! You created your first window", 190, 200, 20, {255, 0, 200, 255});
#pragma endregion

	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5) { deltaTime = 1.f / 5; } // limit deltaTime to a maximum of 1/5 seconds (200ms) to avoid large jumps

	DrawTexturePro(assetManager.dirt, { 0,0,(float)assetManager.dirt.width, (float)assetManager.dirt.height }, { 50,50, 100, 100 }, { 0,0 }, 0, WHITE);

	return true;
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";

	std::ofstream f(RESOURCES_PATH "f.txt");
	f << "\nCLOSED\n";
	f.close();
}
