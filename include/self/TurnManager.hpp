#pragma once

#include <self/Entity.hpp>
#include <vector>

// TODO: implement turn management logic
enum class TurnState
{
    PLAYER_TURN,
    MONSTER_TURN, 
    CALCULATE_TURN
};

class TurnManager
{
public:
    TurnManager();

    void Setup(std::vector<Entity *> initialEntities);
    void Setup();

    void StartTurn();
    void EndTurn();

    TurnState GetCurrentTurnState() const { return currentTurnState; }
    void SetCurrentTurnState(TurnState state) { currentTurnState = state; }

    void GetNextEntity();

    void SetCurrentEntity(Entity *entity) { currentEntity = entity; }
    Entity *GetCurrentEntity() const { return currentEntity; }

    void AddEntity(Entity *entity) { entities.push_back(entity); }
    void RemoveEntity(Entity *entity);
    const std::vector<Entity *> &GetEntities() const { return entities; }

    void DisplayTurnOrder() const;

private:
    TurnState currentTurnState;
    Entity *currentEntity;
    std::vector<Entity *> entities;
};