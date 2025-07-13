#pragma once

#include <self/GameObject.hpp>
#include <self/AttackStrategy.hpp>

#include <raylib.h>
#include <string>
#include <memory>

using namespace std;

class IAttackStrategy;

class Entity : public GameObject
{
public:
    Entity(string Name);
    string GetName() const;

    // ---- [Properties] ----

    float getDamage() const;
    void setDamage(float newDamage);

    float getHealth() const;
    void setHealth(float newHealth);

    float getRange() const;
    void setRange(float newRange);

    // ---- [Position and Grid Coordinate] ----

    virtual void SetPosition(float x, float y);
    virtual void SetPositionByIso(float x, float y);
    virtual void SetGridPosition(float x, float y);
    virtual void SetGridPosition(Vector2 coord);

    virtual Vector2 GetGridCoordinate() const;
    Vector2 GetPosition() const;

    // ---- [Interface] ----

    virtual void SetAttackStrategy(IAttackStrategy *strategy) { attackStrategy = unique_ptr<IAttackStrategy>(strategy); }

    // ---- [Drawing] ----

    virtual void Draw() = 0;
    virtual ~Entity() = default;

protected:
    string name;
    int id;
    float damage;
    float health;
    float range;

    Vector2 position;
    Vector2 gridCoordinate;
    Rectangle dest;
    Texture2D texture;
    Rectangle source;

    unique_ptr<IAttackStrategy> attackStrategy = nullptr;
};

class Player : public Entity
{
public:
    Player(float x, float y, std::string n);
    Vector2 GetGridCoordinate() const;
    Vector2 GetPosition();
    void SetGridPosition(float x, float y);
    void SetPosition(float x, float y);
    void SetPositionByIso(float x, float y);
    void Draw();

protected:
    Vector2 position;
    Vector2 gridCoordinate;
    Rectangle dest;
    string name;
    int health;
    Texture2D texture;
    Rectangle source;
};

// ------- [Monsters] -------
class Monster : public Entity
{
public:
    Monster(
        const std::string &idVal,
        const std::string &nameVal,
        int levelMinVal,
        int levelMaxVal,
        float baseHealthVal,
        float baseDamageVal,
        int goldDropVal,
        float spawnWeightVal);

    Monster(const Monster &other);

    // ---- [Properties] ----

    virtual int getLevelMin() const;
    virtual int getLevelMax() const;
    virtual float getBaseHealth() const;
    virtual float getBaseDamage() const;
    virtual int getGoldDrop() const;
    virtual float getSpawnWeight() const;

    float calculateHealthAtLevel(int level) const;

    // ---- [Position and Grid Coordinate] ----
    // Override to provide specific behavior for monsters

    Vector2 GetGridCoordinate() const;
    Vector2 GetPosition() const;
    void SetGridPosition(float x, float y);
    void SetPosition(float x, float y);
    void SetPositionByIso(float x, float y);

    void Draw();

private:
    std::string id;
    int levelMin;
    int levelMax;
    float baseHealth;
    float baseDamage;
    int goldDrop;
    float spawnWeight;
};

// ------- [Green Slime] --------
class GreenSlime : public Monster
{
public:
    GreenSlime(const std::string &idVal,
               const std::string &nameVal,
               int levelMinVal,
               int levelMaxVal,
               float baseHealthVal,
               float baseDamageVal,
               int goldDropVal,
               float spawnWeightVal);

    void Draw() override;
};
