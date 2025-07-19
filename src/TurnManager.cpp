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

}

void TurnManager::DisplayTurnOrder() const {
    int yOffset = 10;
    int gap = 30;

    DrawText(TextFormat("Current Order: %s", currentTurnState == TurnState::PLAYER_TURN ? "Player" : "Monster"), 10, yOffset+5, 20, BLACK);

    yOffset += 30;

    for (size_t i = 0; i < entities.size(); ++i) {
        Entity *entity = entities[i];
        if (!entity) continue;

        // FIXME: currently I don't know which speed are we using properly - is it the entity or children classes?
        DrawText(TextFormat("%zu: %s [Health: %f, Speed: %.2f]", i, entity->GetName().c_str(), entity->GetHealth(), entity->getSpeed()),
                 10, yOffset + static_cast<int>(i) * gap, 20, BLACK);
    }
}

