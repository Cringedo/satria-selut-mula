#include <self/GameObject.hpp>
// #include <self/Entity.hpp>
#include <self/Constant.h>
#include <self/Grid.hpp>

#include <iostream>
#include <string>
#include <raylib.h>
#include <raymath.h>

using namespace std;

Player::Player(float x, float y, string n, float s) : position({x, y}), Entity(n, s) 
{
    TraceLog(LOG_INFO, "Player (%s) has been created at [%0.0f, %0.0f]", n, position.x, position.y);
    Image image = LoadImage("resources/sprites/player_template.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);

    source.height = texture.height;
    source.width = texture.width;
    source.x = 0;
    source.y = 0;

    dest = {position.x, position.y, SCALE, SCALE};
    SetIsoCoordinate(dest);

    setRange(1);
    setHealth(5);
    setDamage(10);
    setSpeed(s); 
    // SetActionPoints(1);
}

void Player::SetGridPosition(float x, float y)
{
    gridCoordinate = {x, y};
}

void Player::SetPosition(float x, float y)
{
    position = {x, y};
    // SetGridCoordinate(position);

    // TraceLog(LOG_WARNING, "This position is at %0.0f, %0.0f", x, y);

    dest = {x, y, SCALE, SCALE};
}

void Player::SetPositionByIso(float x, float y)
{
    position = {x, y};

    Vector3 isoPosition = Vector3Transform({position.x * SCALE, position.y * SCALE, 1}, toIso);

    dest = {isoPosition.x, isoPosition.y, SCALE, SCALE};
    SetIsoCoordinate(dest);
}

Vector2 Player::GetPosition()
{
    return position;
}

Vector2 Player::GetGridCoordinate() const
{
    // cout << "Player GetGridCoordinate: " << gridCoordinate.x << ", " << gridCoordinate.y << endl;
    return gridCoordinate;
}

//  ---- Draw -----
void Player::Draw()
{
    // cout << "--------- Player: " << dest.x << dest.y << endl;
    // cout << "Player (" << name << ") is at " << source.x << ", " << source.y << endl;
    DrawTexturePro(texture, source, dest, {}, 0.0f, WHITE);
}