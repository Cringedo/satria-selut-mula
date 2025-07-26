#pragma once

#include <self/GameObject.hpp>
#include <self/MonsterActionStrategy.hpp>

#include <raylib.h>
#include <string>
#include <memory>

using namespace std;

class Entity : public GameObject
{
public:
    Entity(string Name, float speed);
    string GetName() const;

    // ---- [Properties] ----

    float GetDamage() const;
    void setDamage(float newDamage);

    float GetHealth() const;
    virtual void setHealth(float newHealth);

    float getRange() const;
    void setRange(float newRange);

    float getSpeed() const;
    void setSpeed(float newSpeed);

    // int GetActionPoints() const { return actionPoints; }
    // void SetActionPoints(int points) { actionPoints = points; }

    // ---- [Position and Grid Coordinate] ----

    virtual void SetPosition(float x, float y);
    virtual void SetPositionByIso(float x, float y);
    virtual void SetGridPosition(float x, float y);
    virtual void SetGridPosition(Vector2 coord);

    virtual Vector2 GetGridCoordinate() const;
    Vector2 GetPosition() const;

    // ---- [Target] ----
    void SetTarget(Entity* target);
    Entity* GetTarget() const;

    // ---- [Interface] ----

    // ---- [Drawing] ----

    virtual void Draw() = 0;
    virtual ~Entity() = default;

protected:
    string name;
    int id;
    float damage;
    float health;
    float range;
    float speed;
    // int actionPoints;

    Vector2 position;
    Vector2 gridCoordinate;
    Rectangle dest;
    Texture2D texture;
    Rectangle source;

    Entity* Target = nullptr;
};

class Player : public Entity
{
public:
    Player(float x, float y, std::string n, float s);
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
    int health;
    Texture2D texture;
    Rectangle source;
};

// ------- [Monsters] -------
class Monster : public Entity, public AggressiveActionStrategy
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
        float spawnWeightVal,
        float speedVal);

    Monster(const Monster &other);

    // ---- [Properties] ----

    void setHealth(float newHealth) override;

    virtual int getLevelMin() const;
    virtual int getLevelMax() const;
    virtual float getBaseHealth() const;
    virtual float getBaseDamage() const;
    virtual int getGoldDrop() const;
    virtual float getSpawnWeight() const;
    virtual float getSpeed() const { return speed; }

    float calculateHealthAtLevel(int level) const;

    // ---- [Action Logic] ----
    // TODO: START HERE FOR THE LOGIC
    virtual void TakeAction() = 0;

    // ---- [Position and Grid Coordinate] ----
    // Override to provide specific behavior for monsters

    Vector2 GetGridCoordinate() const;
    Vector2 GetPosition() const;
    void SetGridPosition(float x, float y);
    void SetPosition(float x, float y);
    void SetPositionByIso(float x, float y);

    void Draw();

protected:
    std::string id;
    int levelMin;
    int levelMax;
    float baseHealth;
    float baseDamage;
    int goldDrop;
    float spawnWeight;
    bool isDead = false;
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
               float spawnWeightVal,
               float speedVal);

private:
    void TakeAction() override;
};

// ------- [Dark Green Slime] --------

class DarkGreenSlime : public Monster
{
public:
    DarkGreenSlime(const std::string &idVal,
                   const std::string &nameVal,
                   int levelMinVal,
                   int levelMaxVal,
                   float baseHealthVal,
                   float baseDamageVal,
                   int goldDropVal,
                   float spawnWeightVal,
                   float speedVal);

private:
    void TakeAction() override;
};
