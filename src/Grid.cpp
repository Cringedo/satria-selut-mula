#include <self/Grid.hpp>
#include <self/Entity.hpp>
#include <self/Constant.h>
#include <self/FastNoiseLite.h>
#include <self/GameObject.hpp>

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
void DisplayTileDetails(Tile tile);
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
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp = Vector3Transform({(float)i * SCALE, (float)j * SCALE, 1}, toIso);
            float noiseValue = noise.GetNoise((float)i, (float)j) * 10;

            tile = Tile({temp.x, temp.y, SCALE, SCALE}, noiseValue);
            tile.SetGridCoordinate({(float)i, (float)j});
            tiles.push_back(tile);

            if (tile.GetNoiseValue() < SAFE_TILE_NOISE)
            {
                safeTiles.push_back(tile.GetGridCoordinate());
            }
        }
    }
}

void Grid::PlacePlayerByGridCoordinate(Player &player, int i, int j)
{
    Tile *tile = GetTileByGridCoordinate(i, j);
    if (tile)
    {
        Rectangle rect = tile->GetRectangle();
        player.SetPosition(rect.x, rect.y - rect.height / 2.0f);
        player.SetGridPosition(i, j);
    }
}

bool CheckForTile(Vector2 coord)
{
    for (Vector2 &c : safeTiles)
    {
        TraceLog(LOG_INFO, "Checking for [%f, %f] - [%f, %f]", coord.x, coord.y, c.x, c.y);
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
    int randomX = std::rand() % width;
    int randomY = std::rand() % height;

    while (!safeSpawn)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        safeSpawn = CheckForTile({(float)randomX, (float)randomY});

        randomX = std::rand() % width;
        randomY = std::rand() % height;
    }

    TraceLog(LOG_INFO, "!!!Spawning at [%d, %d]", randomX, randomY);
    return {(float)randomX, (float)randomY};
}

Tile *Grid::GetTileByGridCoordinate(int i, int j)
{
    TraceLog(LOG_INFO, "Coordinate New Player Position[%0.0f,%0.0f]", (float)i, (float)j);
    for (Tile &tile : tiles)
    {
        // TraceLog(LOG_INFO, "Coordinate [%0.0f,%0.0f]", tile.GetGridCoordinate().x, tile.GetGridCoordinate().y);
        if (tile.GetGridCoordinate().x == float(i) && tile.GetGridCoordinate().y == float(j))
        {
            TraceLog(LOG_INFO, "Found the exact coordinate at Tile[%0.0f,%0.0f]-[%0.0f,%0.0f]", tile.GetGridCoordinate().x, tile.GetGridCoordinate().y, float(i), float(j));
            return &tile;
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
    for (const Tile &tile : tiles)
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
            // TODO: HERE
            for (Vector2 range : GetTilesWithinRange())
            {
                if (tile.GetGridCoordinate().x == range.x && tile.GetGridCoordinate().y == range.y)
                {
                    // TraceLog(LOG_INFO, "PLAYER IS AT [%f, %f]", PLAYER_GRID_COORDINATE.x, PLAYER_GRID_COORDINATE.y);
                    DrawTexturePro(texture, source, tile.GetRectangle(), {}, 0.0f, GREEN);
                }
            }

            if (CheckCollisionPointRec(GetMousePosition(), tile.GetRectangle()))
            {
                DisplayTileDetails(tile);
            }
        }
    }
}

void DisplayTileDetails(Tile tile)
{
    // tileDetails << std::fixed << std::setprecision(2);
    tileDetails << "Tile: [" << tile.GetGridCoordinate().x << ", " << tile.GetGridCoordinate().y << "]";
    float textSize = 32;
    float padding = 5;

    float textWidth = MeasureText(tileDetails.str().c_str(), textSize);

    Rectangle debugPanelSize = {padding, 33, textWidth, textSize};

    DrawText(tileDetails.str().c_str(), debugPanelSize.x + padding, debugPanelSize.y + padding, textSize, BLUE);
    tileDetails.str("");
    tileDetails.clear();
}

// ======================
// TILE

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