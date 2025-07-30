#pragma once

#include <self/Constant.h>  
#include <raylib.h>

class Entity;

enum class MonsterActionType {
    ATTACK,
    MOVE,
    DEFEND,
    NEUTRAL
};

struct MonsterAction {
    MonsterActionType type;
    Entity *target; 
    Vector2 position; 
};

class MonsterActionStrategy {
public:
    virtual ~MonsterActionStrategy() = default;

    virtual MonsterAction Execute(Entity &entity) = 0;

    virtual bool IsApplicable(const Entity &entity) const = 0;
};

class AggressiveActionStrategy : public MonsterActionStrategy {
public:
    MonsterAction Execute(Entity &entity) override;
    bool IsApplicable(const Entity &entity) const override;
};