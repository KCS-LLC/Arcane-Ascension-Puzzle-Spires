#include "PCH.h"
#include "DataManager.h"
#include "StringUtils.h"

// =================================================================================
// JSON Conversion Implementations
// =================================================================================

using json = nlohmann::json;

void from_json(const json& j, SecondaryGemTypeData& sgtd) {
    j.at("id").get_to(sgtd.id);
    j.at("name").get_to(sgtd.name);
    sgtd.primaryType = static_cast<PrimaryGemType>(j.at("primaryTypeId").get<int>());
    j.at("category").get_to(sgtd.category);
    if (j.contains("oppositeId") && !j.at("oppositeId").is_null()) {
        j.at("oppositeId").get_to(sgtd.oppositeId);
    } else {
        sgtd.oppositeId = -1;
    }
}

void from_json(const json& j, GemCatalogEntry& gce) {
    j.at("id").get_to(gce.id);
    j.at("name").get_to(gce.name);
    if (j.contains("secondaryTypeId") && !j.at("secondaryTypeId").is_null()) {
        j.at("secondaryTypeId").get_to(gce.secondaryTypeId);
    } else {
        gce.secondaryTypeId = -1;
    }
    j.at("level").get_to(gce.level);
    if (j.contains("texturePath") && !j.at("texturePath").is_null()) {
        j.at("texturePath").get_to(gce.texturePath);
    } else {
        gce.texturePath = "";
    }
}

void from_json(const json& j, Teleporter& t) {
    t.color = stringToDoorColor(j.at("color").get<std::string>());
    j.at("destinationId").get_to(t.destinationRoomId);
}

void from_json(const json& j, Room& r) {
    j.at("id").get_to(r.id);
    j.at("name").get_to(r.name);
    r.type = stringToRoomType(j.at("type").get<std::string>());
    if (j.contains("monsterId")) { j.at("monsterId").get_to(r.monsterId); }
    j.at("connections").get_to(r.connections);
}

void from_json(const json& j, JudgementTrial& trial) {
    j.at("trialId").get_to(trial.trialId);
    j.at("objective").get_to(trial.objective);
    trial.type = static_cast<JudgementTrialType>(j.at("type").get<int>());
    j.at("turnLimit").get_to(trial.turnLimit);
    if (j.contains("timeLimit")) {
        j.at("timeLimit").get_to(trial.timeLimit);
    }
    j.at("scoreGoal").get_to(trial.scoreGoal);
}

void from_json(const json& j, Attunement& a) {
    j.at("id").get_to(a.id);
    j.at("name").get_to(a.name);
    j.at("description").get_to(a.description);
    j.at("spellIds").get_to(a.spellIds);
    
    a.mana_types.clear();
    for (int type_id : j.at("mana_types")) {
        a.mana_types.push_back(static_cast<GemSubType>(type_id));
    }
}

void from_json(const json& j, Effect& e) {
    j.at("type").get_to(e.type);
    if (j.contains("params")) {
        j.at("params").get_to(e.params);
    }
}

void from_json(const json& j, Spell& s) {
    j.at("id").get_to(s.id);
    j.at("name").get_to(s.name);
    if (j.contains("description")) {
        j.at("description").get_to(s.description);
    }
    if (j.contains("targeting_prompt")) {
        j.at("targeting_prompt").get_to(s.targetingPrompt);
    }
    j.at("manaCost").get_to(s.manaCost);
    s.costType = stringToGemSubType(j.at("manaType").get<std::string>());
    j.at("speedCost").get_to(s.speedCost);
    if (j.contains("effects")) {
        j.at("effects").get_to(s.effects);
    }
}

void from_json(const json& j, MonsterData& md) {
    j.at("name").get_to(md.name);
    j.at("hp").get_to(md.hp);
    j.at("speed").get_to(md.speed);
    j.at("attackDamage").get_to(md.attack);
    if (j.contains("manaAffinities")) {
        md.manaAffinities.clear();
        for (int type_id : j.at("manaAffinities")) {
            md.manaAffinities.push_back(static_cast<GemSubType>(type_id));
        }
    }
}

void from_json(const json& j, Floor& f) {
    j.at("floorNumber").get_to(f.floorNumber);
    j.at("startRoomId").get_to(f.startRoomId);
    j.at("rooms").get_to(f.rooms);
}

// =================================================================================
// DataManager Implementation
// =================================================================================

DataManager::DataManager() {
    if (!m_font.openFromFile("assets/OpenSans-Regular.ttf")) {
        std::cerr << "Failed to load font." << '\n';
    }
    if (!loadPrimaryGemTypes("data/primary_gem_types.json")) {
        std::cerr << "Failed to load primary gem types." << '\n';
    }
    if (!loadSecondaryGemTypes("data/secondary_gem_types.json")) {
        std::cerr << "Failed to load secondary gem types." << '\n';
    }
    if (!loadGemCatalog("data/gems.json")) {
        std::cerr << "Failed to load gem catalog." << '\n';
    }
    if (!loadAttunements("data/attunements.json")) {
        std::cerr << "Failed to load attunements." << '\n';
    }
    if (!loadSpells("data/spells.json")) {
        std::cerr << "Failed to load spells." << '\n';
    }
    if (!loadMonsterData("data/monster.json")) {
        std::cerr << "Failed to load monster data." << '\n';
    }
    if (!loadFloor("data/floor1.json")) {
        std::cerr << "Failed to load floor data." << '\n';
    }
    if (!loadJudgementTrials()) {
        std::cerr << "Failed to load judgement trials." << '\n';
    }
}

bool DataManager::loadPrimaryGemTypes(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return false;
    try {
        json data = json::parse(f);
        for (const auto& item : data) {
            m_primaryGemTypes[static_cast<PrimaryGemType>(item.at("id").get<int>())] = item.at("name").get<std::string>();
        }
    } catch (const json::exception& e) {
        std::cerr << "JSON error in primary gem types: " << e.what() << '\n';
        return false;
    }
    return true;
}

bool DataManager::loadSecondaryGemTypes(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return false;
    try {
        json data = json::parse(f);
        for (const auto& item : data) {
            m_secondaryGemTypes[item.at("id").get<int>()] = item.get<SecondaryGemTypeData>();
        }
    } catch (const json::exception& e) {
        std::cerr << "JSON error in secondary gem types: " << e.what() << '\n';
        return false;
    }
    return true;
}

bool DataManager::loadGemCatalog(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return false;
    try {
        json data = json::parse(f);
        for (const auto& item : data) {
            m_gemCatalog[static_cast<GemSubType>(item.at("id").get<int>())] = item.get<GemCatalogEntry>();
        }
    } catch (const json::exception& e) {
        std::cerr << "JSON error in gem catalog: " << e.what() << '\n';
        return false;
    }
    return true;
}

bool DataManager::loadAttunements(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return false;
    try {
        json data = json::parse(f);
        attunements = data.get<std::vector<Attunement>>();
    } catch (const json::exception& e) {
        std::cerr << "JSON error in attunements: " << e.what() << '\n';
        return false;
    }
    return true;
}
bool DataManager::loadSpells(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return false;
    try {
        json data = json::parse(f);
        spells = data.get<std::vector<Spell>>();
    } catch (const json::exception& e) {
        std::cerr << "JSON error in spells: " << e.what() << '\n';
        return false;
    }
    return true;
}
bool DataManager::loadMonsterData(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "Could not open monster file: " << path << '\n';
        return false;
    }
    try {
        json data = json::parse(f);
        m_monsterData = data.get<MonsterData>();
    } catch (const json::exception& e) {
        std::cerr << "JSON error in monster data: " << e.what() << '\n';
        return false;
    }
    return true;
}
bool DataManager::loadFloor(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return false;
    try {
        json data = json::parse(f);
        currentFloor = data.get<Floor>();
    } catch (const json::exception& e) {
        std::cerr << "JSON error in floor data: " << e.what() << '\n';
        return false;
    }
    return true;
}
bool DataManager::loadJudgementTrials() {
    const std::vector<std::string> trialFiles = {
        "data/trial_power.json",
        "data/trial_haste.json",
        "data/trial_control.json"
    };

    for (const auto& filePath : trialFiles) {
        std::ifstream f(filePath);
        if (!f.is_open()) {
            std::cerr << "Failed to open trial file: " << filePath << '\n';
            return false;
        }
        try {
            json data = json::parse(f);
            m_judgementTrials.push_back(data.get<JudgementTrial>());
        } catch (const json::exception& e) {
            std::cerr << "JSON error in " << filePath << ": " << e.what() << '\n';
            return false;
        }
    }
    return true;
}

const GemCatalogEntry* DataManager::getGemCatalogEntry(GemSubType subType) const {
    auto it = m_gemCatalog.find(subType);
    return (it != m_gemCatalog.end()) ? &it->second : nullptr;
}

const std::map<GemSubType, GemCatalogEntry>& DataManager::getGemCatalog() const {
    return m_gemCatalog;
}

const SecondaryGemTypeData* DataManager::getSecondaryGemTypeData(int secondaryTypeId) const {
    auto it = m_secondaryGemTypes.find(secondaryTypeId);
    return (it != m_secondaryGemTypes.end()) ? &it->second : nullptr;
}

PrimaryGemType DataManager::getPrimaryGemType(GemSubType subType) const {
    const GemCatalogEntry* gce = getGemCatalogEntry(subType);
    if (gce != nullptr && gce->secondaryTypeId != -1) {
        const SecondaryGemTypeData* sgtd = getSecondaryGemTypeData(gce->secondaryTypeId);
        if (sgtd != nullptr) {
            return sgtd->primaryType;
        }
    }
    return PrimaryGemType::Generic;
}

const std::vector<Attunement>& DataManager::getAttunements() const { return attunements; }
const Attunement* DataManager::getAttunementById(const std::string& id) const {
    auto it = std::find_if(attunements.begin(), attunements.end(), [&](const Attunement& a) {
        return a.id == id;
    });

    if (it != attunements.end()) {
        return &(*it);
    }
    
    return nullptr; 
}
const std::vector<Spell>& DataManager::getAllSpells() const { return spells; }
const Spell* DataManager::getSpellById(const std::string& id) const {
    auto it = std::find_if(spells.begin(), spells.end(), [id](const Spell& spell) {
        return spell.id == id;
    });
    if (it != spells.end()) {
        return &(*it);
    }
    return nullptr;
}
const MonsterData& DataManager::getMonsterData() const { return m_monsterData; }
int DataManager::getMonsterHP() const { return m_monsterData.hp; }
int DataManager::getMonsterSpeed() const { return m_monsterData.speed; }
int DataManager::getMonsterAttackDamage() const { return m_monsterData.attack; }
std::string DataManager::getMonsterName() const { return m_monsterData.name; }
const std::vector<GemSubType>& DataManager::getMonsterManaAffinities() const { return m_monsterData.manaAffinities; }
const Floor& DataManager::getFloor() const { return currentFloor; }
const Room* DataManager::getRoomById(int roomId) const { return nullptr; }
const std::vector<JudgementTrial>& DataManager::getJudgementTrials() const { return m_judgementTrials; }
const sf::Font& DataManager::getFont() const { return m_font; }
