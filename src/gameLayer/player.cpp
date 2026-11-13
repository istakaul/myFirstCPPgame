#include "player.h"
#include "assetManager.h"
#include <helpers.h>
//#include <items.h>

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

	if (heldItem) {
		textureUV = getTextureAtlas(animations.positionX, animations.positionY + 3, 32, 64, animations.movingLeft);
	}

	DrawTexturePro(
		assetManager.getBackTexture(armourChest),
		textureUV,
		aabb, // dest
		{0,0}, // origin (top-left corner)
		0.0f, // rotation
		WHITE
	);

	DrawTexturePro(
		assetManager.getFeetTexture(armourLegs),
		textureUV,
		aabb, // dest
		{0,0}, // origin (top-left corner)
		0.0f, // rotation
		WHITE
	);

	DrawTexturePro(
		assetManager.getHeadTexture(armourHead),
		textureUV,
		aabb, // dest
		{0,0}, // origin (top-left corner)
		0.0f, // rotation
		WHITE
	);

	if (heldItem) {
		Texture texture = getTextureForItemType(heldItem, assetManager);
		Rectangle textureUVItem = getTextureCoordinatesForItemType(heldItem);

		auto pos = aabb;
		//if is block
		if (heldItem < Block::BLOCKS_COUNT)
		{
			pos.width = 0.4;
			pos.height = 0.4;

			if (animations.movingLeft)
			{
				pos.y += 0.5;
				pos.x -= 0.2;
			}
			else
			{
				pos.y += 0.5;
				pos.x += 0.6;
			}

		}
		else
		{
			pos.width = 1;
			pos.height = 1;

			if (animations.movingLeft)
			{
				pos.y += 0.1;
				pos.x -= 0.7;
				textureUVItem = flipTextureAtlasX(textureUVItem);
			}
			else
			{
				pos.y += 0.1;
				pos.x += 0.5;
			}

		}

		DrawTexturePro(
			texture,
			textureUVItem,
			pos,
			{0,0},
			0.0f,
			WHITE
		);
	}

	// held item will go here
	DrawTexturePro(
		assetManager.getFrontTexture(armourChest),
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