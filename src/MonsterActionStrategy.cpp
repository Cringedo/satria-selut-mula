
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
    if (entity.GetHealth() <= 0) {
        TraceLog(LOG_WARNING, "Entity %s is not applicable for aggressive action (health <= 0).", entity.GetName().c_str());
        return false;
    }

    TraceLog(LOG_INFO, "Entity %s is applicable for aggressive action.", entity.GetName().c_str());

    

    return true;

}
