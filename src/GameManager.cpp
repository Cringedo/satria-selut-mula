#include <self/GameManger.hpp>
#include <iostream>
#include <raylib.h>

#include <self/SpawnManager.hpp>

#include <self/Entity.hpp>
#include <self/Grid.hpp>
#include <self/Constant.h>
#include <self/FastNoiseLite.h>

// Global/Static instance definition
GameManager &GameManager::GetInstance()
{
    static GameManager instance;
    return instance;
}

// Private constructor
GameManager::GameManager() : currentState(GameState::INTRO)
{
    TraceLog(LOG_INFO, "GameManager: Initializing...");
    SetRandomSeed(GetTime());
}

GameManager::~GameManager()
{
    TraceLog(LOG_INFO, "GameManager: Shutting down...");
}

void GameManager::Init()
{
    TraceLog(LOG_INFO, "GameManager: Initializing game systems...");

    playerPtr = std::make_unique<Player>(0, 0, "PlayerOne");
    gridPtr = std::make_unique<Grid>(GRID_HEIGHT, GRID_WIDTH);
    gridPtr->Generate();

    Vector2 playerSpawnCoordinate = gridPtr->GetRandomSafeTile();
    gridPtr->PlacePlayerByGridCoordinate(*playerPtr, playerSpawnCoordinate.x, playerSpawnCoordinate.y);
    PLAYER_GRID_COORDINATE = playerPtr->GetGridCoordinate();

    // TODO: Spawn mobs and objects here
    std::vector<Monster> liveMonsters;
    
    std::vector<const Monster *> monstersToSpawnTemplates = SpawnManager::GetInstance().GetMonstersForLevel(PLAYER_LEVEL);

    for (const Monster *templateMonster : monstersToSpawnTemplates)
    {
        Vector2 monsterSpawnGridCoordinate = gridPtr->GetRandomSafeTile();

        // Use the copy constructor to create a *new, mutable* Monster instance from the template
        // This is where a new "live" monster object is created from the template's data.
        liveMonsters.emplace_back(*templateMonster); // Creates a copy

        // Get a mutable reference to the newly created monster for placement
        Monster &newLiveMonster = liveMonsters.back();

        // Place the *live* monster using its grid coordinates
        gridPtr->PlaceEntityByGridCoordinate(newLiveMonster, monsterSpawnGridCoordinate.x, monsterSpawnGridCoordinate.y);
    }

    ChangeState(GameState::MENU);
}

void GameManager::Update(float dt)
{
    // Main update logic based on current state
    switch (currentState)
    {
    case GameState::INTRO:
        // Update intro screen elements, check for input to proceed
        if (IsKeyPressed(KEY_ENTER))
        {
            ChangeState(GameState::MENU);
        }
        break;
    case GameState::MENU:
        // Update menu elements, check for menu selections
        if (IsKeyPressed(KEY_ENTER))
        {
            ChangeState(GameState::PLAYING);
        }
        break;
    case GameState::PLAYING:
        // Update all game entities and systems
        if (IsKeyPressed(KEY_RIGHT))
        {
            gridPtr->PlacePlayerByGridCoordinate(*playerPtr, playerPtr->GetGridCoordinate().x + 1, playerPtr->GetGridCoordinate().y);
        }
        if (IsKeyPressed(KEY_LEFT))
        {
            gridPtr->PlacePlayerByGridCoordinate(*playerPtr, playerPtr->GetGridCoordinate().x - 1, playerPtr->GetGridCoordinate().y);
        }
        if (IsKeyPressed(KEY_UP))
        {
            gridPtr->PlacePlayerByGridCoordinate(*playerPtr, playerPtr->GetGridCoordinate().x, playerPtr->GetGridCoordinate().y + 1);
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            gridPtr->PlacePlayerByGridCoordinate(*playerPtr, playerPtr->GetGridCoordinate().x, playerPtr->GetGridCoordinate().y - 1);
        }

        if (IsKeyPressed(KEY_Z))
        {
            gridPtr->Generate();
            Vector2 newSpawn = gridPtr->GetRandomSafeTile();
            gridPtr->PlacePlayerByGridCoordinate(*playerPtr, newSpawn.x, newSpawn.y);
        }
        PLAYER_GRID_COORDINATE = playerPtr->GetGridCoordinate(); // Keep global in sync
        break;

    case GameState::PAUSED:
        // Update pause menu logic, check for resume or quit
        if (IsKeyPressed(KEY_P))
        { // Unpause
            ChangeState(GameState::PLAYING);
        }
        break;
    case GameState::GAMEOVER:
        // Update game over screen logic, check for retry or quit
        if (IsKeyPressed(KEY_R))
        {
            // Reset game state and return to playing or menu
            // ChangeState(GameState::PLAYING); // Or back to menu
        }
        break;
    case GameState::LOADING:
        // Show loading screen, load resources asynchronously
        // Once loaded, ChangeState to next logical state
        break;
    }
}

void GameManager::Draw()
{
    // Drawing logic based on current state
    switch (currentState)
    {
    case GameState::INTRO:
        DrawText("INTRO SCREEN", GetScreenWidth() / 2 - MeasureText("INTRO SCREEN", 40) / 2, GetScreenHeight() / 2 - 20, 40, DARKBLUE);
        break;
    case GameState::MENU:
        DrawText("MAIN MENU", GetScreenWidth() / 2 - MeasureText("MAIN MENU", 40) / 2, GetScreenHeight() / 2 - 20, 40, DARKGREEN);
        DrawText("Press ENTER to Play", GetScreenWidth() / 2 - MeasureText("Press ENTER to Play", 20) / 2, GetScreenHeight() / 2 + 30, 20, GRAY);
        break;
    case GameState::PLAYING:
        gridPtr->Draw();
        playerPtr->Draw();
        // Call debug display here, passing playerPtr->get() if needed
        // DisplayDebug(*playerPtr); // Assuming DisplayDebug exists and is updated
        break;
    case GameState::PAUSED:
        // Draw dimmed game state + pause overlay
        gridPtr->Draw();
        playerPtr->Draw();
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
        DrawText("PAUSED", GetScreenWidth() / 2 - MeasureText("PAUSED", 60) / 2, GetScreenHeight() / 2 - 30, 60, WHITE);
        break;
    case GameState::GAMEOVER:
        DrawText("GAME OVER!", GetScreenWidth() / 2 - MeasureText("GAME OVER!", 60) / 2, GetScreenHeight() / 2 - 30, 60, RED);
        break;
    case GameState::LOADING:
        DrawText("LOADING...", GetScreenWidth() / 2 - MeasureText("LOADING...", 40) / 2, GetScreenHeight() / 2 - 20, 40, GRAY);
        break;
    }
}

void GameManager::Shutdown()
{
    // Perform cleanup before the game exits
    TraceLog(LOG_INFO, "GameManager: Shutting down game systems...");
}

void GameManager::ChangeState(GameState newState)
{
    if (newState == currentState)
        return; // No change needed

    ExitState(currentState);  // Perform exit actions for old state
    currentState = newState;  // Update state
    EnterState(currentState); // Perform entry actions for new state

    TraceLog(LOG_INFO, TextFormat("Game State Changed: %d", (int)newState));
}

void GameManager::EnterState(GameState state)
{
    switch (state)
    {
    case GameState::PLAYING:
        // Actions when entering PLAYING state (e.g., resume music, unpause physics)
        break;
    case GameState::MENU:
        // Actions when entering MENU state (e.g., play menu music, show cursor)
        break;
        // ... handle other states
    }
}

void GameManager::ExitState(GameState state)
{
    switch (state)
    {
    case GameState::PLAYING:
        // Actions when exiting PLAYING state (e.g., pause music, save game)
        break;
    case GameState::MENU:
        // Actions when exiting MENU state (e.g., hide menu elements)
        break;
        // ... handle other states
    }
}