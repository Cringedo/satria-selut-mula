#include <Entity.hpp>
#include <raylib.h>

using namespace std;

Player::Player(float x, float y, string n): positionX(x), positionY(y), name(n) {
    TraceLog(LOG_INFO, "Player (%s) has been created at [%f, %f]", n, positionX, positionY);
}

string Player::UpdateName() {

    if (IsKeyDown(KEY_RIGHT)) name = "abu";
    if (IsKeyDown(KEY_LEFT)) name = "paria";

    TraceLog(LOG_INFO, "Player's name has been updated to %s", name);

    return name;
}