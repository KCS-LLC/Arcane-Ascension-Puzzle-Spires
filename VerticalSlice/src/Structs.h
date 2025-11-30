#pragma once

#include <string>
#include <vector>
#include "GemSubType.h"
#include "PrimaryGemType.h"

// --- Enums ---
enum class RoomType { Entrance, Combat, Treasure, Boss, Sanctuary, Special, Puzzle, Trap, AgilityChallenge, EnduranceChallenge, MagicChallenge, Unknown };
enum class DoorColor { Red, Orange, Yellow, Green, Blue, Indigo, Violet, Copper, Silver, Gold, White, Unknown };
enum class StatusEffect { None, Burning, Frozen, Poisoned, Drained };
enum class ActionState { None, Selected, ValidMoveHint };


#include "json.hpp"

// --- Data Structures ---

struct TargetingRequest {
    int numberOfClicks = 0;
    TargetType type = TargetType::Gem;
    std::string abilityId = ""; // e.g., "quick_swap", "gust_of_wind"
};

struct Effect {
    std::string type;
    std::map<std::string, nlohmann::json> params;
};

struct Spell {
    std::string id;
    std::string name;
    std::string description;
    std::string targetingPrompt;
    GemSubType costType;
    int manaCost;
    int speedCost;
    std::vector<Effect> effects;
};

struct Attunement {
    std::string id;
    std::string name;
    std::string description;
    std::vector<std::string> spellIds;
    std::vector<GemSubType> mana_types;
};

struct ActiveEffect {
    std::string effectId;
    std::string modifier;
    float value;
    float duration;
    float maxDuration;
    bool justAppliedThisTurn = false;
};


// New: Represents an entry from secondary_gem_types.json
struct SecondaryGemTypeData {
    int id;
    std::string name;
    PrimaryGemType primaryType;
    std::string category;
    int oppositeId;
};

// New: Represents an entry from gems.json
struct GemCatalogEntry {
    int id; // Corresponds to GemSubType enum
    std::string name;
    int secondaryTypeId;
    int level;
    std::string texturePath;
};

struct MonsterData {
    std::string id;
    std::string name;
    int rank;
    bool isBoss;
    struct {
        int hp;
        int speed;
        int vigor;
        int wit;
        int attackDamage; // Combined attackDamage into stats
    } stats;
    std::vector<GemSubType> manaAffinities;
    std::vector<std::string> abilities; // IDs of spells the monster can use
    std::map<std::string, float> resistances; // Element string to float multiplier (e.g., "Fire": 0.5)
    std::map<std::string, float> vulnerabilities; // Element string to float multiplier (e.g., "Water": 1.5)
};

struct Teleporter {
    DoorColor color;
    int destinationRoomId;
};

struct Room {
    int id;
    std::string name;
    RoomType type;
    std::string monsterId;
    std::vector<Teleporter> connections;
};

struct Floor {
    int floorNumber;
    std::vector<Room> rooms;
    int startRoomId;
};
