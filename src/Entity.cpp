#include <self/Entity.hpp>

Entity::Entity(std::string Name)
    : GameObject({}), // Call the base class (GameObject) constructor if needed
      name(Name),     // Initialize the 'name' member
      id(0),          // Initialize 'id' (or assign a unique one)
      position({0.0f, 0.0f}), // Initialize position
      gridCoordinate({0.0f, 0.0f}) // Initialize grid coordinate
{
    // Any other setup logic specific to Entity objects
    // TraceLog(LOG_INFO, "Entity '%s' created.", Name.c_str());
}

void Entity::SetPosition(float x, float y){
    position = {x, y};
}

// If you want SetGridPosition(float, float) overload:
void Entity::SetGridPosition(float x, float y) {
    gridCoordinate.x = x;
    gridCoordinate.y = y;
}

// And ensure you have the Vector2 overload too, if it's declared:
void Entity::SetGridPosition(Vector2 coord) {
    gridCoordinate = coord;
}

// Implementation for GetPosition()
Vector2 Entity::GetPosition() const {
    return position; // Assuming 'position' is a member of Entity
}