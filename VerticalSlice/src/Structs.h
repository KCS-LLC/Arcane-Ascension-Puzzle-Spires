#pragma once

#include "GemSubType.h"
#include "json.hpp"
#include "StringUtils.h"

enum class EffectType { Damage, Heal, Stun, Shield };
enum class TargetType { Player, Monster };

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

struct GemDefinition {
    int id;
    std::string name;
    GemSubType subType;
    std::string texturePath;
};

inline void from_json(const nlohmann::json& j, GemDefinition& gd) {
    j.at("id").get_to(gd.id);
    j.at("name").get_to(gd.name);
    gd.subType = stringToGemSubType(j.at("subType").get<std::string>());
    if (j.contains("texturePath")) {
        j.at("texturePath").get_to(gd.texturePath);
    }
}

struct MonsterData {
    std::string name;
    int hp;
    int speed;
    int attack;
};
