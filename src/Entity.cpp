#include <raylib.h>
#include <self/Entity.hpp>
#include <self/GameObject.hpp>

Entity::Entity(string name): name(name), GameObject({})
{
}