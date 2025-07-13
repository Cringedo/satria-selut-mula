#pragma once

#include <self/Entity.hpp>
#include <self/GameObject.hpp>

#include <vector>
#include <utility>
#include <raylib.h>

class Tile;

class Grid
{
public:
    Grid(int height, int width);
    std::pair<int, int> GetSize();
    Texture2D texture;
    Rectangle source;

    void Generate();
    void Draw();

    void PlaceEntityByGridCoordinate(Entity &e, int i, int j);
    void PlaceMonsterByGridCoordinate(Monster &m, int i, int j);
    void PlacePlayerByGridCoordinate(Player &p, int i, int j);
    Tile *GetTileByGridCoordinate(int i, int j);
    Vector2 GetRandomSafeTile();
    bool CheckForTile(Vector2 coord);

private:
    int height;
    int width;
    std::vector<std::vector<Tile>> tiles;
};

enum class TileType
{
    SAFE_TILE,
    DANGEROUS_TILE,
    MONSTER_TILE,
    ITEM_TILE,
    PLAYER_TILE,
    EMPTY_TILE 
};

class Tile : public GameObject
{
public:
    using GameObject::GameObject;
    Tile(Rectangle r, float noiseValue, TileType type);

    Rectangle GetRectangle() const;
    Vector2 GetGridCoordinate() const;
    float GetNoiseValue() const;
    void SetTileType(TileType type) { this->type = type; }
    TileType GetTileType() const { return type; }

private:
    Rectangle rectangle;
    float noiseValue;
    TileType type;
};