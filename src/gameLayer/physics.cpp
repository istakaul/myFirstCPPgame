#include "physics.h"
#include "physics.h"
#include "physics.h"
#include "gameMap.h"
#include <raymath.h>

void PhysicalEntity::resolveConstraints(GameMap &mapData)
{
	upTouch = false;
	downTouch = false;
	leftTouch = false;
	rightTouch = false;

	Vector2 &pos = transform.pos;
	float distance = Vector2Distance(lastPosition, pos); // we calculate how much we move

	if (distance == 0) {
		// no movement happened
		return;
	}

	// if the player moves more than a predefined constant we just call checkCollisionOnce()
	float GRANULARITY = 0.8;

	if (distance <= GRANULARITY) {
		checkCollisionOnce(pos, mapData);
	}
	else {
		// We start from last position
		Vector2 newPos = lastPosition;

		/*-- and slowly move the player to the new position-- */
		Vector2 delta = pos - lastPosition;
		delta = Vector2Normalize(delta);
		delta *= GRANULARITY * 0.99;

		do {
			newPos += delta;
			Vector2 posTest = newPos;
		/*--------------------------------------------------------*/
			checkCollisionOnce(newPos, mapData);

				// here we check if checkCollisionOnce changed the position and hence a collision happened
				if (newPos != posTest) {
					pos = newPos;
					goto end;
				}
				} while (Vector2Length((newPos + delta) - pos) > GRANULARITY);
			
			// we check one last time
			checkCollisionOnce(pos, mapData);
		}

end:
	
	// don't get out of the world on x
	if (pos.x - transform.w / 2 < 0) { pos.x = transform.w / 2; }
	if (pos.x + transform.w / 2 > (mapData.w)) { pos.x = mapData.w - transform.w / 2; }
	if (pos.y + transform.h / 2 > mapData.h) { pos.y = mapData.h - transform.h / 2; }


	// If we do collide we need to stop the velocity
	if (leftTouch && velocity.x < 0) { velocity.x = 0; }
	if (rightTouch && velocity.x > 0) { velocity.x = 0; }

	if (upTouch && velocity.y < 0) { velocity.y = 0; }
	if (downTouch && velocity.y > 0) { velocity.y = 0; }
}

void PhysicalEntity::checkCollisionOnce(Vector2 &pos, GameMap &mapData)
{
	Vector2 delta = pos - lastPosition; // compute the direction the player is moving in
	Vector2 newPos = performCollisionOnOneAxis(mapData, { pos.x, lastPosition.y }, { delta.x, 0 }); // move the player only on the x-axis
	pos = performCollisionOnOneAxis(mapData, { newPos.x, pos.y }, { 0, delta.y }); // this is the new player position
}

Vector2 PhysicalEntity::performCollisionOnOneAxis(GameMap &mapData, Vector2 pos, Vector2 delta) {

	if (delta.x == 0 && delta.y == 0) { return pos; } // delta is how much the player moved

	/* -- calculate the box around the player that will define the blocks possibly to be touched -- */
	// we will compute a starting point and an ending point of this box.
	Vector2 dimensions = { transform.w, transform.h };

	// we do this add and subtract of 1 and the floor ceil to make sure that the box is always bigger than the player
	int minX = floor(pos.x - dimensions.x / 2.f - 1);
	int maxX = ceil((pos.x + dimensions.x / 2.f + 1));

	int minY = floor(pos.y - dimensions.y / 2.f - 1);
	int maxY = ceil((pos.y + dimensions.y / 2.f + 1));
	
	// Here we make sure that the box is inside the map, so that we don't check for blocks that are outside the map
	minX = std::max(0, minX);
	minY = std::max(0, minY);
	maxX = std::min(mapData.w, maxX);
	maxY = std::min(mapData.h, maxY);

	/*-------------------------------------------------------------------------------------------*/

	// Now we iterate through this box
	for (int y = minY; y < maxY; y++) {
		for (int x = minX; x < maxX; x++) {
			if (mapData.getBlockUnsafe(x, y).isCollidable()) { // we can use the unsafe method because we made sure the block is inside the map

				/* We create two transforms so that we can intersect them */
				Transform2D entity;
				entity.pos = pos; // we use the position supplied to the function and not the original player position
				entity.w = dimensions.x;
				entity.h = dimensions.y;

				Transform2D block;
				block.pos = { x + 0.5f, y + 0.5f };
				block.w = 1;
				block.h = 1;

				if (entity.intersectTransform(block, -0.00005f)) {
					if (delta.x != 0) {
						if (delta.x < 0) {  // moving left
							leftTouch = 1;
							pos.x = x + 1.f + dimensions.x / 2;
							return pos;
						}
						else {
							rightTouch = 1;
							pos.x = x - dimensions.x / 2;
							return pos;
						}
					}
					else if (delta.y != 0) {
						if (delta.y < 0) { // moving up
							upTouch = 1;
							pos.y = y + 1.f + dimensions.y / 2;
							return pos;
						}
						else {
							downTouch = 1;
							pos.y = y - dimensions.y / 2;
							return pos;
						}
					}
				}
			}
		}
	}

	return pos;
}