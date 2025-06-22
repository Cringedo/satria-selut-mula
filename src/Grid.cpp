#include "self/Grid.hpp"
#include <self/Constant.h>

#include <raylib.h>
#include <raymath.h>
#include <vector>

Grid::Grid(int height, int width) : height(10), width(10)
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
    Tile tile = Tile({});
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp = Vector3Transform({(float)i * scale, (float)j * scale, 1}, toIso);
            tile = Tile({temp.x, temp.y, scale, scale});
            tiles.push_back(tile);
        }
    }
}

Tile::Tile(Rectangle rectangle) : rectangle(rectangle)
{
}

Rectangle Tile::GetRectangle() const
{
    return rectangle;
}

void Grid::Draw()
{
    for (const Tile& tile : tiles)
    {
        DrawTexturePro(texture, source, tile.GetRectangle(), {}, 0.0f, WHITE);
    }
}

std::pair<int, int> Tile::GetCoordinate()
{
    return std::pair<int, int>();
}