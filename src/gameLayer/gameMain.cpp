#include <raylib.h>
#include "gameMain.h"
#include <iostream>
#include <fstream>
#include <asserts.h>
#include <assetManager.h>
#include <gameMap.h>
#include <helpers.h>
#include <raymath.h>

struct GameData {
	GameMap gameMap;
	Camera2D camera;
} gameData;

AssetManager assetManager;

bool initGame()
{
	assetManager.loadAll();

	//testMap(2);

	gameData.gameMap.create(700, 500);

	//for (int y = 0; y < 700; y++) {
	//	for (int x = 0; x < 500; x++) {
	//		gameData.gameMap.getBlockUnsafe(y, x).type = Block::stone;
	//	}
	//}

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

	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
	int blockX = (int)floor(worldPos.x);
	int blockY = (int)floor(worldPos.y);

	levelDesignInput(blockX, blockY);


#pragma endregion

	BeginMode2D(gameData.camera);

	Vector2 topLeftView = GetScreenToWorld2D({0,0}, gameData.camera);
	Vector2 bottomRightView = GetScreenToWorld2D({(float)GetScreenWidth(), (float)GetScreenHeight()}, gameData.camera);
	
	int startXView = (int)floorf(topLeftView.x-1);
	int endXView = (int)ceilf(bottomRightView.x+1);
	int startYView = (int)floorf(topLeftView.y-1);
	int endYView = (int)ceilf(bottomRightView.y+1);

	startXView = Clamp(startXView, 0, gameData.gameMap.w - 1);
	endXView = Clamp(endXView, 0, gameData.gameMap.w - 1);
	startYView = Clamp(startYView, 0, gameData.gameMap.h - 1);
	endYView = Clamp(endYView, 0, gameData.gameMap.h - 1);

	for (int y = startYView; y <= endYView; y++) {
		for (int x = startXView; x <= endXView; x++) {

			auto &b = gameData.gameMap.getBlockUnsafe(x, y);
			auto &wb = gameData.gameMap.getWallBlockUnsafe(x, y);

			if (wb.type != WallBlock::air) {
				DrawTexturePro(
					assetManager.wallBlockTextures,
					getTextureAtlas(wb.type, 0, 32, 32), // source
					{ (float)x, (float)y, 1, 1 }, // destination
					{ 0, 0 }, // origin (top-left corner)
					0.f, // rotation
					WHITE // tint
				);
			}


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

			if (gameData.gameMap.getBlockSafe(x, y)->type == Block::woodLog
				&& gameData.gameMap.getBlockSafe(x, y + 1)->type == Block::dirt) {
				DrawTexturePro(
					assetManager.treeTextures,
					getTextureAtlas(TreeBlock::trunkBottom, 0, 32, 32), // source
					{ (float)x, (float)y, 1, 1 }, // destination
					{ 0, 0 }, // origin (top-left corner)
					0.f, // rotation
					WHITE // tint
				);
			}
			else if (gameData.gameMap.getBlockSafe(x, y)->type == Block::woodLog
				&& gameData.gameMap.getBlockSafe(x + 1, y)->type == Block::leaves
				&& gameData.gameMap.getBlockSafe(x - 1, y)->type == Block::leaves
				&& gameData.gameMap.getBlockSafe(x, y + 1)->type == Block::woodLog
				&& gameData.gameMap.getBlockSafe(x, y - 1)->type == Block::leaves) {
				DrawTexturePro(
					assetManager.treeTextures,
					getTextureAtlas(TreeBlock::trunkFolliageAround, 0, 32, 32), // source
					{ (float)x, (float)y, 1, 1 }, // destination
					{ 0, 0 }, // origin (top-left corner)
					0.f, // rotation
					WHITE // tint
				);
			}
		}
	}

	// draw selected block (we draw it after the map so that it is displayed on top)
	DrawTexturePro(
		assetManager.frame,
		{ 0,0,(float)assetManager.frame.width,(float)assetManager.frame.height},
		{ (float)blockX, (float)blockY, 1, 1 },
		{0,0},
		0.0f,
		WHITE
	);

	EndMode2D();

	DrawFPS(10,10);

	return true;
}

void levelDesignInput(int blockX, int blockY)
{
	if (IsKeyDown(KEY_LEFT_SHIFT) && IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		auto wb = gameData.gameMap.getWallBlockSafe(blockX, blockY);
		if (wb) {
			*wb = {};
		}
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
		if (b) {
			*b = {};
		}
	}

	if (IsKeyDown(KEY_LEFT_SHIFT)) {
		auto wb = gameData.gameMap.getWallBlockSafe(blockX, blockY);
		if (wb) {
			if (IsKeyPressed(KEY_ONE)) {
				wb->type = WallBlock::dirtWall;
			}
			else if (IsKeyPressed(KEY_TWO)) {

				wb->type = WallBlock::stoneWall;
			}
			else if (IsKeyPressed(KEY_THREE)) {

				wb->type = WallBlock::blueRubyWall;
			}
			else if (IsKeyPressed(KEY_FOUR)) {

				wb->type = WallBlock::brickWall;
			}
		}
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
		if (b) {
			if (IsKeyPressed(KEY_ONE)) {

				b->type = Block::dirt;
			}
			else if (IsKeyPressed(KEY_TWO)) {

				b->type = Block::leaves;
			}
			else if (IsKeyPressed(KEY_THREE)) {

				b->type = Block::woodLog;
			}
			else if (IsKeyPressed(KEY_FOUR)) {

				b->type = Block::icePlatform;
			}
			else if (IsKeyPressed(KEY_FIVE)) {

				b->type = Block::platform;
			}
		}

	}
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";

	std::ofstream f(RESOURCES_PATH "f.txt");
	f << "\nCLOSED\n";
	f.close();
}

void testMap(int i)
{
	if (i == 1) {

		gameData.gameMap.create(30, 30);

		for (int y = 0; y < gameData.gameMap.h; y++) {
			for (int x = 0; x < gameData.gameMap.w; x++) {
				if (x % 4 == 0 && y % 4 == 0) {
					gameData.gameMap.getBlockUnsafe(x, y).type = Block::dirt;
				}
				else if (x % 4 == 0) {
					gameData.gameMap.getBlockUnsafe(x, y).type = Block::goldBlock;
				}
				else if (y % 4 == 0) {
					gameData.gameMap.getBlockUnsafe(x, y).type = Block::rubyBlock;
				}
				else {
					gameData.gameMap.getBlockUnsafe(x, y).type = Block::woodPlank;
				}
			}
		}
	}
	else if (i == 2) {
		gameData.gameMap.create(30, 30);

		for (int y = 0; y < gameData.gameMap.h; y++) {
			for (int x = 0; x < gameData.gameMap.w; x++) {

				float s = (std::sin(x) + 1.f) / 2.f;
				float s2 = (std::sin(x * 0.5) + 1.f) / 2.f;

				if (gameData.gameMap.h - (gameData.gameMap.h * 0.3 * s) - gameData.gameMap.h * 0.5 - (gameData.gameMap.h * 0.2 * s2) < y)
				{
					gameData.gameMap.getBlockUnsafe(x, y).type = Block::dirt;
				}
				else
				{
					gameData.gameMap.getBlockUnsafe(x, y).type = Block::air;
				}
			}
		}
	}
}
