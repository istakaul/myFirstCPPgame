#include "player.h"
#include "assetManager.h"
#include <helpers.h>

void Player::render(AssetManager &assetManager) {
	
	auto aabb = physics.transform.getAABB();

	DrawTexturePro(
		assetManager.player,
		{ 0,0, (float)assetManager.player.width, (float)assetManager.player.height }, // source
		aabb, // dest
		{ 0,0 }, // origin (top-left corner)
		0.0f, // rotation
		WHITE // tint
	);
}

bool Player::update(float deltaTime, EntityUpdateData entityUpdateData) {
	return true;
}