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
        return a->getSpeed() > b->getSpeed(); 
    });
}

void TurnManager::DisplayTurnOrder() const {
    int yOffset = 10;
    int gap = 30;

    DrawText("Turn Order:", 10, yOffset, 20, BLACK);

    yOffset += 30;

    for (size_t i = 0; i < entities.size(); ++i) {
        Entity *entity = entities[i];
        if (!entity) continue;

        // FIXME: currently I don't know which speed are we using properly - is it the entity or children classes?
        DrawText(TextFormat("%zu: %s [Health: %f, Speed: %.2f]", i, entity->GetName().c_str(), entity->GetHealth(), entity->getSpeed()),
                 10, yOffset + static_cast<int>(i) * gap, 20, BLACK);
    }
}

