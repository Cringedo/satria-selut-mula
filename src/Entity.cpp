#include <raylib.h>
#include <self/Entity.hpp>
#include <self/GameObject.hpp>

Entity::Entity(string name): name(name), GameObject({})
{
}

float Entity::getDamage() const
{
    return damage;
}

void Entity::setDamage(float newDamage)
{
    damage = newDamage;
}

float Entity::getHealth() const
{
    return health;
}

void Entity::setHealth(float newHealth)
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