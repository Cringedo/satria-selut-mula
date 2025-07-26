#pragma once


class Entity;

class MonsterActionStrategy {
public:
    virtual ~MonsterActionStrategy() = default;

    virtual void Execute(Entity &entity) = 0;

    virtual bool IsApplicable(const Entity &entity) const = 0;
};

class AggressiveActionStrategy : public MonsterActionStrategy {
public:
    void Execute(Entity &entity) override;
    bool IsApplicable(const Entity &entity) const override;
};