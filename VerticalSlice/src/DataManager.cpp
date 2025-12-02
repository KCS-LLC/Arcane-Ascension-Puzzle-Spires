#include "PCH.h"
#include "DataManager.h"
#include "StringUtils.h"


// =================================================================================
// JSON Conversion Implementations
// =================================================================================

using json = nlohmann::json;

void from_json(const json& j, TargetingData& td) {
    j.at("type").get_to(td.type);
    j.at("prompt").get_to(td.prompt);
    j.at("numberOfClicks").get_to(td.numberOfClicks);
}

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
    if (j.contains("targeting")) {
        s.targeting = j.at("targeting").get<TargetingData>();
    } else {
        s.targeting = std::nullopt;
    }
    j.at("manaCost").get_to(s.manaCost);
    s.costType = stringToGemSubType(j.at("manaType").get<std::string>());
    j.at("speedCost").get_to(s.speedCost);
    if (j.contains("effects")) {
        j.at("effects").get_to(s.effects);
    }
}

void from_json(const json& j, MonsterData& md) {
    j.at("id").get_to(md.id);
    j.at("name").get_to(md.name);
    j.at("rank").get_to(md.rank);
    j.at("isBoss").get_to(md.isBoss);
    j.at("stats").at("hp").get_to(md.stats.hp);
    j.at("stats").at("speed").get_to(md.stats.speed);
    j.at("stats").at("vigor").get_to(md.stats.vigor);
    j.at("stats").at("wit").get_to(md.stats.wit);
    j.at("stats").at("attackDamage").get_to(md.stats.attackDamage);

    // Parse manaAffinities from string to GemSubType
    md.manaAffinities.clear();
    for (const auto& affinity_str : j.at("manaAffinities")) {
        md.manaAffinities.push_back(stringToGemSubType(affinity_str.get<std::string>()));
    }

    // Parse abilities
    if (j.contains("abilities")) {
        j.at("abilities").get_to(md.abilities);
    }

    // Parse resistances
    if (j.contains("resistances")) {
        j.at("resistances").get_to(md.resistances);
    }

    // Parse vulnerabilities
    if (j.contains("vulnerabilities")) {
        j.at("vulnerabilities").get_to(md.vulnerabilities);
    }
}

void from_json(const json& j, Floor& f) {
    j.at("floorNumber").get_to(f.floorNumber);
    j.at("startRoomId").get_to(f.startRoomId);
    j.at("rooms").get_to(f.rooms);
}

void from_json(const json& j, ItemBase& ib) {
    j.at("id").get_to(ib.id);
    j.at("name").get_to(ib.name);
    j.at("description").get_to(ib.description);
    // TODO: Need a stringToEquipmentSlot function
    // For now, manual mapping:
    std::string slotStr = j.at("slot").get<std::string>();
    if (slotStr == "Weapon") { ib.slot = EquipmentSlot::Weapon; }
    else if (slotStr == "OffHand") { ib.slot = EquipmentSlot::OffHand; }
    else if (slotStr == "Head") { ib.slot = EquipmentSlot::Head; }
    else if (slotStr == "Body") { ib.slot = EquipmentSlot::Body; }
    else if (slotStr == "Legs") { ib.slot = EquipmentSlot::Legs; }
    else if (slotStr == "Feet") { ib.slot = EquipmentSlot::Feet; }
    else if (slotStr == "Hands") { ib.slot = EquipmentSlot::Hands; }
    else if (slotStr == "Ring1") { ib.slot = EquipmentSlot::Ring1; }
    else if (slotStr == "Ring2") { ib.slot = EquipmentSlot::Ring2; }

    if (j.contains("base_damage")) {
        j.at("base_damage").get_to(ib.baseDamage);
    }
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
    if (!loadAllMonsters("data/monster_templates.json")) { // Changed to loadAllMonsters and new path
        std::cerr << "Failed to load all monster data." << '\n';
    }
    if (!loadFloor("data/floor1.json")) {
        std::cerr << "Failed to load floor data." << '\n';
    }
    if (!loadJudgementTrials()) {
        std::cerr << "Failed to load judgement trials." << '\n';
    }
    if (!loadConfig("data/config.json")) {
        std::cerr << "Failed to load config." << '\n';
    }
    if (!loadItemBases("data/item_bases.json")) {
        std::cerr << "Failed to load item bases." << '\n';
    }
}

bool DataManager::loadConfig(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) { return false; }
    try {
        json data = json::parse(f);
        const auto& timings = data.at("animation_timings");
        timings.at("swap_duration_ms").get_to(m_animationTimings.swap_duration_ms);
        timings.at("destroy_duration_ms").get_to(m_animationTimings.destroy_duration_ms);
        timings.at("fall_duration_ms").get_to(m_animationTimings.fall_duration_ms);
        timings.at("rotate_duration_ms").get_to(m_animationTimings.rotate_duration_ms);
    } catch (const json::exception& e) {
        std::cerr << "JSON error in config: " << e.what() << '\n';
        return false;
    }
    return true;
}

bool DataManager::loadPrimaryGemTypes(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) { return false; }
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
    if (!f.is_open()) { return false; }
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
    if (!f.is_open()) { return false; }
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
    if (!f.is_open()) { return false; }
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
    if (!f.is_open()) { return false; }
    try {
        json data = json::parse(f);
        spells = data.get<std::vector<Spell>>();
    } catch (const json::exception& e) {
        std::cerr << "JSON error in spells: " << e.what() << '\n';
        return false;
    }
    return true;
}
bool DataManager::loadAllMonsters(const std::string& path) { // Renamed from loadMonsterData
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "Could not open monster file: " << path << '\n';
        return false;
    }
    try {
        json data = json::parse(f);
        m_allMonsters = data.get<std::vector<MonsterData>>(); // Load all monsters into the vector
    } catch (const json::exception& e) {
        std::cerr << "JSON error in monster data: " << e.what() << '\n';
        return false;
    }
    return true;
}
bool DataManager::loadFloor(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) { return false; }
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

bool DataManager::loadItemBases(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) { return false; }
    try {
        json data = json::parse(f);
        for (const auto& item : data) {
            ItemBase base = item.get<ItemBase>();
            m_itemBases[base.id] = base;
        }
    } catch (const json::exception& e) {
        std::cerr << "JSON error in item bases: " << e.what() << '\n';
        return false;
    }
    return true;
}

const ItemBase* DataManager::getItemBase(const std::string& id) const {
    auto it = m_itemBases.find(id);
    if (it != m_itemBases.end()) {
        return &it->second;
    }
    return nullptr;
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

const MonsterData* DataManager::getRandomMonsterByRank(int rank, bool isBoss) const {
    std::vector<const MonsterData*> candidates;
    for (const auto& monster : m_allMonsters) {
        if (monster.rank == rank && monster.isBoss == isBoss) {
            candidates.push_back(&monster);
        }
    }

    if (candidates.empty()) {
        std::cerr << "No monster found for rank " << rank << " and isBoss " << (isBoss ? "true" : "false") << '\n';
        return nullptr; // No matching monster found
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, static_cast<int>(candidates.size() - 1));
    
    return candidates[distrib(gen)];
}

const Floor& DataManager::getFloor() const { return currentFloor; }
const Room* DataManager::getRoomById(int roomId) const {
    auto it = std::find_if(currentFloor.rooms.begin(), currentFloor.rooms.end(), 
                           [roomId](const Room& r) { return r.id == roomId; });
    if (it != currentFloor.rooms.end()) {
        return &(*it);
    }
    return nullptr;
}
const std::vector<JudgementTrial>& DataManager::getJudgementTrials() const { return m_judgementTrials; }
const sf::Font& DataManager::getFont() const { return m_font; }
const AnimationTimings& DataManager::getAnimationTimings() const { return m_animationTimings; }