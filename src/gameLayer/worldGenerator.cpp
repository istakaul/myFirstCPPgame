#include "worldGenerator.h"
#include "randomStuff.h"
#include <iostream>
#include <math.h>

void generateWorld(GameMap &gameMap, int seed)
{
	const int w = 900;
	const int h = 500;

	gameMap.create(w, h);

	int stoneSize = 380;
	int dirtSize = 50;

	std::ranlux24_base rng(seed);

	int directionDirt = getRandomInt(rng, -2, 2); // -2: down fast, -1: down, 0: straigh, 1: up, 2:up fast
	int timeToKeepDirectionDirt = getRandomInt(rng, 5, 40);

	int directionStone = getRandomInt(rng, -2, 2);
	int timeToKeepDirectionStone = getRandomInt(rng, 5, 40);

	int dirtHeight = 70;
	int stoneHeight = 90;

	for (int x = 0; x < w; x++) {

		/*Dirt Code------------------------*/
		timeToKeepDirectionDirt--;
		if (timeToKeepDirectionDirt <= 0) {
			timeToKeepDirectionDirt = getRandomInt(rng, 5, 40);
			directionDirt = getRandomInt(rng, -2, 2);
		}

		if (directionDirt == -1) {
			if (getRandomChance(rng, 0.25)) {
				dirtHeight--;
			}
		}
		else if (directionDirt == -2) {
			if (getRandomChance(rng, 0.25)) {
				dirtHeight--;
			}
			if (getRandomChance(rng, 0.25)) {
				dirtHeight--;
			}
		}
		else if (directionDirt == 1) {
			if (getRandomChance(rng, 0.25)) {
				dirtHeight++;
			}
		}
		else if (directionDirt == 2) {
			if (getRandomChance(rng, 0.25)) {
				dirtHeight++;
			}
			if (getRandomChance(rng, 0.25)) {
				dirtHeight++;
			}
		}

		if (dirtHeight < 50) {
			dirtHeight = 50;
		}

		if (dirtHeight > 90) {
			dirtHeight = 90;
		}
		/*--------- End of Dirt code -----------------------*/

		/*----------Stone Code------------------------------*/
		timeToKeepDirectionStone--;
		if (timeToKeepDirectionStone <= 0) {
			timeToKeepDirectionStone = getRandomInt(rng, 5, 40);
			directionStone = getRandomInt(rng, -2, 2);
		}

		if (directionStone == -1) {
			if (getRandomChance(rng, 0.25)) {
				stoneHeight--;
			}
		}
		else if (directionStone == -2) {
			if (getRandomChance(rng, 0.25)) {
				stoneHeight--;
			}
			if (getRandomChance(rng, 0.25)) {
				stoneHeight--;
			}
		}
		else if (directionStone == 1) {
			if (getRandomChance(rng, 0.25)) {
				stoneHeight++;
			}
		}
		else if (directionStone == 2) {
			if (getRandomChance(rng, 0.25)) {
				stoneHeight++;
			}
			if (getRandomChance(rng, 0.25)) {
				stoneHeight++;
			}
		}

		if (stoneHeight < 60) {
			stoneHeight = 60;
		}

		if (stoneHeight > 120) {
			stoneHeight = 120;
		}

		/*------------------------ End of stone code ------------------------*/


		for (int y = 0; y < h; y++) {

			Block b;

			if (y > dirtHeight) {
				b.type = Block::dirt;
			}

			if (y == dirtHeight) {
				b.type = Block::grassBlock;
			}

			if (y > stoneHeight) {
				if (getRandomChance(rng, 0.05)) {
					b.type = Block::gold;
				}
				else {
					b.type = Block::stone;
				}
			}

			gameMap.getBlockUnsafe(x, y) = b;
		}
	}
}
