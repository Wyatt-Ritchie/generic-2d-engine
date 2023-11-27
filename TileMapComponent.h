#pragma once
#include "SpriteComponent.h"
#include <string>
#include "Math.h"
#include <vector>

struct Tile
{
    bool mTraversable = true;
    int mTilesetId = -1;
    Vector2 mDimension = Vector2(32.0f, 32.0f);
    float mScale = 1.0f;
    float mFlip = 0.0f;
};

class TileMapComponent : public SpriteComponent
{
public:
    TileMapComponent(class Actor* actor, int DrawOrder = 100);
    
    // function to load and read CSV file
    void LoadData(std::string fileName);
    void LoadTileSet(std::string fileName);

    // override Draw
    void Draw(SDL_Renderer* renderer) override;

private:
    SDL_Texture* mTileSet;

    std::vector<std::vector<std::vector<Tile>>> mTileMaps;
};

