#include <self/Grid.hpp>
#include <self/Entity.hpp>
#include <self/Constant.h>

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "self/Entity.hpp"

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
    TraceLog(LOG_INFO, "Starting: Generate the grids");

    Vector3 temp;
    Tile tile = Tile({}, {});
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp = Vector3Transform({(float)i * scale, (float)j * scale, 1}, toIso);
            tile = Tile({temp.x, temp.y, scale, scale}, {(float)i, (float)j});
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
        player.SetPosition(rect.x, rect.y-rect.height/2.0f); 
        player.SetGridPosition(i, j);
    }
}

Tile *Grid::GetTileByGridCoordinate(int i, int j)
{
    TraceLog(LOG_INFO, "Coordinate New Player Position[%0.0f,%0.0f]",(float) i, (float) j);
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
        DrawTexturePro(texture, source, tile.GetRectangle(), {}, 0.0f, WHITE);
    }
}

// ======================
// TILE

Tile::Tile(Rectangle rectangle, Vector2 gridCoordinate) : rectangle(rectangle), gridCoordinate(gridCoordinate)
{
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