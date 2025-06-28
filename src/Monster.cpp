#include <self/Entity.hpp>
#include <self/Constant.h>

Texture2D GetMonsterTexture(const std::string& monsterId) {
    Image image = LoadImage(TextFormat("sprites/%s.png", monsterId.c_str()));
    if (image.data == nullptr) {
        TraceLog(LOG_WARNING, "Failed to load monster texture: sprites/%s.png", monsterId.c_str());
        image = GenImageColor(32, 32, MAGENTA); // Placeholder
    }
    Texture2D tex = LoadTextureFromImage(image);
    UnloadImage(image);
    return tex;
}

Monster::Monster(
    const std::string& idVal,
    const std::string& nameVal,
    int levelMinVal,
    int levelMaxVal,
    float baseHealthVal,
    float baseDamageVal,
    int goldDropVal,
    float spawnWeightVal
) :
    Entity(nameVal), // Call Entity's constructor with the name
    id(idVal),
    levelMin(levelMinVal),
    levelMax(levelMaxVal),
    baseHealth(baseHealthVal),
    baseDamage(baseDamageVal),
    goldDrop(goldDropVal),
    spawnWeight(spawnWeightVal)
    // currentHealth(baseHealthVal) // Initialize current health here if not in Entity
{
    // Initialize inherited members (e.g., position, gridCoordinate) here if not done in Entity
    SetPosition(0,0); // Default to (0,0) world coordinates
    SetGridPosition(0,0); // Default to (0,0) grid coordinates

    texture = GetMonsterTexture(id);
    source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
    // Initialize dest using GetPosition() or default to 0,0 for now
    dest = {GetPosition().x, GetPosition().y, (float)TILE_SIZE * SCALING, (float)TILE_SIZE * SCALING}; // Assume these are defined in Constant.h
}

// Copy constructor implementation
Monster::Monster(const Monster& other) :
    Entity(other), // Copy base Entity part
    id(other.id),
    levelMin(other.levelMin),
    levelMax(other.levelMax),
    baseHealth(other.baseHealth),
    baseDamage(other.baseDamage),
    goldDrop(other.goldDrop),
    spawnWeight(other.spawnWeight),
    // currentHealth(other.currentHealth), // Copy current health if not in Entity
    texture(other.texture), // Textures generally need to be ref-counted or reloaded if not shared
    source(other.source),
    dest(other.dest)
{
    // If textures are unique per instance and LoadTexture is heavy,
    // consider a texture manager or sharing textures. This simple copy is shallow.
    // For raylib, Texture2D is a struct and copying it copies the texture ID,
    // which is fine as long as you only Unload once for all copies.
    // Better to manage textures centrally.
}

int Monster::getLevelMin() const {
    return levelMin;
}

int Monster::getLevelMax() const {
    return levelMax;
}

float Monster::getBaseHealth() const {
    return baseHealth;
}

float Monster::getBaseDamage() const {
    return baseDamage;
}

int Monster::getGoldDrop() const {
    return goldDrop;
}

float Monster::getSpawnWeight() const {
    return spawnWeight;
}

float Monster::calculateHealthAtLevel(int level) const {
    return baseHealth + (level - levelMin) * 5.0f;
}

void Monster::Draw() {
    // dest position might need to be updated from Entity's GetPosition() if it's mutable there
    dest.x = GetPosition().x;
    dest.y = GetPosition().y;
    DrawTexturePro(texture, source, dest, Vector2{0,0}, 0.0f, WHITE);
}