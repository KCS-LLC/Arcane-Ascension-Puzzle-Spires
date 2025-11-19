#include "PCH.h"
#include "Player.h"
#include "DataManager.h"
#include "Structs.h"

Player::Player(int initialHp, const std::vector<Spell>& initialSpells) 
    : maxHp(initialHp), currentHp(initialHp), spells(initialSpells) {
    // Initialize all possible mana subtypes to 0
    for (int i = static_cast<int>(GemSubType::Fire); i <= static_cast<int>(GemSubType::Raw); ++i) {
        mana[static_cast<GemSubType>(i)] = 0;
    }
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

const std::vector<GemSubType>& Player::getManaTypes() const {
    return availableManaTypes;
}

int Player::getMana(GemSubType type) const { 
    auto it = mana.find(type);
    if (it != mana.end()) {
        return it->second;
    }
    return 0; // Return 0 if mana type not found, though it should be initialized
}

int Player::getCurrentHp() const { return currentHp; }

const std::vector<Spell>& Player::getSpells() const { return spells; }

void Player::takeDamage(int amount) { currentHp -= amount; }

void Player::addMana(GemSubType type, int amount) {
    auto it = mana.find(type);
    if (it != mana.end()) {
        it->second += amount;
        if (it->second > maxMana) {
            it->second = maxMana;
        }
    }
}

int Player::castSpell(int spellIndex) {
    if (spellIndex < 0 || spellIndex >= spells.size()) {
        return 0; // Invalid index
    }
    const Spell& spell = spells[spellIndex];
    if (getMana(spell.costType) >= spell.costAmount) {
        mana[spell.costType] -= spell.costAmount;
        // Spell casting logic is now more complex, handled in Game.cpp
        return 1; // Indicate success
    }
    return 0; // Not enough mana
}
