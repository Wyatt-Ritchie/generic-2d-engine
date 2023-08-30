#pragma once
#include "SpriteComponent.h"
#include <string>
#include <vector>
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

    std::vector<std::vector<std::vector<int>>> mTileMaps;
};

