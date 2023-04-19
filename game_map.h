
#ifndef GAME_MAP_H_
#define GAME_MAP_H_

#include "General.h"
#include "ImgObjectBase.h"

#define MAX_TILES 61

class TileImage : public ImageObjectBase
{
public:
    TileImage() {}
    ~TileImage() {}
};

class GameMap //chức năng: fill toàn bộ ô vuông của tilemat vào vị trí ô vuông (lưu trong struct) của map
{
public:
    GameMap() {}
    ~GameMap() {}

    void LoadMapData(char* name);
    void LoadTiles(SDL_Renderer* screen);
    void DrawMapToScreen(SDL_Renderer* screen);
    Map getMap() const {return game_map_;}
    void SetMap(Map& map_data) {game_map_ = map_data;}
private:
    Map game_map_;
    TileImage tile_img[MAX_TILES];
};

#endif
