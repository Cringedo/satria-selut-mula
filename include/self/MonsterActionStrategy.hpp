#pragma once

#include <self/Constant.h>  
#include <raylib.h>
#include <string>
#include <iostream>

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

    std::string ToString() const {
        switch (type) {
            case MonsterActionType::ATTACK:
                return "Attack";
            case MonsterActionType::MOVE:
                return "Move";
            case MonsterActionType::DEFEND:
                return "Defend";
            case MonsterActionType::NEUTRAL:
                return "Neutral";
            default:
                return "Unknown Action";
        }
    }
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