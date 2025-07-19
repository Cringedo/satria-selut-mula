#include <self/TurnManager.hpp>

#include <algorithm>
#include <iostream>

TurnManager::TurnManager() {
    std::cout << "TurnManager initialized." << std::endl;
}

void TurnManager::StartTurn() {
    if (currentTurnState == TurnState::PLAYER_TURN) {
        TraceLog(LOG_INFO, "Starting player turn.");
    } else if (currentTurnState == TurnState::MONSTER_TURN) {
        TraceLog(LOG_INFO, "Starting monster turn.");
    } else {
        TraceLog(LOG_WARNING, "Unknown turn state. Cannot start turn.");
    }
}

void TurnManager::EndTurn() {
    if (currentTurnState == TurnState::PLAYER_TURN) {
        TraceLog(LOG_INFO, "Ending player turn.");
        currentTurnState = TurnState::MONSTER_TURN;
    } else if (currentTurnState == TurnState::MONSTER_TURN) {
        TraceLog(LOG_INFO, "Ending monster turn.");
        currentTurnState = TurnState::PLAYER_TURN;
    } else {
        TraceLog(LOG_WARNING, "Unknown turn state. Cannot end turn.");
    }
}

void TurnManager::Setup(std::vector<Entity *> initialEntities) {
    entities = std::move(initialEntities);

    TraceLog(LOG_INFO, "TurnManager setup with %d entities.", entities.size());

    std::sort(entities.begin(), entities.end(), [](Entity *a, Entity *b) {
        return a->getSpeed() > b->getSpeed(); 
    });

    if (dynamic_cast<Monster *>(entities[0])) {
        currentTurnState = TurnState::MONSTER_TURN;
    } else {
        currentTurnState = TurnState::PLAYER_TURN;
    }

    SetCurrentEntity(entities.empty() ? nullptr : entities[0]);
}

void TurnManager::GetNextEntity() {
    if (entities.empty()) {
        TraceLog(LOG_WARNING, "No entities available to get next entity.");
        return;
    }

    if (currentEntity) {
        auto it = std::find(entities.begin(), entities.end(), currentEntity);
        if (it != entities.end()) {
            ++it; 
            if (it == entities.end()) {
                it = entities.begin(); 
            }
            currentEntity = *it;
        } else {
            TraceLog(LOG_WARNING, "Current entity not found in the list.");
            currentEntity = entities.front(); 
        }
    } else {
        currentEntity = entities.front(); 
    }

    if(dynamic_cast<const Player *>(currentEntity)) {
        currentTurnState = TurnState::PLAYER_TURN;
    } else if (dynamic_cast<const Monster *>(currentEntity)) {
        currentTurnState = TurnState::MONSTER_TURN;
    } else {
        TraceLog(LOG_WARNING, "Current entity is neither Player nor Monster.");
    }

    TraceLog(LOG_INFO, "Next entity is: %s", currentEntity->GetName().c_str());
}

void TurnManager::DisplayTurnOrder() const {
    int yOffset = 10;
    int gap = 30;

    DrawText(TextFormat("Current Order: %s (%s)", currentTurnState == TurnState::PLAYER_TURN ? "Player" : "Monster", currentEntity ? currentEntity->GetName().c_str() : "None", currentEntity->GetName().c_str()), 10, yOffset, 20, BLACK);
    // DrawText(TextFormat("Entity: %s", currentEntity->GetName().c_str()), 10, yOffset+10, 20, BLACK);
    // DrawText(TextFormat("Entity: %s", dynamic_cast<Monster *>(currentEntity) ? currentEntity->GetName().c_str() : dynamic_cast<Player *>(currentEntity) ? currentEntity->GetName().c_str() : "None"), 10, yOffset+10, 20, BLACK);

    yOffset += 30;

    for (size_t i = 0; i < entities.size(); ++i) {
        Entity *entity = entities[i];
        if (!entity) continue;

        // FIXME: currently I don't know which speed are we using properly - is it the entity or children classes?
        DrawText(TextFormat("%zu: %s [Health: %f, Speed: %.2f]", i, entity->GetName().c_str(), entity->GetHealth(), entity->getSpeed()),
                 10, yOffset + static_cast<int>(i) * gap, 20, BLACK);
    }
}

