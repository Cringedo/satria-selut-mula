#pragma once

#include <self/GameObject.hpp>

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
    void setHealth(float newHealth);

    float getRange() const;
    void setRange(float newRange);

    float getSpeed() const;
    void setSpeed(float newSpeed);

    // ---- [Position and Grid Coordinate] ----

    virtual void SetPosition(float x, float y);
    virtual void SetPositionByIso(float x, float y);
    virtual void SetGridPosition(float x, float y);
    virtual void SetGridPosition(Vector2 coord);

    virtual Vector2 GetGridCoordinate() const;
    Vector2 GetPosition() const;

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

    Vector2 position;
    Vector2 gridCoordinate;
    Rectangle dest;
    Texture2D texture;
    Rectangle source;
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
        float spawnWeightVal,
        float speedVal);

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
               float spawnWeightVal,
               float speedVal);

    void Draw() override;
};
