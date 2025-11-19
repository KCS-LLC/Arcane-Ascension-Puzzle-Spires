#include "PCH.h"
#include "Player.h"
#include "DataManager.h"
#include "Structs.h"

Player::Player(int initialHp, const std::vector<Spell>& initialSpells) 
    : maxHp(initialHp), currentHp(initialHp), spells(initialSpells) {
    mana[GemType::Air] = 0;
    mana[GemType::Fire] = 0;
    mana[GemType::Earth] = 0;
    mana[GemType::Water] = 0;
    mana[GemType::Umbral] = 0;
    mana[GemType::Light] = 0;
    mana[GemType::Life] = 0;
    mana[GemType::Death] = 0;
    mana[GemType::Mental] = 0;
    mana[GemType::Perception] = 0;
    mana[GemType::Transference] = 0;
    mana[GemType::Enhancement] = 0;
}

void Player::setAttunement(const Attunement& attunement, const DataManager& dataManager) {
    spells.clear();
    for (int spellId : attunement.starting_spell_ids) {
        const Spell* spell = dataManager.getSpellById(spellId);
        if (spell) {
            spells.push_back(*spell);
        }
    }
    availableManaTypes = attunement.mana_types;
}

const std::vector<GemType>& Player::getManaTypes() const {
    return availableManaTypes;
}

int Player::getMana(GemType type) const { return mana.at(type); }

int Player::getCurrentHp() const { return currentHp; }

const std::vector<Spell>& Player::getSpells() const { return spells; }

void Player::takeDamage(int amount) { currentHp -= amount; }

void Player::addMana(GemType type, int amount) {
    if (type != GemType::Skull && type != GemType::Empty) {
        mana[type] += amount;
    }
}

int Player::castSpell(int spellIndex) {
    if (spellIndex < 0 || spellIndex >= spells.size()) {
        return 0; // Invalid index
    }
    const Spell& spell = spells[spellIndex];
    if (mana[spell.costType] >= spell.costAmount) {
        mana[spell.costType] -= spell.costAmount;
        // Spell casting logic is now more complex, handled in Game.cpp
        return 1; // Indicate success
    }
    return 0; // Not enough mana
}
