#pragma once

#include<variant>
#include<array>
#include <raylib.h>

// tile size and scale
constexpr int SCALING{3};
constexpr int TILE_SIZE{32};
constexpr int SCALE = SCALING*TILE_SIZE;

// Grid size
constexpr int GRID_WIDTH{8};
constexpr int GRID_HEIGHT{8};

constexpr float SAFE_TILE_NOISE{4};

extern Vector2 PLAYER_GRID_COORDINATE;
extern float PLAYER_DAMAGE;
extern float PLAYER_RANGE;

extern int PLAYER_LEVEL;

// window variables
constexpr std::array<int,2> windowSize = {2 * SCALE*GRID_WIDTH, 2 * SCALE*GRID_HEIGHT/2 };

// enum for buttons and menu
enum sectionType {top=0, mid=1, bot=2};

// matrix to transform into isometric + centre grid 
constexpr Matrix toIso{
   0.5f , -0.5f, -SCALE/2.0f+windowSize[0]/2.0f, 0.0f,
   0.25f, 0.25f,          GRID_HEIGHT*SCALE/4.0f, 0.0f,
   0.0f ,  0.0f,                           1.0f, 0.0f,
   0.0f ,  0.0f,                           0.0f, 1.0f
};

// matrix to transform from centred isometric grid
constexpr Matrix toGrid{
   1.0f , 2.0f, -windowSize[0]/2.0f - GRID_HEIGHT*SCALE/2.0f, 0.0f,
   -1.0f, 2.0f, -GRID_HEIGHT*SCALE/2.0f + windowSize[0]/2.0f, 0.0f,
   0.0f , 0.0f,                                        1.0f, 0.0f,
   0.0f , 0.0f,                                        0.0f, 1.0f
};


