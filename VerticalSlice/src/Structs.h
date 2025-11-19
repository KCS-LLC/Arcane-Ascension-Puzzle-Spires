#pragma once

#include "GemType.h"

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
    GemType costType;
    int costAmount;
    std::vector<Effect> effects;
};

struct Attunement {
    std::string id;
    std::string name;
    std::string description;
    std::vector<int> starting_spell_ids;
    std::vector<GemType> mana_types;
};

struct MonsterData {
    std::string name;
    int hp;
    int speed;
    int attack;
};
