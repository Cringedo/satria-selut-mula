#include <raylib.h>
#include <iostream>

#include <self/Constant.h>
#include <self/Entity.hpp>
#include <self/GameObject.hpp>

Entity::Entity(string name) : name(name), GameObject({})
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

void Entity::SetHealth(float newHealth)
{
    health = newHealth;
}

float Entity::getRange() const
{
    return range;
}

void Entity::setRange(float newRange)
{
    range = newRange;
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