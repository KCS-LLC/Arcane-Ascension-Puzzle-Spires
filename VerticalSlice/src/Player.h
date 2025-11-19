#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <string>
#include <vector>
#include "Structs.h"
#include "GemType.h"

// Forward declarations
class DataManager;

class Player {
public:
    Player(int initialHp, const std::vector<Spell>& initialSpells);

    void setAttunement(const Attunement& attunement, const DataManager& dataManager);

    int getCurrentHp() const;
    int getMaxHp() const { return maxHp; }
    int getMana(GemType type) const;
    int getMaxMana() const { return maxMana; }
    const std::vector<Spell>& getSpells() const;
    const std::vector<GemType>& getManaTypes() const;

    void takeDamage(int amount);
    void addMana(GemType type, int amount);
    int castSpell(int spellIndex);

private:
    int currentHp;
    int maxHp;
    int maxMana = 100; // Default max mana, can be adjusted
    std::map<GemType, int> mana;
    std::vector<Spell> spells;
    std::vector<GemType> availableManaTypes;
};

#endif // PLAYER_H
