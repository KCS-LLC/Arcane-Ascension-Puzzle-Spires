#ifndef STRUCTS_H
#define STRUCTS_H

#include "Constants.h"
#include <string>
#include <vector>
#include <map>

struct Spell {
    std::string name;
    std::string description;
    int value;
    std::map<GemType, int> cost;
};

struct Attunement {
    std::string id;
    std::string name;
    std::vector<GemType> primary_mana_types;
};

struct MonsterAbility {
    std::string name;
    int damage;

    MonsterAbility(std::string name = "", int damage = 0) : name(name), damage(damage) {}
};

#endif // STRUCTS_H