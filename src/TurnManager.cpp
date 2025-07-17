#include <self/TurnManager.hpp>

#include <algorithm>
#include <iostream>

TurnManager::TurnManager() {
    std::cout << "TurnManager initialized." << std::endl;
}

void TurnManager::Setup(std::vector<Entity *> initialEntities) {
    entities = std::move(initialEntities);

    TraceLog(LOG_INFO, "TurnManager setup with %d entities.", entities.size());

    std::sort(entities.begin(), entities.end(), [](Entity *a, Entity *b) {
       // TODO: compare between the speed

        return a->GetGridCoordinate().y < b->GetGridCoordinate().y ||
               (a->GetGridCoordinate().y == b->GetGridCoordinate().y && a->GetGridCoordinate().x < b->GetGridCoordinate().x);
    });
}

