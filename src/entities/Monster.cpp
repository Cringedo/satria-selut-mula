#include <self/Entity.hpp>
#include <self/Constant.h>
#include <self/MonsterActionStrategy.hpp>
#include <self/Grid.hpp>

#include <iostream>
#include <algorithm>

// --- Target Setter/Getter (now in Entity) ---


Monster::Monster(
    const std::string &idVal,
    const std::string &nameVal,
    int levelMinVal,
    int levelMaxVal,
    float baseHealthVal,
    float baseDamageVal,
    int goldDropVal,
    float spawnWeightVal,
    float speedVal)
    : id(idVal),
      Entity(nameVal, speedVal),
      levelMin(levelMinVal),
      levelMax(levelMaxVal),
      baseHealth(baseHealthVal),
      baseDamage(baseDamageVal),
      goldDrop(goldDropVal),
      spawnWeight(spawnWeightVal)
{
    // Initialize the texture and source rectangle for drawing
    source = {0, 0, 0, 0};       // Will be set later after loading the image
    dest = {0, 0, SCALE, SCALE}; // Default destination rectangle size

    // Load the monster texture from an image file
    {
        Image image = LoadImage("resources/sprites/player_template.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);

        source.x = 0;
        source.y = 0;
        source.width = (float)texture.width;
        source.height = (float)texture.height;

        cout << "Monster (" << nameVal << ") has been created with ID: " << idVal << endl;
        cout << texture.id << " - " << texture.width << "x" << texture.height << endl;

        setHealth(baseHealthVal);
        setSpeed(speedVal);
        // SetActionPoints(1);

        isDead = false; 
    }
}

Monster::Monster(const Monster &other)
    : Entity(other.name, other.speed),
      levelMin(other.levelMin), levelMax(other.levelMax),
      baseHealth(other.baseHealth), baseDamage(other.baseDamage),
      goldDrop(other.goldDrop), spawnWeight(other.spawnWeight)
{
    Image image = LoadImage("resources/sprites/player_template.png"); 
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
}

// ---- [Properties] ----

void Monster::setHealth(float newHealth)
{
    health = newHealth;

    if (health <= 0)
    {
        health = 0;
        TraceLog(LOG_INFO, "Monster %s has been defeated.", name.c_str());
        isDead = true; 
    }
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

// TODO: implement properly the monster scaling
float Monster::calculateHealthAtLevel(int level) const
{
    // Ensure the level is within the defined min and max levels for the monster
    int clampedLevel = std::max(levelMin, std::min(level, levelMax));

    // Simple example: Health scales linearly with level from base health
    // You can adjust this formula based on your game's balancing
    return baseHealth + (clampedLevel - levelMin) * 10.0f;
}

void Monster::TakeAction(Grid* grid)
{
    TraceLog(LOG_INFO, "Monster %s is taking action.", name.c_str());
    // Default implementation does nothing with grid
}


// ---- [Position and Grid Coordinate] ----

Vector2 Monster::GetPosition() const
{
    return position; // Return the 'position' member variable
}

Vector2 Monster::GetGridCoordinate() const
{
    return gridCoordinate;
}

void Monster::SetPosition(float x, float y)
{
    position = {x, y};
    // SetGridCoordinate(position);
    dest = {x, y, SCALE, SCALE};
}

void Monster::SetPositionByIso(float x, float y)
{
    // TODO: use a proper ISO from Constant.h instead
    position.x = x;
    position.y = y;
}

void Monster::SetGridPosition(float x, float y)
{
    gridCoordinate.x = x;
    gridCoordinate.y = y;
}

void Monster::Draw()
{
    // cout << " texture: " << texture.id << " - " << texture.width << "x" << texture.height << endl;
    // cout << "--------- Monster: " << dest.x << "," << dest.y << endl;
    // cout << Entity::GetName() << " is at " << source.x << ", " << source.y << endl;
    if(isDead)
    {
        // If the monster's health is zero or less, it should not be drawn
        return;
    }

    int fontSize = 20;
    DrawText(TextFormat("%s: %0.f", GetName().c_str(), GetHealth()), dest.x, dest.y - 10, fontSize, RED);
    DrawText(TextFormat("Coordinate: (%.2f, %.2f)", GetGridCoordinate().x, GetGridCoordinate().y), dest.x, dest.y + 10, fontSize, RED);
    DrawTexturePro(texture, source, dest, {}, 0.0f, RED);
}

// ======== [Monster Subclass ] ========

// -----[Green Slime]-------
GreenSlime::GreenSlime(
    const std::string &idVal,
    const std::string &nameVal,
    int levelMinVal,
    int levelMaxVal,
    float baseHealthVal,
    float baseDamageVal,
    int goldDropVal,
    float spawnWeightVal,
    float speedVal)
    : Monster(idVal, nameVal, levelMinVal, levelMaxVal, baseHealthVal, baseDamageVal, goldDropVal, spawnWeightVal, speedVal)
{
    Image image = LoadImage("resources/sprites/green_slime.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);

    source.x = 0;
    source.y = 0;
    source.width = (float)texture.width;
    source.height = (float)texture.height;

    std::cout << "Monster (" << nameVal << ") has been created with ID: " << idVal << std::endl;
    std::cout << texture.id << " - " << texture.width << "x" << texture.height << std::endl;
}

void GreenSlime::TakeAction(Grid* grid)
{
    // Implement specific action logic for Green Slime
    // For example, move towards the player or attack
    
    MonsterAction action = GreenSlime::Execute(*this); // You can now use grid if needed
    TraceLog(LOG_INFO, "Green Slime %s is taking action: %s", GetName().c_str(), action.ToString().c_str());
    grid->PlaceMonsterByGridCoordinate(*this, action.position.x, action.position.y); // Assuming grid has a method to perform actions
}

// TODO: Implement other monster subclasses

// ---- [Dark Green Slime] ----
DarkGreenSlime::DarkGreenSlime(
    const std::string &idVal,
    const std::string &nameVal,
    int levelMinVal,
    int levelMaxVal,
    float baseHealthVal,
    float baseDamageVal,
    int goldDropVal,
    float spawnWeightVal,
    float speedVal)
    : Monster(idVal, nameVal, levelMinVal, levelMaxVal, baseHealthVal, baseDamageVal, goldDropVal, spawnWeightVal, speedVal)
{
    // TODO: have a proper green slime
    Image image = LoadImage("resources/sprites/green_slime.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);

    source.x = 0;
    source.y = 0;
    source.width = (float)texture.width;
    source.height = (float)texture.height;

    std::cout << "Monster (" << nameVal << ") has been created with ID: " << idVal << std::endl;
    std::cout << texture.id << " - " << texture.width << "x" << texture.height << std::endl;
}

void DarkGreenSlime::TakeAction(Grid* grid)
{
    // Implement specific action logic for Dark Green Slime
    // For example, move towards the player or attack
    MonsterAction action = DarkGreenSlime::Execute(*this);
    TraceLog(LOG_INFO, "Dark Green Slime %s is taking action: %s", GetName().c_str(), action.ToString().c_str());
    DarkGreenSlime::Execute(*this); // You can now use grid if needed
}
// ---- [White Slime] ----