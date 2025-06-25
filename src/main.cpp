#include <raylib-cpp.hpp>
#include <self/Entity.hpp>
#include <self/Grid.hpp>
#include <self/Panel.hpp>

#include <self/Constant.h>

#include<raymath.h>

using namespace std;

void DisplayDebug(Player player);

int main()
{

    int screenWidth = windowSize[0];
    int screenHeight = windowSize[1];

    raylib::Color textColor(LIGHTGRAY);
    raylib::Window w(screenWidth, screenHeight, "Raylib C++ Starter Kit Example");

    SetTargetFPS(180);

    Grid grid = Grid(12, 12);
    grid.Generate();

    Player player = Player(1, 1, "Nabil");
    string updatedText = player.UpdateName();

    grid.PlacePlayerByGridCoordinate(player, 0, 0);

    Panel panel = Panel({0, 100, 200, 100});

    // Main game loop
    while (!w.ShouldClose()) // Detect window close button or ESC key
    {
        // ======================================
        // Update

        updatedText = "Hello " + player.UpdateName() + "!";
        if (IsKeyPressed(KEY_RIGHT))
        {
            grid.PlacePlayerByGridCoordinate(player, player.GetGridCoordinate().x + 1, player.GetGridCoordinate().y);
        }
        if (IsKeyPressed(KEY_LEFT))
        {
            grid.PlacePlayerByGridCoordinate(player, player.GetGridCoordinate().x - 1, player.GetGridCoordinate().y);
        }
        if (IsKeyPressed(KEY_UP))
        {
            grid.PlacePlayerByGridCoordinate(player, player.GetGridCoordinate().x, player.GetGridCoordinate().y + 1);
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            grid.PlacePlayerByGridCoordinate(player, player.GetGridCoordinate().x, player.GetGridCoordinate().y - 1);
        }
        if (IsKeyPressed(KEY_Z))
        {
            grid.Generate();
        }
        // ======================================
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        grid.Draw();
        player.Draw();

        panel.Draw();
        Panel(100, 200).Draw();

        DisplayDebug(player);

        EndDrawing();
    }

    return 0;
}

void DisplayDebug(Player player)
{
    const char* debugString;
    
    debugString = TextFormat("Player Position: (%.2f, %.2f)", player.GetGridCoordinate().x, player.GetGridCoordinate().y);

    DrawText(debugString, 1, 1, 32, BLACK);
}
