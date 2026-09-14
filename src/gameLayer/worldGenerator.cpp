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
	std::unique_ptr<FastNoiseSIMD> cavesNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());

	dirtNoiseGenrator->SetSeed(seed++);
	cavesNoiseGenerator->SetSeed(seed++);

	dirtNoiseGenrator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	dirtNoiseGenrator->SetFractalOctaves(6);
	dirtNoiseGenrator->SetFractalGain(0.4f); // lower gain = sharper
	dirtNoiseGenrator->SetFrequency(0.01f);

	cavesNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	cavesNoiseGenerator->SetFractalOctaves(3);
	cavesNoiseGenerator->SetFrequency(0.02f);

	float *dirtNoise = FastNoiseSIMD::GetEmptySet(w);
	dirtNoiseGenrator->FillNoiseSet(dirtNoise, 0, 0, 0, w, 1, 1);

	for (int i = 0; i < w; i++) {
		dirtNoise[i] = (dirtNoise[i] + 1) / 2;
	}

	float *cavesNoise = FastNoiseSIMD::GetEmptySet(w * h);
	cavesNoiseGenerator->FillNoiseSet(cavesNoise, 0, 0, 0, h, w, 1); // make sure you flip h, w

	for (int i = 0; i < w * h; i++) {
		cavesNoise[i] = (cavesNoise[i] + 1) / 2;
	}

	auto getCaveNoise = [&](int x, int y)
		{
			return cavesNoise[x + y * w];
		};

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

			// bigger more interesting caves
			// getCaveNoise(x,y) < 0.80 && getCaveNoise(x,y)>0.60
			if (getCaveNoise(x, y) < 0.30f) { // this means that something like 30% of the world will be covered with caves
				b.type = Block::air;
			}

			gameMap.getBlockUnsafe(x, y) = b;
		}
	}

	FastNoiseSIMD::FreeNoiseSet(dirtNoise);
	FastNoiseSIMD::FreeNoiseSet(cavesNoise);
}
