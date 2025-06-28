#include <self/SpawnManager.hpp>
#include <iostream>
#include <fstream>
#include <self/json.hpp> // nlohmann/json library

using json = nlohmann::json;

SpawnManager& SpawnManager::GetInstance() {
    static SpawnManager instance;
    return instance;
}

SpawnManager::SpawnManager() {}
SpawnManager::~SpawnManager() {}

bool SpawnManager::LoadMonsterData(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open JSON file: " << filePath << std::endl;
        return false;
    }

    try {
        json data = json::parse(file);

        if (!data.is_array()) {
            std::cerr << "Error: JSON root is not an array in " << filePath << std::endl;
            return false;
        }

        for (const auto& monsterJson : data) {
            std::string id = monsterJson.at("id").get<std::string>();
            std::string name = monsterJson.at("name").get<std::string>();
            int levelMin = monsterJson.at("levelMin").get<int>();
            int levelMax = monsterJson.at("levelMax").get<int>();
            float baseHealth = monsterJson.at("baseHealth").get<float>();
            float baseDamage = monsterJson.at("baseDamage").get<float>();
            int goldDrop = monsterJson.at("goldDrop").get<int>();
            float spawnWeight = monsterJson.at("spawnWeight").get<float>();

            // Create Monster object and add to vector
            allMonsterTemplates.emplace_back(
                id, name, levelMin, levelMax,
                baseHealth, baseDamage, goldDrop, spawnWeight
            );
            
            // Store a pointer to the newly added template monster
            monsterTemplateMap[id] = &allMonsterTemplates.back();
        }
        std::cout << "Successfully loaded " << allMonsterTemplates.size() << " monster templates from " << filePath << std::endl;
        return true;

    } catch (const json::exception& e) {
        std::cerr << "JSON parse error in " << filePath << ": " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Standard library error while parsing " << filePath << ": " << e.what() << std::endl;
        return false;
    }
}

const Monster* SpawnManager::GetMonsterById(const std::string& id) const {
    auto it = monsterTemplateMap.find(id);
    if (it != monsterTemplateMap.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<const Monster*> SpawnManager::GetMonstersForLevel(int playerLevel) const {
    std::vector<const Monster*> validMonsters;
    for (const auto& monster : allMonsterTemplates) {
        if (playerLevel >= monster.getLevelMin() && playerLevel <= monster.getLevelMax()) {
            validMonsters.push_back(&monster);
        }
    }
    return validMonsters;
}