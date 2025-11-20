#include "PCH.h"
#include "DataManager.h"
#include "Structs.h"
#include "SpireData.h"
#include "json.hpp"
#include "StringUtils.h"

using json = nlohmann::json;

DataManager::DataManager() : monsterHP(0), monsterSpeed(0), monsterAttackDamage(0) {
    loadJudgementTrials();
}

bool DataManager::loadAttunements(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "Could not open attunement file: " << path << std::endl;
        return false;
    }

    try {
        json data = json::parse(f);
        for (const auto& item : data) {
            Attunement attunement;
            attunement.id = item.at("id").get<std::string>();
            attunement.name = item.at("name").get<std::string>();
            attunement.description = item.at("description").get<std::string>();
            
            for (const auto& spell_id : item.at("starting_spells")) {
                attunement.starting_spell_ids.push_back(spell_id.get<int>());
            }

            for (const auto& mana_type : item.at("mana_types")) {
                attunement.mana_types.push_back(stringToGemSubType(mana_type.get<std::string>()));
            }
            attunements.push_back(attunement);
        }
    } catch (json::parse_error& e) {
        std::cerr << "JSON parse error in attunement file: " << e.what() << std::endl;
        return false;
    }
    return true;
}


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
            spell.id = item.at("id").get<int>();
            spell.name = item.at("name").get<std::string>();
            spell.costAmount = item.at("costAmount").get<int>();
            spell.costType = stringToGemSubType(item.at("costType").get<std::string>());
            
            spell.effects.clear();
            for (const auto& effect_data : item.at("effects")) {
                Effect effect;
                effect.type = stringToEffectType(effect_data.at("type").get<std::string>());
                effect.amount = effect_data.at("amount").get<int>();
                effect.target = stringToTargetType(effect_data.at("target").get<std::string>());
                spell.effects.push_back(effect);
            }
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
        monsterName = data.at("name").get<std::string>();
    } catch (json::parse_error& e) {
        std::cerr << "JSON parse error in monster file: " << e.what() << std::endl;
        return false;
    }
    return true;
}

const std::vector<Attunement>& DataManager::getAttunements() const {
    return attunements;
}

const std::vector<Spell>& DataManager::getAllSpells() const {
    return spells;
}

const Spell* DataManager::getSpellById(int id) const {
    auto it = std::find_if(spells.begin(), spells.end(), [id](const Spell& spell) {
        return spell.id == id;
    });
    if (it != spells.end()) {
        return &(*it);
    }
    return nullptr;
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

std::string DataManager::getMonsterName() const {
    return monsterName;
}

bool DataManager::loadFloor(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "Could not open floor file: " << path << std::endl;
        return false;
    }

    try {
        json data = json::parse(f);
        currentFloor = data.get<Floor>();
    } catch (json::parse_error& e) {
        std::cerr << "JSON parse error in floor file: " << e.what() << std::endl;
        return false;
    } catch (json::exception& e) {
        std::cerr << "JSON data error in floor file: " << e.what() << std::endl;
        return false;
    }
    return true;
}

const Floor& DataManager::getFloor() const {
    return currentFloor;
}

const Room* DataManager::getRoomById(int roomId) const {
    for (const auto& room : currentFloor.rooms) {
        if (room.id == roomId) {
            return &room;
        }
    }
    return nullptr;
}

void DataManager::loadJudgementTrials() {
    std::vector<std::string> trialFiles = {
        "data/trial_power.json",
        "data/trial_haste.json",
        "data/trial_control.json"
    };

    for (const auto& filePath : trialFiles) {
        std::ifstream f(filePath);
        if (!f.is_open()) {
            std::cerr << "Could not open judgement trial file: " << filePath << std::endl;
            continue;
        }

        try {
            json data = json::parse(f);
            JudgementTrial trial;
            std::string typeStr = data.at("type").get<std::string>();
            if (typeStr == "Power") {
                trial.type = JudgementTrialType::Power;
            } else if (typeStr == "Haste") {
                trial.type = JudgementTrialType::Haste;
            } else if (typeStr == "Control") {
                trial.type = JudgementTrialType::Control;
            } else {
                std::cerr << "Unknown JudgementTrialType: " << typeStr << " in " << filePath << std::endl;
                continue;
            }
            trial.boardLayoutFile = data.at("boardLayout").get<std::string>();
            trial.turnLimit = data.at("turnLimit").get<int>();
            trial.scoreGoal = data.at("scoreGoal").get<int>();
            m_judgementTrials.push_back(trial);

        } catch (json::parse_error& e) {
            std::cerr << "JSON parse error in judgement trial file " << filePath << ": " << e.what() << std::endl;
        } catch (json::exception& e) {
            std::cerr << "JSON data error in judgement trial file " << filePath << ": " << e.what() << std::endl;
        }
    }
}

const std::vector<JudgementTrial>& DataManager::getJudgementTrials() const {
    return m_judgementTrials;
}
