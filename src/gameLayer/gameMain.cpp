#include <raylib.h>
#include "gameMain.h"
#include <iostream>
#include <fstream>
#include <asserts.h>

struct GameData {
	float positionX = 100;
	float positionY = 100;
} gameData;

bool initGame()
{
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

	if (IsKeyDown(KEY_A)) { gameData.positionX -= 200 * deltaTime; }
	if (IsKeyDown(KEY_D)) { gameData.positionX += 200 * deltaTime; }
	if (IsKeyDown(KEY_W)) { gameData.positionY -= 200 * deltaTime; }
	if (IsKeyDown(KEY_S)) { gameData.positionY += 200 * deltaTime; }

	DrawRectangle(gameData.positionX, gameData.positionY, 50, 50, {255, 0, 200, 255});

	return true;
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";

	std::ofstream f(RESOURCES_PATH "f.txt");
	f << "\nCLOSED\n";
	f.close();
}
