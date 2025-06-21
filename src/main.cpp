#include <raylib-cpp.hpp>
#include <Entity.hpp>

using namespace std;

int main() {
    
    int screenWidth = 800;
    int screenHeight = 450;

    raylib::Color textColor(LIGHTGRAY);
    raylib::Window w(screenWidth, screenHeight, "Raylib C++ Starter Kit Example");
    
    SetTargetFPS(180);

    Player player = Player(1, 1, "Nabil");
    string updatedText = player.UpdateName();

    // Main game loop
    while (!w.ShouldClose()) // Detect window close button or ESC key
    {
        // ======================================
        // Update

        updatedText = "Hello " + player.UpdateName() + "!";
        
        // ======================================
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        textColor.DrawText(updatedText, 1, 1, 20);
        EndDrawing();
    }

    return 0;
}