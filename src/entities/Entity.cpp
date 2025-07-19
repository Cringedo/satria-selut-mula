#include <raylib.h>
#include <iostream>

#include <self/Constant.h>
#include <self/Entity.hpp>
#include <self/GameObject.hpp>

Entity::Entity(string name, float speed) : name(name), speed(speed), GameObject({})
{
}

string Entity::GetName() const
{
    return name;
}

float Entity::GetDamage() const
{
    return damage;
}

void Entity::setDamage(float newDamage)
{
    damage = newDamage;
}

float Entity::GetHealth() const
{
    return health;
}

void Entity::setHealth(float newHealth)
{
    health = newHealth;

    if (health <= 0)
    {
        health = 0;
        TraceLog(LOG_INFO, "Entity %s has been defeated.", name.c_str());
    }
}

float Entity::getRange() const
{
    return range;
}

void Entity::setRange(float newRange)
{
    range = newRange;
}

float Entity::getSpeed() const
{
    return speed;
}

void Entity::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

void Entity::SetPosition(float x, float y)
{
    position = {x, y};
    // SetGridCoordinate(position);

    cout << "Test: " << x << ","  << y << endl;

    dest = {x, y, SCALE, SCALE};
}

void Entity::SetPositionByIso(float x, float y)
{
    // TODO: use a proper ISO from Constant.h instead
    position.x = x;
    position.y = y;
}

Vector2 Entity::GetGridCoordinate() const
{
    return gridCoordinate;
}

void Entity::SetGridPosition(float x, float y)
{
    gridCoordinate.x = x;
    gridCoordinate.y = y;
}

void Entity::SetGridPosition(Vector2 coord)
{
    gridCoordinate = coord;
}