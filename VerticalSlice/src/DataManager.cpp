#include "DataManager.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

// Helper function to convert string to GemType
GemType stringToGemType(const std::string& s) {
    if (s == "Fire") return GemType::Fire;
    if (s == "Water") return GemType::Water;
    if (s == "Earth") return GemType::Earth;
    if (s == "Light") return GemType::Light;
    return GemType::Empty; // Default
}

DataManager::DataManager() : monsterHP(0), monsterSpeed(0), monsterAttackDamage(0) {}

bool DataManager::loadSpells(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "Could not open spell file: " << path << std::endl;
        return false;
    }

    try {
        json data = json::parse(f);
        for (const auto& item : data) {
            Spell spell;
            spell.name = item.at("name").get<std::string>();
            spell.costAmount = item.at("costAmount").get<int>();
            spell.costType = stringToGemType(item.at("costType").get<std::string>());
            spell.damage = item.at("damage").get<int>();
            spells.push_back(spell);
        }
    } catch (json::parse_error& e) {
        std::cerr << "JSON parse error in spell file: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool DataManager::loadMonsterData(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "Could not open monster file: " << path << std::endl;
        return false;
    }

    try {
        json data = json::parse(f);
        monsterHP = data.at("hp").get<int>();
        monsterSpeed = data.at("speed").get<int>();
        monsterAttackDamage = data.at("attackDamage").get<int>();
    } catch (json::parse_error& e) {
        std::cerr << "JSON parse error in monster file: " << e.what() << std::endl;
        return false;
    }
    return true;
}

const std::vector<Spell>& DataManager::getAllSpells() const {
    return spells;
}

int DataManager::getMonsterHP() const {
    return monsterHP;
}

int DataManager::getMonsterSpeed() const {
    return monsterSpeed;
}

int DataManager::getMonsterAttackDamage() const {
    return monsterAttackDamage;
}
