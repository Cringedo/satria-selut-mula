#include <raylib-cpp.hpp>
#include <self/Entity.hpp>
#include <self/Grid.hpp>
#include <self/Constant.h>

using namespace std;

int main() {
    
    int screenWidth = windowSize[0];
    int screenHeight = windowSize[1];

    raylib::Color textColor(LIGHTGRAY);
    raylib::Window w(screenWidth, screenHeight, "Raylib C++ Starter Kit Example");
    
    SetTargetFPS(180);

    Grid grid = Grid(10, 10);
    grid.Generate();

    Player player = Player(1, 1, "Nabil");
    string updatedText = player.UpdateName();

    grid.PlacePlayerByGridCoordinate(player, 0, 0);
    // Main game loop
    while (!w.ShouldClose()) // Detect window close button or ESC key
    {
        // ======================================
        // Update
        
        updatedText = "Hello " + player.UpdateName() + "!";
        if(IsKeyDown(KEY_RIGHT)){
            TraceLog(LOG_INFO, "alo");
            grid.PlacePlayerByGridCoordinate(player, player.GetGridCoordinate().x+1, player.GetGridCoordinate().y+1);
        }
        // ======================================
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        textColor.DrawText(updatedText, 1, 1, 20);
        grid.Draw();
        player.Draw();
        
        
        
        EndDrawing();
    }

    return 0;
}