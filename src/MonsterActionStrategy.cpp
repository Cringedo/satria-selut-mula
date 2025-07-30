
#include <self/MonsterActionStrategy.hpp>
#include <self/Entity.hpp>

#include <raylib.h> 


MonsterAction AggressiveActionStrategy::Execute(Entity &entity) {
    if(!IsApplicable(entity)) {
        return { MonsterActionType::NEUTRAL, entity.GetTarget(), entity.GetGridCoordinate() };
    }
    
    TraceLog(LOG_INFO, "Aggressive action executed for entity: %s", entity.GetName().c_str());
    
    Vector2 targetPosition = entity.GetTarget()->GetGridCoordinate();
    Vector2 entityPosition = entity.GetGridCoordinate();

    TraceLog(LOG_INFO, "Entity %s is at position: (%f, %f)", entity.GetName().c_str(), entityPosition.x, entityPosition.y);
    TraceLog(LOG_INFO, "Target %s is at position: (%f, %f)", entity.GetTarget()->GetName().c_str(), targetPosition.x, targetPosition.y);

    // TODO: Implement logic to determine the action based on the target's position
    // FIXME: If there is no tile, it will force move
    if(targetPosition.x > entityPosition.x) {
        return { MonsterActionType::MOVE, entity.GetTarget(), {entityPosition.x + 1, entityPosition.y} };
    } else if(targetPosition.x < entityPosition.x) {
        return { MonsterActionType::MOVE, entity.GetTarget(), {entityPosition.x - 1, entityPosition.y} };
    } else if(targetPosition.y > entityPosition.y) {
        return { MonsterActionType::MOVE, entity.GetTarget(), {entityPosition.x, entityPosition.y + 1} };
    } else if(targetPosition.y < entityPosition.y) {
        return { MonsterActionType::MOVE, entity.GetTarget(), {entityPosition.x, entityPosition.y - 1} };
    }

    // If already at the same position, do nothing or attack if adjacent
    return { MonsterActionType::NEUTRAL, entity.GetTarget(), entity.GetGridCoordinate() };
}

bool AggressiveActionStrategy::IsApplicable(const Entity &entity) const {
    if (entity.GetHealth() <= 0 || entity.getSpeed() <= 0 || entity.GetTarget() == nullptr) {
        TraceLog(LOG_WARNING, "Entity %s is not applicable for aggressive action.", entity.GetName().c_str());
        return false;
    }

    TraceLog(LOG_INFO, "Entity %s is applicable for aggressive action.", entity.GetName().c_str());



        

    return true;

}
