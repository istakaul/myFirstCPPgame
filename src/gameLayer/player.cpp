#include "player.h"
#include "assetManager.h"
#include <helpers.h>

void Player::render(AssetManager &assetManager) {
	
	auto aabb = physics.transform.getAABB();

	// OLD VERSION
	//DrawTexturePro(
	//	assetManager.player,
	//	{ 0,0, (float)assetManager.player.width, (float)assetManager.player.height }, // source
	//	aabb, // dest
	//	{ 0,0 }, // origin (top-left corner)
	//	0.0f, // rotation
	//	WHITE // tint
	//);

	// We need a texture atlas for the player
	auto textureUV = getTextureAtlas(animations.positionX, animations.positionY, 32, 64, animations.movingLeft);

	DrawTexturePro(
		assetManager.playerBack,
		textureUV,
		aabb, // dest
		{0,0}, // origin (top-left corner)
		0.0f, // rotation
		WHITE
	);

	DrawTexturePro(
		assetManager.playerFeet,
		textureUV,
		aabb, // dest
		{0,0}, // origin (top-left corner)
		0.0f, // rotation
		WHITE
	);

	DrawTexturePro(
		assetManager.playerHead,
		textureUV,
		aabb, // dest
		{0,0}, // origin (top-left corner)
		0.0f, // rotation
		WHITE
	);

	// held item will go here
	DrawTexturePro(
		assetManager.playerFront,
		textureUV,
		aabb, // dest
		{0,0}, // origin (top-left corner)
		0.0f, // rotation
		WHITE
	);
}

bool Player::update(float deltaTime, EntityUpdateData entityUpdateData) {
	return true;
}