#include <self/Grid.hpp>
#include <self/Entity.hpp>
#include <self/Constant.h>
#include <self/FastNoiseLite.h>
// #include <self/GameObject.hpp>

#include <raylib.h>
#include <raymath.h>
#include <vector>

// ======================
// GRID

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
            temp = Vector3Transform({(float)i * scale, (float)j * scale, 1}, toIso);
            float noiseValue = noise.GetNoise((float)i, (float)j) * 10;

            tile = Tile({temp.x, temp.y, scale, scale}, noiseValue);
            tile.SetGridCoordinate({(float)i, (float)j});
            tiles.push_back(tile);
        }
    }
}

void Grid::PlacePlayerByGridCoordinate(Player &player, int i, int j)
{
    Tile *tile = GetTileByGridCoordinate(i, j);
    TraceLog(LOG_INFO, "OKIE!");
    if (tile)
    {
        Rectangle rect = tile->GetRectangle();
        player.SetPosition(rect.x, rect.y - rect.height / 2.0f);
        player.SetGridPosition(i, j);
    }
}

Tile *Grid::GetTileByGridCoordinate(int i, int j)
{
    TraceLog(LOG_INFO, "Coordinate New Player Position[%0.0f,%0.0f]", (float)i, (float)j);
    for (Tile &tile : tiles)
    {
        TraceLog(LOG_INFO, "Coordinate [%0.0f,%0.0f]", tile.GetGridCoordinate().x, tile.GetGridCoordinate().y);
        if (tile.GetGridCoordinate().x == float(i) && tile.GetGridCoordinate().y == float(j))
        {
            TraceLog(LOG_INFO, "Found the exact coordinate at Tile[%0.0f,%0.0f]-[%0.0f,%0.0f]", tile.GetGridCoordinate().x, tile.GetGridCoordinate().y, float(i), float(j));
            return &tile;
        }
    }

    return nullptr;
}
void Grid::Draw()
{
    for (const Tile &tile : tiles)
    {
        // TraceLog(LOG_INFO, TextFormat("Tile %f, %f - %f", tile.GetRectangle().x, tile.GetRectangle().y, tile.GetNoiseValue()));
        if (tile.GetNoiseValue() > 4)
        {
            // DrawTexturePro(texture, source, tile.GetRectangle(), {}, 0.0f, RED);
            continue;
        }
        else
        {
            DrawTexturePro(texture, source, tile.GetRectangle(), {}, 0.0f, WHITE);
            // TODO: expand on this!
            if (CheckCollisionPointRec(GetMousePosition(),tile.GetRectangle()))
            {
                DrawText("Mouse over tile!", 10, 70, 20, BLUE);
            }
        }
    }
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

Vector2 Tile::GetGridCoordinate()
{
    return gridCoordinate;
}

float Tile::GetNoiseValue() const
{
    return noiseValue;
}