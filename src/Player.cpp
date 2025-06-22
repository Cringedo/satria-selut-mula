#include <self/Entity.hpp>
#include <self/Constant.h>
#include <self/Grid.hpp>

#include <raylib.h>
#include <raymath.h>

using namespace std;

Player::Player(float x, float y, string n) : position({x, y}), name(n)
{
    TraceLog(LOG_INFO, "Player (%s) has been created at [%0.0f, %0.0f]", n, position.x, position.y);
    Image image = LoadImage("sprites/tile.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);

    source.height = texture.height;
    source.width = texture.width;
    source.x = 0;
    source.y = 0;

    dest = {position.x, position.y, scale, scale};
}

void Player::SetGridPosition(float x, float y)
{
    gridCoordinate = {x, y};
}

void Player::SetPosition(float x, float y)
{
    position = {x, y};

    TraceLog(LOG_WARNING, "This position is at %0.0f, %0.0f", x, y);

    dest = {x, y, scale, scale};
}

void Player::SetPositionByIso(float x, float y)
{
    position = {x, y};

    Vector3 isoPosition = Vector3Transform({position.x * scale, position.y * scale, 1}, toIso);

    dest = {isoPosition.x, isoPosition.y, scale, scale};
}

void Player::Draw()
{
    // DrawTexture(texture, positionY, positionY, RED);
    DrawTexturePro(texture, source, dest, {}, 0.0f, RED);
}

Vector2 Player::GetPosition()
{
    return position;
}

Vector2 Player::GetGridCoordinate()
{
    return gridCoordinate;
}

string Player::UpdateName()
{

    if (IsKeyDown(KEY_RIGHT))
        name = "abu";
    if (IsKeyDown(KEY_LEFT))
        name = "paria";

    return name;
}