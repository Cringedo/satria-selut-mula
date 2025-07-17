#include <self/TurnManager.hpp>

#include <algorithm>
#include <iostream>

TurnManager::TurnManager() {
    std::cout << "TurnManager initialized." << std::endl;
}

void TurnManager::Setup(std::vector<Entity *> initialEntities) {
    entities = std::move(initialEntities);

    TraceLog(LOG_INFO, "TurnManager setup with %d entities.", entities.size());
}

