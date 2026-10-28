#include "slime.h"
#include <assetManager.h>
#include "helpers.h"
#include <randomStuff.h>

void Slime::render(AssetManager &assetManager)
{
	auto aabb = getRectangleForEntity(physics.transform, 1, 1);

	DrawTexturePro(
		assetManager.slime,
		getTextureAtlas(0, 0, 32, 32),
		aabb, //dest
		{ 0,0 }, // origin (top-left corner)
		0.0f, // rotation
		WHITE // tint
	);
}

void Slime::update(float deltaTime)
{
}
 