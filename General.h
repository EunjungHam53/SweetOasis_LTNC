
#ifndef GENERAL_H_
#define GENERAL_H_

#include <windows.h>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

static Mix_Chunk* g_sound_blaster;
static Mix_Chunk* g_sound_collect;
static Mix_Chunk* g_frog_quak;
static Mix_Chunk* g_mechanical_sound;
static Mix_Chunk* g_break_robot;
static Mix_Chunk* g_wow_sound;
static Mix_Chunk* g_game_lose;
static Mix_Chunk* g_sound_pop;

// Screen
const int FRAME_PER_SECOND = 18; //fps
const int SCREEN_WIDTH = 1152; // == 18*TILE_SIZE
const int SCREEN_HEIGHT = 576; // == 9*TILE_SIZE
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 195;
const int COLOR_KEY_G = 195;
const int COLOR_KEY_B = 195;

const int RENDER_DRAW_COLOR = 0Xff;

#define NUM_OF_MENU_OPTIONS 2

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10

   //Bài 3: Kỹ thuật tile mapB
#define BLANK_TILE 0
#define TILE_SIZE 64

// Các ô đất
#define EARTH1_1 1
#define EARTH1_2 2
#define EARTH1_3 3
#define EARTH2_1 39
#define EARTH2_2 40
#define EARTH2_3 41
#define WOOD 49
#define BRICK 50
#define LEFT_BLOCK 22
#define RIGHT_BLOCK 24
#define UP_BLOCK 26
#define DOWN_BLOCK 26
#define SPECIAL_BLOCK 55

#define MAX_MAP_X 400
#define MAX_MAP_Y 10

typedef struct Input
{
    int left_;
    int right_;
    int up_;
    int down_;
    int jump_;
};

typedef struct Map
{
    int start_x_;
    int start_y_;

    int max_x_;
    int max_y_;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name_;
}; 

namespace General
{
    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
    bool CheckFocusWithRect(const int& x, const int& y, const SDL_Rect& rect);
}

#endif