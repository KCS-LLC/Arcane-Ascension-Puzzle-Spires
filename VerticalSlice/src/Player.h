
#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <string>
#include <vector>

enum class GemType {
    Empty,
    Skull,
    // Primary Mana
    Air,
    Fire,
    Earth,
    Water,
    Umbral,
    Light,
    Life,
    Death,
    Mental,
    Perception,
    Transference,
    Enhancement,
    // Action Gems
    Coin
};

struct Spell {
    std::string name;
    GemType costType;
    int costAmount;
    int damage;
};

class Player {
public:
    Player(int initialHp, const std::vector<Spell>& initialSpells);

    int getCurrentHp() const;
    int getMaxHp() const { return maxHp; }
    int getMana(GemType type) const;
    int getMaxMana() const { return maxMana; }
    const std::vector<Spell>& getSpells() const;

    void takeDamage(int amount);
    void addMana(GemType type, int amount);
    int castSpell(int spellIndex);

private:
    int currentHp;
    int maxHp;
    int maxMana = 100; // Default max mana, can be adjusted
    std::map<GemType, int> mana;
    std::vector<Spell> spells;
};

#endif // PLAYER_H
