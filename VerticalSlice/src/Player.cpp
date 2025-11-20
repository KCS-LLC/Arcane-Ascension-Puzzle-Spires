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

std::string Player::determineAttunement(const JudgementResults& results, const DataManager& dataManager) const {
    if (results.trialScores.empty()) {
        return "elementalist"; // Default fallback
    }

    std::string bestTrialId = "";
    int maxScore = -1;

    for (const auto& pair : results.trialScores) {
        if (pair.second > maxScore) {
            maxScore = pair.second;
            bestTrialId = pair.first;
        }
    }

    if (bestTrialId == "trial_power") {
        return "executioner";
    } else if (bestTrialId == "trial_haste") {
        return "elementalist";
    } else if (bestTrialId == "trial_control") {
        return "guardian";
    }

    return "elementalist"; // Default fallback
}

const std::string& Player::getAttunementId() const {
    return m_attunementId;
}

void Player::finalizeJudgement(const JudgementResults& results, const DataManager& dataManager) {
    // For now, let's just assign a default attunement for testing
    m_attunementId = determineAttunement(results, dataManager);
    const Attunement* finalAttunement = dataManager.getAttunementById(m_attunementId);

    if (finalAttunement) {
        setAttunement(*finalAttunement, dataManager);
    } else {
        std::cerr << "Could not find final attunement with id: " << m_attunementId << std::endl;
        // Fallback to a default
        const Attunement* fallback = dataManager.getAttunementById("adept");
        if (fallback) {
            setAttunement(*fallback, dataManager);
        }
    }
}
