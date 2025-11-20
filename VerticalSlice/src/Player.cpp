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

void Player::setStartingStats(int tactical_score, int mana_affinity_score) {
    // Basic formula: More tactical proficiency yields more HP, more mana affinity yields more Mana.
    // This can be expanded with more complex calculations, curves, and caps.
    maxHp = 100 + (tactical_score / 10); // e.g., 1 HP for every 10 points
    maxMana = 100 + (mana_affinity_score / 5); // e.g., 1 Mana for every 5 points

    currentHp = maxHp; // Start with full health
}

void Player::finalizeJudgement(const JudgementResults& results) {
    int tactical_score = 0;
    int mana_affinity_score = 0;

    for (const auto& pair : results.trialScores) {
        switch (pair.first) {
            case JudgementTrialType::Tactical:
            case JudgementTrialType::Power:
            case JudgementTrialType::Haste:
            case JudgementTrialType::Control:
                tactical_score += pair.second;
                break;
            case JudgementTrialType::ManaAffinity:
                mana_affinity_score += pair.second;
                break;
        }
    }

    setStartingStats(tactical_score, mana_affinity_score);

    // Future logic could also award starting items or spells based on Treasure scores, etc.
}
