#include "slime.h"
#include <assetManager.h>
#include "helpers.h"
#include <randomStuff.h>

void Slime::render(AssetManager &assetManager) // rendering must be free of update stuff (animation etc)
{
	auto aabb = getRectangleForEntity(physics.transform, 1, 1);

	DrawTexturePro(
		assetManager.slime,
		getTextureAtlas(animation.positionX, animation.positionY, 32, 32),
		aabb, //dest
		{ 0,0 }, // origin (top-left corner)
		0.0f, // rotation
		WHITE // tint
	);
}

void Slime::update(float deltaTime, EntityUpdateData entityUpdateData)
{
	changeStateTimer -= deltaTime;

	if (changeStateTimer < 0) {
		changeStateTimer = getRandomFloat(entityUpdateData.rng, 1, 7);

		float distanceToPlayer = Vector2Distance(entityUpdateData.playerPosition, getPosition());

		if (distanceToPlayer < 20) {
			if (getRandomChance(entityUpdateData.rng, 0.8)) {
				currentState = STATE_CHASING;
			}
			else {
				currentState = STATE_WANDERING;
			}
		}
		else {
			currentState = STATE_WANDERING;
		}
	}

	if (physics.downTouch) {
		moveSpeed = 0;
		animation.setAnimation(0); // staying
	}
	else {
		animation.setAnimation(1); // in air
	}

	jumpTimer -= deltaTime;

	switch (currentState) {
		case STATE_WANDERING: {
			if (jumpTimer < 0) {
				jumpTimer = getRandomFloat(entityUpdateData.rng, 3, 12);
				physics.jump(10);
				moveSpeed = getRandomFloat(entityUpdateData.rng, -7, 7);
			}
		}
		break;

		case STATE_CHASING: {
			if (jumpTimer < 0) {
				jumpTimer = getRandomFloat(entityUpdateData.rng, 2, 7);
				physics.jump(10);

				if (entityUpdateData.playerPosition.x > getPosition().x) {
					moveSpeed = getRandomFloat(entityUpdateData.rng, 1, 7);
				}
				else {
					moveSpeed = -getRandomFloat(entityUpdateData.rng, 1, 7);
				}
			}

		}
		break;
	}

	if (moveSpeed) {
		getPosition().x += deltaTime * moveSpeed;
	}

	animation.update(deltaTime, 0.08, 7); // 7 frames lasting 0.08 seconds each
}
 