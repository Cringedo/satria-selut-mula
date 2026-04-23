#pragma once

#include <string>
#include <vector>
#include <map>
#include <self/Entity.hpp> // Now includes Monster.hpp directly

class SpawnManager {
public:
    static SpawnManager& GetInstance();

    SpawnManager(const SpawnManager&) = delete;
    SpawnManager& operator=(const SpawnManager&) = delete;

    bool LoadMonsterData(const std::string& filePath);

    // Now returns const Monster* because the stored monsters are the templates.
    // They are copied when "spawned" into the live game vector.
    const Monster* GetMonsterById(const std::string& id) const;
    std::vector<const Monster*> GetMonstersForLevel(int playerLevel) const;

private:
    SpawnManager();
    ~SpawnManager();

    std::vector<Monster> allMonsterTemplates; // Stores template Monster objects
    std::map<std::string, const Monster*> monsterTemplateMap; // For quick lookup by ID
};