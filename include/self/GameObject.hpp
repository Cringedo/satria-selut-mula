#pragma once

#include <raylib.h>
#include <raymath.h>

class GameObject
{
public:
    GameObject(Rectangle rect);

    void SetIsoCoordinate(Rectangle rect);
    Rectangle GetIsoCoordinate();

    void SetGridCoordinate(Vector2 coord);
    Vector2 GetGridCoordinate() const;
    
    float GetObjectID();

protected:
    float objectID;
    Rectangle isoCoordinate;
    Vector2 gridCoordinate;
};