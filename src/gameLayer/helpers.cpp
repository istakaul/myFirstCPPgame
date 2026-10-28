#include "helpers.h"

Rectangle getTextureAtlas(int x, int y, int cellSizePixelsX, int cellSizePixelsY)
{
	return Rectangle{ (float)x * cellSizePixelsX, (float)y * cellSizePixelsY, (float)cellSizePixelsX, (float)cellSizePixelsY };
}

Rectangle getRectangleForEntity(Transform2D transform, float textureW, float textureH)
{
	Transform2D rezult = transform;
	rezult.w = textureW;
	rezult.h = textureH;

	// move the sprite so that the bottom of the sprite matches the bottom of the collider
	rezult.pos.y -= (rezult.h - transform.h) / 2;

	return rezult.getAABB();
}
