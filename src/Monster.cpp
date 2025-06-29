#include <self/Entity.hpp>
#include <algorithm>

Monster::Monster(
    const std::string &idVal,
    const std::string &nameVal,
    int levelMinVal,
    int levelMaxVal,
    float baseHealthVal,
    float baseDamageVal,
    int goldDropVal,
    float spawnWeightVal)
    : id(idVal),
      Entity(nameVal),
      levelMin(levelMinVal),
      levelMax(levelMaxVal),
      baseHealth(baseHealthVal),
      baseDamage(baseDamageVal),
      goldDrop(goldDropVal),
      spawnWeight(spawnWeightVal)
{
}

Monster::Monster(const Monster &other)
    : Entity(other), 
      levelMin(other.levelMin), levelMax(other.levelMax),
      baseHealth(other.baseHealth), baseDamage(other.baseDamage),
      goldDrop(other.goldDrop), spawnWeight(other.spawnWeight)
{
}

int Monster::getLevelMin() const
{
    return levelMin;
}

int Monster::getLevelMax() const
{
    return levelMax;
}

float Monster::getBaseHealth() const
{
    return baseHealth;
}

float Monster::getBaseDamage() const
{
    return baseDamage;
}

int Monster::getGoldDrop() const
{
    return goldDrop;
}

float Monster::getSpawnWeight() const
{
    return spawnWeight;
}

// TODO: Monster Health Calculation
float Monster::calculateHealthAtLevel(int level) const
{
    // Ensure the level is within the defined min and max levels for the monster
    int clampedLevel = std::max(levelMin, std::min(level, levelMax));

    // Simple example: Health scales linearly with level from base health
    // You can adjust this formula based on your game's balancing
    return baseHealth + (clampedLevel - levelMin) * 10.0f;
}

void Monster::Draw()
{
    DrawTexturePro(texture, source, dest, {}, 0.0f, RED);
}