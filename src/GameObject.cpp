#include <self/GameObject.hpp>
#include <self/Constant.h>

GameObject::GameObject(Rectangle rectangle)
{
    Vector3 isoCoord = Vector3Transform({rectangle.x * SCALE, rectangle.y * SCALE, 1}, toIso);
    isoCoordinate = {isoCoord.x, isoCoord.y, SCALE, SCALE};
}

void GameObject::SetIsoCoordinate(Rectangle rect)
{
    isoCoordinate = {rect.x, rect.y, SCALE, SCALE};
}

Rectangle GameObject::GetIsoCoordinate()
{
    return isoCoordinate;
}

void GameObject::SetGridCoordinate(Vector2 coord)
{
    gridCoordinate = coord;
}

Vector2 GameObject::GetGridCoordinate() const
{
    return gridCoordinate;
}