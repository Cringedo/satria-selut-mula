#include <self/GameObject.hpp>
#include <self/Constant.h>

GameObject::GameObject(Rectangle rectangle)
{
    Vector3 isoCoord = Vector3Transform({rectangle.x * scale, rectangle.y * scale, 1}, toIso);
    isoCoordinate = {isoCoord.x, isoCoord.y, scale, scale};
}

void GameObject::SetIsoCoordinate(Rectangle rect)
{
    isoCoordinate = {rect.x, rect.y, scale, scale};
}

Rectangle GameObject::GetIsoCoordinate()
{
    return isoCoordinate;
}

void GameObject::SetGridCoordinate(Vector2 coord)
{
    gridCoordinate = coord;
}

Vector2 GameObject::GetGridCoordinate()
{
    return gridCoordinate;
}