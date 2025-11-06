
#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <string>
#include <vector>

enum class GemType {
    Skull,
    Fire,
    Water,
    Earth,
    Light,
    Empty
};

struct Spell {
    std::string name;
    GemType costType;
    int costAmount;
    int damage;
};

class Player {
public:
    Player(int initialHp) : currentHp(initialHp) {
        mana[GemType::Fire] = 0;
        mana[GemType::Water] = 0;
        mana[GemType::Earth] = 0;
        mana[GemType::Light] = 0;

        // Add four basic spells
        spells.push_back({"Fireball", GemType::Fire, 10, 25});
        spells.push_back({"Ice Lance", GemType::Water, 10, 25});
        spells.push_back({"Rock Throw", GemType::Earth, 10, 25});
        spells.push_back({"Smite", GemType::Light, 10, 25});
    }

    int getCurrentHp() const { return currentHp; }
    int getMana(GemType type) const { return mana.at(type); }
    const std::vector<Spell>& getSpells() const { return spells; }

    void takeDamage(int amount) { currentHp -= amount; }
    void addMana(GemType type, int amount) {
        if (type != GemType::Skull && type != GemType::Empty) {
            mana[type] += amount;
        }
    }

    int castSpell(int spellIndex) {
        if (spellIndex < 0 || spellIndex >= spells.size()) {
            return 0; // Invalid index
        }
        const Spell& spell = spells[spellIndex];
        if (mana[spell.costType] >= spell.costAmount) {
            mana[spell.costType] -= spell.costAmount;
            return spell.damage;
        }
        return 0; // Not enough mana
    }

private:
    int currentHp;
    std::map<GemType, int> mana;
    std::vector<Spell> spells;
};

#endif // PLAYER_H
