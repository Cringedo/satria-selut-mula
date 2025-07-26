
#include <self/MonsterActionStrategy.hpp>
#include <self/Entity.hpp>

#include <raylib.h> 

void AggressiveActionStrategy::Execute(Entity &entity) {
    if(!IsApplicable(entity)) {
        return;
    }

    
    TraceLog(LOG_INFO, "Aggressive action executed for entity: %s", entity.GetName().c_str());
}

bool AggressiveActionStrategy::IsApplicable(const Entity &entity) const {
    return entity.GetHealth() > 0; 
}
