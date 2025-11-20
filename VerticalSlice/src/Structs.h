#pragma once

#include <string>
#include <vector>
#include "GemSubType.h"
#include "PrimaryGemType.h"

// --- Enums ---
enum class EffectType { Damage, Heal, Stun, Shield };
enum class TargetType { Player, Monster };
enum class RoomType { Entrance, Combat, Treasure, Boss, Sanctuary, Special, Puzzle, Trap, AgilityChallenge, EnduranceChallenge, MagicChallenge, Unknown };
enum class DoorColor { Red, Orange, Yellow, Green, Blue, Indigo, Violet, Copper, Silver, Gold, White, Unknown };


// --- Data Structures ---

struct Effect {
    EffectType type;
    int amount;
    TargetType target;
};

struct Spell {
    int id;
    std::string name;
    GemSubType costType;
    int costAmount;
    std::vector<Effect> effects;
};

struct Attunement {
    std::string id;
    std::string name;
    std::string description;
    std::vector<int> starting_spell_ids;
    std::vector<GemSubType> mana_types;
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
