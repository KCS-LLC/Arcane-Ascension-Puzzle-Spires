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

struct Effect {
    std::string type;
    std::map<std::string, nlohmann::json> params;
};

struct Spell {
    std::string id;
    std::string name;
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
    std::string name;
    int hp;
    int speed;
    int attack;
    std::vector<GemSubType> manaAffinities;
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
