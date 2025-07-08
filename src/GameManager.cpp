#include <iostream>
#include <fstream>
#include <raylib.h>

#include <self/GameManager.hpp>
#include <self/json.hpp>
#include <self/Entity.hpp>
#include <self/Grid.hpp>
#include <self/Constant.h>
#include <self/FastNoiseLite.h>

using json = nlohmann::json;
using namespace std;

GameManager &GameManager::GetInstance()
{
    static GameManager instance;
    return instance;
}

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

    // TODO: Initialise the load for the monster.json
    LoadMonsterData("data/monster.json");

    // Initialize your managed game objects/systems
    playerPtr = std::make_unique<Player>(0, 0, "PlayerOne");
    gridPtr = std::make_unique<Grid>(GRID_HEIGHT, GRID_WIDTH);
    gridPtr->Generate();

    // Place player at a random safe tile on the generated grid
    Vector2 playerSpawnCoordinate = gridPtr->GetRandomSafeTile();
    gridPtr->PlacePlayerByGridCoordinate(*playerPtr, playerSpawnCoordinate.x, playerSpawnCoordinate.y);
    PLAYER_GRID_COORDINATE = playerPtr->GetGridCoordinate();

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // ----- Mobs Spawn -------
    // TODO: do the proper spawning rate based on the player level
    Vector2 monsterSpawnCoordinate;
    monstersPtr.emplace_back(move(monstersTemplate[0]));
    monstersPtr.emplace_back(move(monstersTemplate[1]));
    for (unique_ptr<Monster> &monster : monstersPtr)
    {
        monsterSpawnCoordinate = gridPtr->GetRandomSafeTile();
        gridPtr->PlaceMonsterByGridCoordinate(*monster, monsterSpawnCoordinate.x, monsterSpawnCoordinate.y);
    }

    ChangeState(GameState::MENU);
}

void GameManager::Update(float dt)
{
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

            Vector2 monsterSpawnCoordinate;
            for (unique_ptr<Monster> &monster : monstersPtr)
            {
                monsterSpawnCoordinate = gridPtr->GetRandomSafeTile();
                gridPtr->PlaceMonsterByGridCoordinate(*monster, monsterSpawnCoordinate.x, monsterSpawnCoordinate.y);
            }
        }
        PLAYER_GRID_COORDINATE = playerPtr->GetGridCoordinate(); // Keep global in sync

        // Check for game over condition
        // if (playerPtr->IsDead()) ChangeState(GameState::GAMEOVER);
        // if (IsKeyPressed(KEY_P)) ChangeState(GameState::PAUSED); // Example pause
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

    std::vector<Entity *> drawableEntities;
    drawableEntities.push_back(playerPtr.get());

    for (const auto &monster : monstersPtr)
    {
        drawableEntities.push_back(monster.get());
    }

    // TODO:  sort the drawableEntities based on their grid coordinates
    std::sort(drawableEntities.begin(), drawableEntities.end(),
              [](const Entity *a, const Entity *b)
              {
                  return a->GetGridCoordinate().y < b->GetGridCoordinate().y ||
                         (a->GetGridCoordinate().y == b->GetGridCoordinate().y &&
                          a->GetGridCoordinate().x < b->GetGridCoordinate().x);
              });

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

        // TODO: Draw player and monsters based on their grid coordinates
        // ---------- Draw Player and Monsters ----------

        for (Entity *entity : drawableEntities)
        {
            // cout << "Drawing entity: " << entity->GetName() << endl;
            entity->Draw();
        }

        // for (unique_ptr<Monster> &monster : monstersPtr)
        // {
        //     // cout << "Drawing monster: " << monster->GetName() << endl;
        //     monster->Draw();
        // }
        break;

    case GameState::PAUSED:
        // Draw dimmed game state + pause overlay
        // gridPtr->Draw();
        // playerPtr->Draw();

        // for (unique_ptr<Monster> &monster : monstersPtr)
        // {
        //     monster->Draw();
        // }

        // DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
        // DrawText("PAUSED", GetScreenWidth() / 2 - MeasureText("PAUSED", 60) / 2, GetScreenHeight() / 2 - 30, 60, WHITE);
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
    TraceLog(LOG_INFO, "GameManager: Shutting down game systems...");
}

void GameManager::ChangeState(GameState newState)
{
    if (newState == currentState)
        return; // No change needed

    ExitState(currentState);
    currentState = newState;
    EnterState(currentState);

    TraceLog(LOG_INFO, TextFormat("Game State Changed: %d", (int)newState));
}

bool GameManager::LoadMonsterData(const string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        cerr << "Error: Could not open JSON file: " << filePath << std::endl;
        return false;
    }

    try
    {
        json data = json::parse(file);

        if (!data.is_array())
        {
            cerr << "Error: JSON root is not an array in " << filePath << std::endl;
            return false;
        }

        for (const auto &monsterJson : data)
        {
            string id = monsterJson.at("id").get<std::string>();
            string name = monsterJson.at("name").get<std::string>();
            int levelMin = monsterJson.at("levelMin").get<int>();
            int levelMax = monsterJson.at("levelMax").get<int>();
            float baseHealth = monsterJson.at("baseHealth").get<float>();
            float baseDamage = monsterJson.at("baseDamage").get<float>();
            int goldDrop = monsterJson.at("goldDrop").get<int>();
            float spawnWeight = monsterJson.at("spawnWeight").get<float>();

            // Create Monster object and add to vector
            monstersTemplate.emplace_back(
                make_unique<Monster>(
                    id, name, levelMin, levelMax,
                    baseHealth, baseDamage, goldDrop, spawnWeight));

            // monstersTemplate.emplace_back(
            //     id, name, levelMin, levelMax,
            //     baseHealth, baseDamage, goldDrop, spawnWeight
            // );

            // Store a pointer to the newly added template monster
            monsterTemplateMap[id] = &monstersTemplate.back();
        }
        std::cout << "Successfully loaded " << monstersTemplate.size() << " monster templates from " << filePath << std::endl;
        return true;
    }
    catch (const json::exception &e)
    {
        std::cerr << "JSON parse error in " << filePath << ": " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Standard library error while parsing " << filePath << ": " << e.what() << std::endl;
        return false;
    }
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