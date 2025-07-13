#pragma once

#include <self/Entity.hpp>
#include <self/Grid.hpp>

#include <string>
#include <memory>
#include <map>

enum class GameState
{
    INTRO,
    MENU,
    PLAYING,
    PAUSED,
    GAMEOVER,
    LOADING,
};

class GameManager
{
public:
    static GameManager &GetInstance();

    GameManager(const GameManager &) = delete;
    GameManager &operator=(const GameManager &) = delete;

    void Init();
    void Update(float dt);
    void Draw();
    void Shutdown();

    GameState GetCurrentState() const { return currentState; }
    void ChangeState(GameState newState);
    bool LoadMonsterData(const string &filePath);

    Player *GetPlayer() { return playerPtr.get(); }
    Grid *GetGrid() { return gridPtr.get(); }

private:
    GameManager();
    ~GameManager();

    GameState currentState;

    // all monsters configured are loaded by LoadMonsterData() into this
    vector<unique_ptr<Monster>>monstersTemplate; 
    map<string, unique_ptr<Monster>> monsterTemplateMap;

    vector<unique_ptr<Monster>> monstersPtr;
    unique_ptr<Player> playerPtr;
    unique_ptr<Grid> gridPtr;

    void EnterState(GameState state);
    void ExitState(GameState state);
    
    void MovePlayer(int dx, int dy);
    void DrawEntities(const std::vector<Entity *> &entities);
    void DisplayDrawEntityOrder(const std::vector<Entity *> &drawableEntities);
};