#pragma once
#include <self/Entity.hpp>
#include <raylib.h>
#include <string>
#include <memory>

class Entity;

class IAttackStrategy {
public:
    virtual ~IAttackStrategy() = default;
    virtual void Execute(Entity& self, Entity& target) = 0;
};