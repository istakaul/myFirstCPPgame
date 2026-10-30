#include "items.h"
#include <assetManager.h>
#include <helpers.h>

bool isItem(int type)
{
    return type >= Block::BLOCKS_COUNT;
}

bool isBlock(int type)
{
    return !isItem(type);
}

Texture getTextureForItemType(int itemType, AssetManager &assetManager)
{
    if (itemType < Block::BLOCKS_COUNT) {
        // is a block
        return assetManager.textures;
    }
    else {
        // is an item
        return assetManager.items;
    }
}

Rectangle getTextureCoordinatesForItemType(int itemType)
{
    if (itemType < Block::BLOCKS_COUNT) {
        // is a block
        return getTextureAtlas(itemType, 4, 32, 32);
    }
    else {
        // is an item
        return getTextureAtlas(itemType - Block::BLOCKS_COUNT, 0, 32, 32);
    }
}
