#include <self/Grid.hpp>
#include <self/Entity.hpp>
#include <self/Constant.h>
#include <self/FastNoiseLite.h>
#include <self/GameObject.hpp>

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// ======================
// GRID

vector<Vector2> safeTiles;

bool CheckForTile(Vector2 coord);
void DisplayTileDetails(const Tile& tile);
std::stringstream tileDetails;

Grid::Grid(int height, int width) : height(height), width(width)
{
    Image image = LoadImage("sprites/tile.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);

    source.x = 0;
    source.y = 0;
    source.width = (float)texture.width;
    source.height = (float)texture.height;

    TraceLog(LOG_INFO, TextFormat("Texture: %dx%d", texture.width, texture.height));
    TraceLog(LOG_INFO, TextFormat("Source: %0.0fx%0.0f", source.width, source.height));
}

std::pair<int, int> Grid::GetSize()
{
    return std::pair<int, int>(width, height);
}

void Grid::Generate()
{
    tiles.clear();
    safeTiles.clear();

    TraceLog(LOG_INFO, "Starting: Generate the grids");

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Value);
    noise.SetSeed(GetRandomValue(0, 100));
    noise.SetFrequency(0.5f);

    // This is assign the tile based on the coordinate
    Vector3 temp;
    Tile tile = Tile({}, {});
    
    vector<Tile> tempTiles;
    for (int i = 0; i < height; i++)
    {
        tempTiles.clear();
        for (int j = 0; j < width; j++)
        {
            temp = Vector3Transform({(float)i * SCALE, (float)j * SCALE, 1}, toIso);
            float noiseValue = noise.GetNoise((float)i, (float)j) * 10;

            tile = Tile({temp.x, temp.y, SCALE, SCALE}, noiseValue);
            tile.SetGridCoordinate({(float)i, (float)j});
            tempTiles.push_back(tile);

            // TODO: perhaps instead of putting this into a vector, we can use 2d vector
            if (tile.GetNoiseValue() < SAFE_TILE_NOISE)
            {
                safeTiles.push_back(tile.GetGridCoordinate());
            }
        }
        TraceLog(LOG_INFO, "Finished processing row: %d", i);
        TraceLog(LOG_INFO, "Tile Count: %zu", tempTiles.size());
        tiles.push_back(tempTiles);
    }

    // for (size_t rowIdx = 0; rowIdx < tiles.size(); ++rowIdx)
    // {
    //     for (size_t colIdx = 0; colIdx < tiles[rowIdx].size(); ++colIdx)
    //     {
    //         Tile& t = tiles[rowIdx][colIdx];
    //         TraceLog(LOG_INFO, TextFormat("Tile Index: [%zu, %zu] - Grid: [%0.0f, %0.0f] - Noise: %0.2f",
    //             rowIdx, colIdx, t.GetGridCoordinate().x, t.GetGridCoordinate().y, t.GetNoiseValue()));
    //     }
    //     TraceLog(LOG_INFO, "Finished processing row: %zu", rowIdx);
    // }
    // Handling object spawn
    for (Vector2 &v : safeTiles){

    }
}

void Grid::PlaceEntityByGridCoordinate(Entity &entity, int i, int j)
{
    // Tile *tile = GetTileByGridCoordinate(i, j);
    Tile *tile = &tiles[i][j];
    cout << entity.GetName() << endl;
    TraceLog(LOG_INFO, "Monster %s  is spawning at [%d, %d]", entity.GetName(), i, j);
    if (tile)
    {
        Rectangle rect = tile->GetRectangle();
        entity.SetPosition(rect.x, rect.y - rect.height / 2.0f);
        entity.SetGridPosition(i, j);
    }
}

void Grid::PlaceMonsterByGridCoordinate(Monster &monster, int i, int j)
{
    // Tile *tile = GetTileByGridCoordinate(i, j);
    Tile *tile = &tiles[i][j];
    cout << monster.GetName() << endl;
    TraceLog(LOG_INFO, "Monster %s  is spawning at [%d, %d]", monster.GetName(), i, j);
    if (tile)
    {
        Rectangle rect = tile->GetRectangle();
        monster.SetPosition(rect.x, rect.y - rect.height / 2.0f);
        monster.SetGridPosition(i, j);
    }
}

void Grid::PlacePlayerByGridCoordinate(Player &player, int i, int j)
{
    // Tile *tile = GetTileByGridCoordinate(i, j);
    Tile *tile = &tiles[i][j];
    
    cout << tile->GetGridCoordinate().x << ", " << tile->GetGridCoordinate().y << endl;

    if (tile)
    {
        Rectangle rect = tile->GetRectangle();
        player.SetPosition(rect.x, rect.y - rect.height / 2.0f);
        player.SetGridPosition(i, j);
    }
}

// TODO: probably can use std::map to make this faster
bool CheckForTile(Vector2 coord)
{
    for (Vector2 &c : safeTiles)
    {
        // TraceLog(LOG_INFO, "Checking for [%f, %f] - [%f, %f]", coord.x, coord.y, c.x, c.y);
        if (c.x == coord.x && c.y == coord.y)
        {
            return true;
        }
    }

    return false;
}

Vector2 Grid::GetRandomSafeTile()
{
    bool safeSpawn = false;
    int randomX = std::rand() % GRID_WIDTH;
    int randomY = std::rand() % GRID_HEIGHT;

    while (!safeSpawn)
    {
        // FIXME: This doesn't really check for a safe tile
        safeSpawn = CheckForTile({(float)randomX, (float)randomY});

        randomX = std::rand() % GRID_WIDTH;
        randomY = std::rand() % GRID_HEIGHT;
    }

    // TraceLog(LOG_INFO, "!!!Spawning at [%d, %d]", randomX, randomY);
    return {(float)randomX, (float)randomY};
}

Tile *Grid::GetTileByGridCoordinate(int i, int j)
{
    TraceLog(LOG_INFO, "Coordinate an Entity Position[%0.0f,%0.0f]", (float)i, (float)j);
    for (auto& row : tiles)
    {
        for (Tile &tile : row)
        {
            // TraceLog(LOG_INFO, "Coordinate [%0.0f,%0.0f]", tile.GetGridCoordinate().x, tile.GetGridCoordinate().y);
            if (tile.GetGridCoordinate().x == float(i) && tile.GetGridCoordinate().y == float(j))
            {
                // TraceLog(LOG_INFO, "Found the exact coordinate at Tile[%0.0f,%0.0f]-[%0.0f,%0.0f]", tile.GetGridCoordinate().x, tile.GetGridCoordinate().y, float(i), float(j));
                return &tile;
            }
        }
    }

    return nullptr;
}

vector<Vector2> GetTilesWithinRange()
{
    vector<Vector2> rangeTiles;

    rangeTiles.push_back({PLAYER_GRID_COORDINATE.x + PLAYER_RANGE, PLAYER_GRID_COORDINATE.y});
    rangeTiles.push_back({PLAYER_GRID_COORDINATE.x - PLAYER_RANGE, PLAYER_GRID_COORDINATE.y});
    rangeTiles.push_back({PLAYER_GRID_COORDINATE.x, PLAYER_GRID_COORDINATE.y + PLAYER_RANGE});
    rangeTiles.push_back({PLAYER_GRID_COORDINATE.x, PLAYER_GRID_COORDINATE.y - PLAYER_RANGE});

    return rangeTiles;
}

void Grid::Draw()
{
    bool tileDetailsDrawn = false;
    for (const auto& row : tiles)
    {
        for (const Tile& tile : row)
        {
            // TraceLog(LOG_INFO, TextFormat("Tile %f, %f - %f", tile.GetRectangle().x, tile.GetRectangle().y, tile.GetNoiseValue()));
            if (tile.GetNoiseValue() > SAFE_TILE_NOISE)
            {
                // DrawTexturePro(texture, source, tile.GetRectangle(), {}, 0.0f, RED);
                safeTiles.push_back({tile.GetRectangle().x, tile.GetRectangle().y});
                continue;
            }
            else
            {
                DrawTexturePro(texture, source, tile.GetRectangle(), {}, 0.0f, WHITE);
                for (Vector2 range : GetTilesWithinRange())
                {
                    if (tile.GetGridCoordinate().x == range.x && tile.GetGridCoordinate().y == range.y)
                    {
                        // TraceLog(LOG_INFO, "PLAYER IS AT [%f, %f]", PLAYER_GRID_COORDINATE.x, PLAYER_GRID_COORDINATE.y);
                        DrawTexturePro(texture, source, tile.GetRectangle(), {}, 0.0f, GREEN);
                    }
                }

                if (CheckCollisionPointRec(GetMousePosition(), tile.GetRectangle()) && !tileDetailsDrawn)
                {
                    DisplayTileDetails(tile);
                    tileDetailsDrawn = true; // Only show details for the first hovered tile
                }
            }
        }
    }
}

void DisplayTileDetails(const Tile& tile)
{
    std::ostringstream details;
    details << "Tile: [" << tile.GetGridCoordinate().x << ", " << tile.GetGridCoordinate().y << "]";
    float textSize = 32;
    float padding = 5;

    float textWidth = MeasureText(details.str().c_str(), textSize);

    Rectangle debugPanelSize = {padding, 33, textWidth, textSize};

    DrawText(details.str().c_str(), debugPanelSize.x + padding, debugPanelSize.y + padding, textSize, BLUE);
}

// ======================
// TILE Section

Tile::Tile(Rectangle rectangle, float noiseValue) : rectangle(rectangle), noiseValue(noiseValue), GameObject({})
{
    SetIsoCoordinate(rectangle);
    SetGridCoordinate(gridCoordinate);
    // TraceLog(LOG_INFO, "Comparison: %f,%f = %f,%f", rectangle.x, rectangle.y, GetIsoCoordinate().x, GetIsoCoordinate().y);
    // TraceLog(LOG_INFO, "Instantiate tile Rect: [%0.0f,%0.0f] Coordinate: [%0.0f,%0.0f]", rectangle.x, rectangle.y, gridCoordinate.x, gridCoordinate.y);
}

Rectangle Tile::GetRectangle() const
{
    return rectangle;
}

Vector2 Tile::GetGridCoordinate() const
{
    return gridCoordinate;
}

float Tile::GetNoiseValue() const
{
    return noiseValue;
}