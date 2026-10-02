#pragma once
#include <vector>
#include <blocks.h>
#include <gameMap.h>
#include <raylib.h>

struct Structure {

	int w = 0;
	int h = 0;

	std::vector<Block> mapData;
	std::vector<WallBlock> mapDataWalls;

	void create(int w, int h);

	Block &getBlockUnsafe(int x, int y);

	Block *getBlockSafe(int x, int y);

	WallBlock &getWallBlockUnsafe(int x, int y);

	WallBlock *getWallBlockSafe(int x, int y);

	void copyFromMap(GameMap &map, Vector2 start, Vector2 end);

	void pasteIntoMap(GameMap &map, Vector2 start);
};
