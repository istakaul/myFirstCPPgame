#include "saveMap.h"
#include <asserts.h>

struct BlockSaveRepresentation1 {
	std::uint16_t type = 0;

	Block toBlock() {
		Block b;
		b.type = type;
		return b;
	}
};

const int VERSION = 1;

BlockSaveRepresentation1 toBlockRespresentation(Block b) {
	BlockSaveRepresentation1 rez;
	rez.type = b.type;
	return rez;
}

//struct TreeBlockSaveRepresentation1 {
//	std::uint16_t type = 0;
//
//	TreeBlock toTreeBlock() {
//		TreeBlock b;
//		b.type = type;
//		return b;
//	}
//};
//
//struct WallBlockRespresentation1 {
//	std::uint16_t type = 0;
//
//	WallBlock toWallBlock() {
//		WallBlock b;
//		b.type = type;
//		return b;
//	}
//};

bool saveBlockDataToFile(std::vector<Block> &blocks, int w, int h, const char *fileName)
{;
	std::ofstream f(fileName, std::ios::binary); // open a file in binary format

	if (!f.is_open()) { return false; } // if it can't be opened return false

	permaAssertDevelopement(blocks.size() == w * h); // check that the map is not empty and the data is correct (development mode)
	permaAssertDevelopement(blocks.size() != 0); // check that the map is not empty and the data is correct (development mode)
	if (blocks.size() != w * h) { return false; } // check that the map is not empty and the data is correct (production mode)
	if (blocks.size() == 0) { return false; } // check that the map is not empty and the data is correct (production mode)

	f.write((const char *)&VERSION, sizeof(VERSION)); 
	f.write((const char *)&w, sizeof(w)); // write the width
	f.write((const char *)&h, sizeof(h)); // write the height

	for (int i = 0; i < blocks.size(); i++) {
		auto b = toBlockRespresentation(blocks[i]);
		f.write((const char *)&b, sizeof(b));
	}

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

	int readVersion = 0;

	f.read((char *)&readVersion, sizeof(readVersion));
	// Read dimensions
	if (readVersion == 1) {
		f.read((char *)&w, sizeof(w));
		f.read((char *)&h, sizeof(h));
	} 
	//else if (readVersion == 2) {
	//	// todo 
	//}
	//else {
	//	//error
	//	return 0;
	//}

	if (!f || w <= 0 || h <= 0) {
		f.close();
		return false;
	}

	if (w > 10000) { f.close(); return false; }
	if (h > 10000) { f.close(); return false; }

	switch (readVersion) {
		case 1:
		{
			size_t blockCount = w * h;
			blocks.resize(blockCount);

			for (int i = 0; i < blockCount; i++) {
				BlockSaveRepresentation1 read;
				f.read((char *)&read, sizeof(read));

				if (!f) {
					blocks.clear();
					w = 0;
					h = 0;
					f.close();
					return false;
				}

				blocks[i] = read.toBlock();
			}

			break;
		}
		default:
		{
			// incorrect version
			w = 0;
			h = 0;
			f.close();
			return false;
		}
	}

	for (int i = 0; i < blocks.size(); i++) {
		blocks[i].sanitize();
	}

	f.close();

	return true;
}
