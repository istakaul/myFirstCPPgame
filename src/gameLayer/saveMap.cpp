#include "saveMap.h"
#include <asserts.h>

bool saveBlockDataToFile(std::vector<Block> blocks, int w, int h, const char *fileName)
{;
	std::ofstream f(fileName, std::ios::binary); // open a file in binary format

	if (!f.is_open()) { return false; } // if it can't be opened return false

	permaAssertDevelopement(blocks.size() == w * h); // check that the map is not empty and the data is correct (development mode)
	permaAssertDevelopement(blocks.size() != 0); // check that the map is not empty and the data is correct (development mode)
	if (blocks.size() != w * h) { return false; } // check that the map is not empty and the data is correct (production mode)
	if (blocks.size() == 0) { return false; } // check that the map is not empty and the data is correct (production mode)

	f.write((const char *)&w, sizeof(w)); // write the width
	f.write((const char *)&h, sizeof(h)); // write the height

	f.write((const char *)blocks.data(), sizeof(Block) * blocks.size()); // write the entire content of the vector , binary

	f.close();

	return true;
}

bool loadBlockDataFromFile(std::vector<Block> &blocks, int &w, int &h, const char *fileName)
{
	// Always clear first, so if you get any errors the data will be set to 0
	blocks.clear();
	w = 0;
	h = 0;

	std::ifstream f(fileName, std::ios::binary);
	
	if (!f.is_open()) { return false; }

	// Read dimensions
	f.read((char *)&w, sizeof(w));
	f.read((char *)&h, sizeof(h));

	if (!f || w <= 0 || h <= 0) {
		f.close();
		return false;
	}

	if (w > 10000) { f.close(); return false; }
	if (h > 10000) { f.close(); return false; }

	size_t blockCount = w * h;
	blocks.resize(blockCount);

	f.read((char *)blocks.data(), sizeof(Block) * blockCount);

	if (!f) {
		blocks.clear();
		w = 0;
		h = 0;
		f.close();
		return false;
	}

	for (int i = 0; i < blocks.size(); i++) {
		blocks[i].sanitize();
	}

	f.close();

	return true;
}
