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

void GameManager::Init(GameState initialState)
{
    TraceLog(LOG_INFO, "GameManager: Initializing game systems...");

    LoadMonsterData("resources/data/monster.json");


    InitializePlayerAndMonsters();

    // ----- Turn Manager Initialization -------
    turnManager.Setup(entities);

    ChangeState(initialState);
}

void GameManager::InitializePlayerAndMonsters()
{
    // ----- Player and Grid Initialization -------
    playerPtr = std::make_unique<Player>(0, 0, "PlayerOne", 5.0f);
    gridPtr = std::make_unique<Grid>(GRID_HEIGHT, GRID_WIDTH);
    gridPtr->Generate();

    Vector2 playerSpawnCoordinate = gridPtr->GetRandomSafeTile();
    gridPtr->PlacePlayerByGridCoordinate(*playerPtr, playerSpawnCoordinate.x, playerSpawnCoordinate.y);
    PLAYER_GRID_COORDINATE = playerPtr->GetGridCoordinate();

    entities.push_back(playerPtr.get());
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // ----- Mobs Spawn -------
    // TODO: do the proper spawning rate based on the player level
    Vector2 monsterSpawnCoordinate;

    // TODO: just spawn 2 monsters
    monstersPtr.emplace_back(move(monsterTemplateMap["green_slime"]));
    monstersPtr.emplace_back(move(monsterTemplateMap["dark_green_slime"]));
    for (unique_ptr<Monster> &monster : monstersPtr)
    {
        TraceLog(LOG_INFO, "[GameManager]: Spawning monster %s", monster->GetName().c_str());
        monsterSpawnCoordinate = gridPtr->GetRandomSafeTile();
        gridPtr->PlaceMonsterByGridCoordinate(*monster, monsterSpawnCoordinate.x, monsterSpawnCoordinate.y);

        entities.push_back(monster.get());
    }
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
        
        // TODO: we can have this to be based on the player movement speed
        
        // ---- [🌟Turn Management] -----------------
        switch (turnManager.GetCurrentTurnState())
        {
        case TurnState::CALCULATE_TURN:
            turnManager.StartTurn();
            turnManager.SetCurrentEntity(nullptr);

            break;

        case TurnState::MONSTER_TURN:
            // turnManager.GetNextEntity();
            if (turnManager.GetCurrentEntity())
            {
                Monster *currentMonsterPtr = dynamic_cast<Monster *>(turnManager.GetCurrentEntity());
                currentMonsterPtr->TakeAction();
                turnManager.GetNextEntity();
            }
            else
            {
                TraceLog(LOG_WARNING, "No current entity to take action.");
            }
            break;

        case TurnState::PLAYER_TURN:
            turnManager.SetCurrentEntity(playerPtr.get());
            // TODO: Remap this into a UI-based movement instead
            if (IsKeyPressed(KEY_RIGHT))
                MovePlayer(1, 0);
            if (IsKeyPressed(KEY_LEFT))
                MovePlayer(-1, 0);
            if (IsKeyPressed(KEY_UP))
                MovePlayer(0, 1);
            if (IsKeyPressed(KEY_DOWN))
                MovePlayer(0, -1);
            break;

        default:
            break;
        }

        if (IsKeyPressed(KEY_B))
        {
            turnManager.GetNextEntity();
        }

        if (IsKeyPressed(KEY_V))
        {
            ChangeState(GameState::MENU);
        }

        // Change the level layout
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

        if (IsKeyPressed(KEY_R))
        {
            Shutdown();
            WaitTime(0.5f);
            Init(GameState::PLAYING);
        }

        PLAYER_GRID_COORDINATE = playerPtr->GetGridCoordinate(); // Keep global in sync

        if (IsKeyPressed(KEY_P))
        ChangeState(GameState::PAUSED); // Example pause
        
        // TODO: Player death
        // if (playerPtr->IsDead()) ChangeState(GameState::GAMEOVER);
        
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

    std::sort(drawableEntities.begin(), drawableEntities.end(),
              [](Entity *a, Entity *b)
              {
                  return a->GetGridCoordinate().y < b->GetGridCoordinate().y;
              });

    DisplayDrawEntityOrder(drawableEntities);
    turnManager.DisplayTurnOrder();

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

        // Draw player and monsters based on their grid coordinates
        DrawEntities(drawableEntities);

        break;

    case GameState::PAUSED:
        gridPtr->Draw();
        DrawEntities(drawableEntities);

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
    playerPtr.reset();
    gridPtr.reset();
    monstersPtr.clear();
    entities.clear();
    monsterTemplateMap.clear();
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
            float speed = monsterJson.value("speed", 1.0f); // Default speed if not provided

            // Store a pointer to the newly added template monster
            if (id == "green_slime")
            {
                TraceLog(LOG_INFO, "GameManager: Loaded monster template: %s", id.c_str());
                monsterTemplateMap[id] = make_unique<GreenSlime>(
                    id, name, levelMin, levelMax,
                    baseHealth, baseDamage, goldDrop, spawnWeight, speed);
            }
            else if (id == "dark_green_slime")
            {
                TraceLog(LOG_INFO, "GameManager: Loaded monster template: %s", id.c_str());
                monsterTemplateMap[id] = make_unique<DarkGreenSlime>(
                    id, name, levelMin, levelMax,
                    baseHealth, baseDamage, goldDrop, spawnWeight, speed);
            }
            else
            {
                TraceLog(LOG_INFO, "GameManager: Loaded monster template: %s", id.c_str());
                // Create Monster object and add to vector
            }
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

void GameManager::AddEntity(std::unique_ptr<Entity> entity)
{
    if (entity) {
        entities.push_back(entity.get());
        // Remove nullptrs from entities
        entities.erase(std::remove(entities.begin(), entities.end(), nullptr), entities.end());
    }
}

void GameManager::DisplayDrawEntityOrder(const std::vector<Entity *> &drawableEntities)
{
    int yOffset = 200;
    int gap = 20;

    DrawText("Entity Draw Order:", 10, yOffset - 30, 20, BLACK);

    for (size_t i = 0; i < drawableEntities.size(); ++i)
    {
        const Entity *entity = drawableEntities[i];

        // FIXME: instead of using children class, we could perhaps have it to be wrapped into only the parent class, Entity for the properties
        // Check if the entity is a Player or Monster
        if (dynamic_cast<const Monster *>(entity))
        {
            const Monster *monster = static_cast<const Monster *>(entity);
            DrawText(TextFormat("%zu: %s [%0.f] | Speed: [%0.f]", i, monster->GetName().c_str(), monster->GetHealth(), monster->getSpeed()),
                     10, yOffset + static_cast<int>(i) * gap, 20, BLACK);

            continue;
        }

        DrawText(TextFormat("%zu: %s [%0.f]", i, entity->GetName().c_str(), entity->GetHealth(), entity->getSpeed()),
                 10, yOffset + static_cast<int>(i) * gap, 20, BLACK);
    }
}

void GameManager::DrawEntities(const std::vector<Entity *> &entities)
{
    for (Entity *entity : entities)
    {
        entity->Draw();
    }
}

void GameManager::MovePlayer(int dx, int dy)
{
    // TODO: check for collision with grid boundaries or obstacles
    Vector2 coord = playerPtr->GetGridCoordinate();
    if (coord.x + dx < 0 || coord.x + dx >= gridPtr->GetSize().first ||
        coord.y + dy < 0 || coord.y + dy >= gridPtr->GetSize().second)
    {
        return;
    }

    gridPtr->PlacePlayerByGridCoordinate(*playerPtr, coord.x + dx, coord.y + dy);
    turnManager.GetNextEntity();
}