#pragma once
#include <raylib.h>
#include <unordered_map>

struct AssetManager {

	Texture2D dirt = {};
	Texture2D textures = {};
	Texture2D frame = {};
	Texture2D treeTextures = {};
	Texture2D wallBlockTextures = {};
	Texture2D player = {};
	Texture2D slime = {};
	Texture2D items = {};

	Texture2D forestBG = {};
	Texture2D desertBG = {};
	Texture2D snowBG = {};
	Texture2D caveBG = {};

	std::unordered_map <int, Texture2D> frontArmour;
	std::unordered_map <int, Texture2D> backArmour;
	std::unordered_map <int, Texture2D> feetArmour;
	std::unordered_map <int, Texture2D> headArmour;

	Texture2D getHeadTexture(int item);
	Texture2D getBackTexture(int item);
	Texture2D getFeetTexture(int item);
	Texture2D getFrontTexture(int item);

	void loadAll();
};