#include "PCH.h"
#include "DataManager.h"
#include "Structs.h"
#include "StringUtils.h"

// =================================================================================
// JSON Conversion Implementations (Centralized Here)
// =================================================================================

using json = nlohmann::json;

void from_json(const json& j, Teleporter& t) {
    t.color = stringToDoorColor(j.at("color").get<std::string>());
    j.at("destinationId").get_to(t.destinationRoomId);
}

void from_json(const json& j, Room& r) {
    j.at("id").get_to(r.id);
    j.at("name").get_to(r.name);
    r.type = stringToRoomType(j.at("type").get<std::string>());
    if (j.contains("monsterId")) {
        j.at("monsterId").get_to(r.monsterId);
    }
    j.at("connections").get_to(r.connections);
}

void from_json(const json& j, Floor& f) {
    j.at("floorNumber").get_to(f.floorNumber);
    j.at("startRoomId").get_to(f.startRoomId);
    j.at("rooms").get_to(f.rooms);
}

// =================================================================================
// DataManager Implementation
// =================================================================================

DataManager::DataManager() : monsterHP(0), monsterSpeed(0), monsterAttackDamage(0) {
    if (!m_font.openFromFile("assets/OpenSans-Regular.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
    }
    if (!loadJudgementTrials()) {
        std::cerr << "Failed to load judgement trials." << std::endl;
    }
    // loadGemDefinitions("data/gem_definitions.json"); // Obsolete
}

const sf::Font& DataManager::getFont() const {
    return m_font;
}
bool DataManager::loadAttunements(const std::string& path) {
    std::vector<std::string> attunementFiles = {
        path,
        "data/executioner.json"
    };

    try {
        for (const auto& filePath : attunementFiles) {
            std::ifstream f(filePath);
            if (!f.is_open()) {
                std::cerr << "Could not open attunement file: " << filePath << std::endl;
                continue;
            }
            
            json data = json::parse(f);
            if (data.is_array()) {
                for (const auto& item : data) {
                    Attunement attunement;
                    attunement.id = item.at("id").get<std::string>();
                    attunement.name = item.at("name").get<std::string>();
                    attunement.description = item.at("description").get<std::string>();
                    attunement.starting_spell_ids = item.at("starting_spells").get<std::vector<int>>();
                    for (const auto& mana_type : item.at("mana_types")) {
                        attunement.mana_types.push_back(stringToGemSubType(mana_type.get<std::string>()));
                    }
                    attunements.push_back(attunement);
                }
            } else if (data.is_object()) {
                Attunement attunement;
                attunement.id = data.at("id").get<std::string>();
                attunement.name = data.at("name").get<std::string>();
                attunement.description = data.at("description").get<std::string>();
                attunement.starting_spell_ids = data.at("starting_spells").get<std::vector<int>>();
                for (const auto& mana_type : data.at("mana_types")) {
                    attunement.mana_types.push_back(stringToGemSubType(mana_type.get<std::string>()));
                }
                attunements.push_back(attunement);
            }
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

const std::vector<Attunement>& DataManager::getAttunements() const { return attunements; }

const Attunement* DataManager::getAttunementById(const std::string& id) const {
    auto it = std::find_if(attunements.begin(), attunements.end(), [&](const Attunement& a) { return a.id == id; });
    return (it != attunements.end()) ? &(*it) : nullptr;
}

const std::vector<Spell>& DataManager::getAllSpells() const { return spells; }

const Spell* DataManager::getSpellById(int id) const {
    auto it = std::find_if(spells.begin(), spells.end(), [id](const Spell& spell) { return spell.id == id; });
    return (it != spells.end()) ? &(*it) : nullptr;
}

int DataManager::getMonsterHP() const { return monsterHP; }
int DataManager::getMonsterSpeed() const { return monsterSpeed; }
int DataManager::getMonsterAttackDamage() const { return monsterAttackDamage; }
std::string DataManager::getMonsterName() const { return monsterName; }

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

const Floor& DataManager::getFloor() const { return currentFloor; }

const Room* DataManager::getRoomById(int roomId) const {
    for (const auto& room : currentFloor.rooms) {
        if (room.id == roomId) {
            return &room;
        }
    }
    return nullptr;
}

bool DataManager::loadJudgementTrials() {
    std::vector<std::string> trialFiles = { "data/trial_power.json", "data/trial_haste.json", "data/trial_control.json" };
    try {
        for (const auto& filePath : trialFiles) {
            std::ifstream f(filePath);
            if (!f.is_open()) {
                std::cerr << "Could not open judgement trial file: " << filePath << std::endl;
                continue;
            }
            json data = json::parse(f);
            JudgementTrial trial;
            trial.trialId = data.at("trialId").get<std::string>();
            trial.objective = data.at("objective").get<std::string>();
            trial.type = static_cast<JudgementTrialType>(data.at("type").get<int>());
            
            // Board layout is no longer loaded this way; it's dynamic.

            trial.turnLimit = data.at("turnLimit").get<int>();
            if (data.contains("timeLimit")) {
                trial.timeLimit = data.at("timeLimit").get<int>();
            }
            trial.scoreGoal = data.at("scoreGoal").get<int>();
            m_judgementTrials.push_back(trial);
        }
    } catch (json::parse_error& e) {
        std::cerr << "JSON parse error in judgement trial files: " << e.what() << std::endl;
        assert(false);
    } catch (json::exception& e) {
        std::cerr << "JSON data error in judgement trial files: " << e.what() << std::endl;
        assert(false);
    }
    assert(!m_judgementTrials.empty() && "No judgement trials were loaded!");
    return true;
}

const std::vector<JudgementTrial>& DataManager::getJudgementTrials() const { return m_judgementTrials; }
