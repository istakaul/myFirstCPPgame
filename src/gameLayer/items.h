#pragma once

#include "blocks.h"
#include <raylib.h>

struct AssetManager;

/*-- Functions outside the struct --*/
bool isItem(int type);
bool isBlock(int type);

// add more helpers like these, to check if an item is a weapon, or a specific type of wood

/*----------------------------------*/

struct Item {

	enum
	{
		woodPickaxe = Block::BLOCKS_COUNT,
		woodAxe,
		woodHammer,
		stonePickaxe,
		stoneAxe,
		stoneHammer,
		copperPickaxe,
		copperAxe,
		copperHammer,
		ironPickaxe,
		ironAxe,
		ironHammer,
		goldPickaxe,
		goldAxe,
		goldHammer,
		slime,
		copperIngot,
		ironIngot,
		goldIngot,
		woodenSword,
		stoneSword,
		copperSword,
		ironSword,
		goldSword,
		ruby,
		copperSpear,
		ironSpear,
		goldSpear,
		blueRuby,
		iceSpear,
		healthPotion,
		apple,
		paper,
		book,
		bone,
		shuriken,
		copperDagger,
		ironDagger,
		iceDagger,
		copperChestPlate,
		copperHelmet,
		copperBoots,

		ironChestPlate,
		ironHelmet,
		ironBoots,

		goldChestPlate,
		goldHelmet,
		goldBoots,

		partyHat,
		sunGlasses,

		iceChestPlate,
		iceHelmet,
		iceBoots,

		LAST_ITEM

	};

	int type = 0;
	int count = 1;

	bool isItem() {
		return ::isItem(type); // the :: , tells C++ to use the function from outside the struct
	}

	bool isBlock() {
		return ::isBlock(type);
	}
};

Texture getTextureForItemType(int itemType, AssetManager &assetManager);

Rectangle getTextureCoordinatesForItemType(int itemType);
