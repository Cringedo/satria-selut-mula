#pragma once

#include <self/Entity.hpp>
#include <vector>

// TODO: implement turn management logic
enum class TurnState
{
    PLAYER_TURN,
    MONSTER_TURN
};

class TurnManager {
    public: 
        TurnManager();
        void StartTurn();
        void EndTurn();

        TurnState GetCurrentTurnState() const { return currentTurnState; }
        void SetCurrentEntity(Entity* entity) { currentEntity = entity; }
        Entity* GetCurrentEntity() const { return currentEntity; }
        void AddEntity(Entity* entity) { entities.push_back(entity); }
        void RemoveEntity(Entity* entity) {
            entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
        }

    private:
        TurnState currentTurnState;
        Entity* currentEntity; 
        std::vector<Entity*> entities;
};