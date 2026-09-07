#include "worldGenerator.h"
#include "randomStuff.h"
#include <iostream>
#include <math.h>
#include <FastNoiseSIMD.h>

void generateWorld(GameMap &gameMap, int seed)
{
	const int w = 900;
	const int h = 500;

	gameMap.create(w, h);

	int stoneSize = 380;
	int dirtSize = 50;

	std::ranlux24_base rng(seed++);

	int desertStart = getRandomInt(rng, 0, w - 210);
	int desertEnd = desertStart + 100 + getRandomInt(rng, 0, 100);
	if(desertEnd > w) {
		desertEnd = w;
	}

	std::unique_ptr<FastNoiseSIMD> dirtNoiseGenrator(FastNoiseSIMD::NewFastNoiseSIMD());
	dirtNoiseGenrator->SetSeed(seed++);

	dirtNoiseGenrator->SetFractalType(FastNoiseSIMD::FractalType::FBM);
	dirtNoiseGenrator->SetFractalOctaves(6);
	dirtNoiseGenrator->SetFractalGain(0.4f); // lower gain = sharper
	dirtNoiseGenrator->SetFrequency(0.01f);

	float *dirtNoise = FastNoiseSIMD::GetEmptySet(w);

	dirtNoiseGenrator->FillNoiseSet(dirtNoise, 0, 0, 0, w, 1, 1);

	for (int i = 0; i < w; i++) {
		dirtNoise[i] = (dirtNoise[i] + 1) / 2;
	}

	int dirtOffsetStart = -5;
	int dirtOffsetEnd = 35;

	int directionStone = getRandomInt(rng, -2, 2);
	int timeToKeepDirectionStone = getRandomInt(rng, 5, 40);

	int stoneHeight = 90;

	for (int x = 0; x < w; x++) {

		bool inDesert = (x >= desertStart && x <= desertEnd);

		int dirtHeight = dirtOffsetStart + (dirtOffsetEnd - dirtOffsetStart) * dirtNoise[x];
		dirtHeight = stoneHeight - dirtHeight;

		int dirtType = Block::dirt;
		int grassType = Block::grassBlock;
		int stoneType = Block::stone;

		if(inDesert) {
			dirtType = Block::sand;
			grassType = Block::sand;
			stoneType = Block::sandStone;
		}

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
				b.type = dirtType;
			}

			if (y == dirtHeight) {
				b.type = grassType;
			}

			if (y > stoneHeight) {
				if (getRandomChance(rng, 0.05)) {
					b.type = Block::gold;
				}
				else {
					b.type = stoneType;
				}
			}

			if (inDesert) {
				int desertMid = (desertStart + desertEnd) / 2;
				int deserthalfWidth = (desertEnd - desertStart) / 2;
				int distanceFromDesertMid = std::abs(x - desertMid);

				// This gives a value between 0 and 1, where 1 is at the center of the desert and 0 is at the edges
				float desertDistance = 1.0f - (float)distanceFromDesertMid / deserthalfWidth;

				int desertStoneStart = 10 + stoneHeight;
				int desertStoneDepth = 20 + stoneHeight; // how deep the desert triangle goes

				int triangleStoneY = desertStoneStart + desertDistance * desertStoneDepth;

				if(y > triangleStoneY) {
					b.type = Block::stone;
				}
			}

			gameMap.getBlockUnsafe(x, y) = b;
		}
	}

	FastNoiseSIMD::FreeNoiseSet(dirtNoise);
}
