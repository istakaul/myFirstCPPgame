#include <raylib.h>
#include "gameMain.h"
#include <iostream>
#include <fstream>
#include <asserts.h>
#include <helpers.h>
#include <assetManager.h>
#include <gameMap.h>
#include <structure.h>
#include <raymath.h>
#include "randomStuff.h"
#include <imgui.h>
#include <rlImGui.h>
#include <../ImGui_Theme/BlackDevil.h>
#include <worldGenerator.h>
#include <saveMap.h>
#include <physics.h>
#include <entities/slime.h>
#include <entityIdHolder.h>
#include <droppedItem.h>
#include <player.h>
#include <items.h>

struct GameData {
	GameMap gameMap;
	Camera2D camera = {}; // Initialize camera to zero/default values

	int creativeSelectedBlock = Block::air; // this represents the currently selected block

	// These are map points where we select an item on the map
	Vector2 selectionStart = {};
	Vector2 selectionEnd = {};
	Structure copyStructure;

	char saveName[100] = {}; 

	Player player;
	EntityHolder entities;

} gameData;

AssetManager assetManager;

static float CAMERA_SPEED = 50;

bool isShowImgui = false;

void spawnSlime(Vector2 position) {
	Slime slime;

	slime.physics.teleport(position);
	
	auto id = gameData.entities.idHolder.getEntityIdAndIncrement();

	gameData.entities.entities[id] = std::make_unique<Slime>(slime);
}

void spawnDroppedItem(Vector2 position, int type) {
	DroppedItem droppedItem;

	droppedItem.teleport(position);
	droppedItem.itemType = type;

	auto id = gameData.entities.idHolder.getEntityIdAndIncrement();

	gameData.entities.entities[id] = (std::make_unique<DroppedItem>(droppedItem));
}

bool initGame()
{
	assetManager.loadAll();

	generateWorld(gameData.gameMap, std::random_device{}());

	gameData.camera.target = { 20.5, 120.5 }; // world-space center of view
	gameData.camera.rotation = 0.f; // no rotation
	gameData.camera.zoom = 100.f; // no zoom

	// set the player position
	gameData.player.physics.teleport({ 20, 60 }); 
	
	// set the player size
	gameData.player.physics.transform.w = 0.9f; // slightly less than a block
	gameData.player.physics.transform.h = 1.8f; // almost twice the block

	
	spawnSlime({ 18, 60 });

	return true;
}

bool updateGame()
{
	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5) { deltaTime = 1.f / 5; } // limit deltaTime to a maximum of 1/5 seconds (200ms) to avoid large jumps

	gameData.camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }; // center of the screen

	ClearBackground({ 75, 75, 150, 255 }); // clear the screen with a dark green color

	if (IsKeyPressed(KEY_F10)) { isShowImgui = !isShowImgui; }

#pragma region camera movement
	if(IsKeyDown(KEY_LEFT)) gameData.player.getPosition().x -= 7.f * deltaTime;
	if(IsKeyDown(KEY_RIGHT)) gameData.player.getPosition().x += 7.f * deltaTime;
	if(IsKeyDown(KEY_UP)) gameData.player.getPosition().y -= 7.f * deltaTime;
	if(IsKeyDown(KEY_DOWN)) gameData.player.getPosition().y += 7.f * deltaTime;
	if (IsKeyDown(KEY_SPACE)) gameData.player.physics.jump(10);


#pragma region entities

	auto updateEntityPhysics = [&](Entity &entity, bool applyGravity = true) {

		if (applyGravity) {
			entity.physics.applyGravity();
			entity.physics.updateForces(deltaTime);
			entity.physics.resolveConstraints(gameData.gameMap);
			entity.physics.updateFinal();
		}
	};

	updateEntityPhysics(gameData.player, false);

	gameData.camera.target = gameData.player.getPosition();

	// slime
	std::ranlux24_base rng(std::random_device{}());

	// update all entities
	for (auto it = gameData.entities.entities.begin(); it != gameData.entities.entities.end();) { // we are not going to increment at the end, because erase is going to return the next valid iterator 

		EntityUpdateData updateData{
			gameData.player.getPosition(),
			rng,
			gameData.entities,
			it->first
		};

		bool shouldKill = false;

		if (!it->second->update(deltaTime, updateData) || it->second->life <=0) {
			shouldKill = true;
		}

		if (shouldKill) {
			// erase returns the next valid iterator
			it = gameData.entities.entities.erase(it);
		}
		else {
			// physics
			updateEntityPhysics(*it->second);
/*			it->second->update(deltaTime, updateData);
			it->second->physics.applyGravity();
			it->second->physics.updateForces(deltaTime);
			it->second->physics.resolveConstraints(gameData.gameMap);
			it->second->physics.updateFinal();
*/
			++it;
		}

	}
#pragma endregion

	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
	int blockX = (int)floor(worldPos.x);
	int blockY = (int)floor(worldPos.y);

	// make sure we cannot select an inexistent block
	if (gameData.creativeSelectedBlock < 0) { gameData.creativeSelectedBlock = 0; }
	if (gameData.creativeSelectedBlock >= Block::BLOCKS_COUNT) { gameData.creativeSelectedBlock = Block::BLOCKS_COUNT - 1; }

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
					getTextureAtlas(wb.type, wb.variation, 32, 32), // source
					{ (float)x, (float)y, 1, 1 }, // destination
					{ 0, 0 }, // origin (top-left corner)
					0.f, // rotation
					WHITE // tint
				);
			}


			if (b.type != Block::air) {


				DrawTexturePro(
					assetManager.textures,
					getTextureAtlas(b.type, b.variation, 32, 32), // source
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
	if (gameData.creativeSelectedBlock > 0) {
		DrawTexturePro(
			assetManager.textures,
			getTextureAtlas(gameData.creativeSelectedBlock, 0, 32, 32),
			{ (float)blockX, (float)blockY, 1, 1 },
			{ 0,0 },
			0.0f,
			WHITE
		);
	}
	else {
		DrawTexturePro(
			assetManager.frame,
			{ 0,0,(float)assetManager.frame.width,(float)assetManager.frame.height},
			{ (float)blockX, (float)blockY, 1, 1 },
			{0,0},
			0.0f,
			WHITE
		);
	}

	if (isShowImgui) {

		Rectangle rect;
		rect.x = gameData.selectionStart.x;
		rect.y = gameData.selectionStart.y;

		rect.width = gameData.selectionEnd.x - gameData.selectionStart.x;
		rect.height = gameData.selectionEnd.y - gameData.selectionStart.y;

		rect.width++;
		rect.height++;

		DrawRectangleLinesEx(rect, 0.1,
			{20, 101, 250, 145});

	}
	
	for (auto &e : gameData.entities.entities) {

		e.second->render(assetManager);
	}

	DrawTexturePro(
		assetManager.player,
		{ 0,0, (float)assetManager.player.width, (float)assetManager.player.height },
		getRectangleForEntity(gameData.player.physics.transform, 1, 2), // dest
		{ 0,0 }, // origin (top-left corner)
		0.0f, // rotation
		WHITE // tint
	);

	DrawRectangleLinesEx(gameData.player.physics.transform.getAABB(), 0.1, { 20, 101, 250, 120 }); // the collider

	EndMode2D();

	showImgui(isShowImgui);

	DrawFPS(10,10);

	return true;
}

void levelDesignInput(int blockX, int blockY)
{
	// Selection of map objects
	if (isShowImgui) {
		if (IsKeyPressed(KEY_ONE)) { gameData.selectionStart = Vector2{ float(blockX), float(blockY) }; }
		if (IsKeyPressed(KEY_TWO)) { gameData.selectionEnd = Vector2{ float(blockX), float(blockY) }; }
		if (IsKeyPressed(KEY_THREE)) { 
			gameData.copyStructure.pasteIntoMap(gameData.gameMap, Vector2{ float(blockX), float(blockY) });
		}

		// Make sure start is smaller than end
		if (gameData.selectionStart.x > gameData.selectionEnd.x) {
			std::swap(gameData.selectionStart.x, gameData.selectionEnd.x);
		}
		if (gameData.selectionStart.y > gameData.selectionEnd.y) {
			std::swap(gameData.selectionStart.y, gameData.selectionEnd.y);
		}
	}

	if (!isShowImgui) {
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
			if (b) {
				if (b->type) {
					spawnDroppedItem({ (float)blockX + 0.5f, (float)blockY + 0.5f }, Item::healthPotion);
				}

				*b = {};
			}
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
			if (b) {
				b->type = gameData.creativeSelectedBlock;
			}
		}
	}
	else { // when assets inventory opened
		if (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			// Copy selected block on map
			auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
			if (b) {
				gameData.creativeSelectedBlock = b->type;
			}
		}
	}


}

void showImgui(bool isShowImgui)
{
	if (isShowImgui) {

		ImGui::Begin("Game Control");

		ImGui::SliderFloat("Camera zoom:", &gameData.camera.zoom, 1, 150);
		ImGui::SliderFloat("Camera speed:", &CAMERA_SPEED, 5, 30);

		if (ImGui::Button("Hurt a slime")) {
			for (auto &e : gameData.entities.entities) {
				if (e.second->getEntityType() == EntityType_Slime) {
					e.second->life -= 3;
					break;
				}
			}
		}

		if (ImGui::Button("Spawn Slime")) {
			spawnSlime({ 18,20 });
		}

		if (ImGui::Button("Copy")) {
			gameData.copyStructure.copyFromMap(gameData.gameMap,
				gameData.selectionStart, gameData.selectionEnd);
		}

		ImGui::InputText("File Name", gameData.saveName, sizeof(gameData.saveName));

		if (ImGui::Button("Save to File")) {
			std::string path = RESOURCES_PATH "structures/";
			path += gameData.saveName;
			path += ".bin";
			
			saveBlockDataToFile(gameData.copyStructure.mapData,
				gameData.copyStructure.w, gameData.copyStructure.h,
				path.c_str());
		}

		if (ImGui::Button("Load from File")) {
			std::string path = RESOURCES_PATH "structures/";
			path += gameData.saveName;
			path += ".bin";

			loadBlockDataFromFile(gameData.copyStructure.mapData,
				gameData.copyStructure.w, gameData.copyStructure.h,
				path.c_str());
		}

		ImGui::Separator;

		for (int i = 0; i < Block::BLOCKS_COUNT; i++) {
			auto atlas = getTextureAtlas(i, 0, 32, 32);
			atlas.x /= assetManager.textures.width;
			atlas.width /= assetManager.textures.width;
			atlas.y /= assetManager.textures.height;
			atlas.height /= assetManager.textures.height;

			ImGui::PushID(i);

			ImTextureID tex = (ImTextureID)(intptr_t)assetManager.textures.id;
			if (ImGui::ImageButton(
				tex, { 35, 35 }, { atlas.x, atlas.y },
				{ atlas.x + atlas.width, atlas.y + atlas.height }))
			{
				gameData.creativeSelectedBlock = i;
			}

			ImGui::PopID();

			if (i % 10 != 0) {
				ImGui::SameLine();
			}
		}

		ImGui::InputInt("Select Block", &gameData.creativeSelectedBlock);

		ImGui::End();
	}
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";

	std::ofstream f(RESOURCES_PATH "f.txt");
	f << "\nCLOSED\n";
	f.close();
}
