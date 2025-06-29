#include <raylib-cpp.hpp>
#include <self/GameManger.hpp>
#include <self/Constant.h>

// Your DisplayDebug will need to access GameManager's player, or be passed it
extern void DisplayDebug(const Player &player); // Declare, definition likely elsewhere

int main()
{
    raylib::Window w(windowSize[0], windowSize[1], "Satria Selut");
    SetTargetFPS(60);

    // Initialize the GameManager
    GameManager::GetInstance().Init();

    // Main game loop
    while (!w.ShouldClose())
    {
        float deltaTime = GetFrameTime();

        GameManager::GetInstance().Update(deltaTime);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        GameManager::GetInstance().Draw();

        // if (GameManager::GetInstance().GetCurrentState() == GameState::PLAYING) {
        //     DisplayDebug(*(GameManager::GetInstance().GetPlayer())); // Pass player from GameManager
        // }

        EndDrawing();
    }

    GameManager::GetInstance().Shutdown();
    CloseWindow();

    return 0;
}