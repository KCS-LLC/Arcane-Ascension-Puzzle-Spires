#include "PCH.h"
#include "Player.h"
#include "DataManager.h"
#include "Structs.h"
#include "StringUtils.h"

Player::Player(int initialHp, const std::vector<Spell>& initialSpells)
    : m_hp(initialHp), m_maxHp(initialHp), m_maxMana(100), spells(initialSpells) {
    // Initialize all possible mana subtypes to 0
    for (int i = static_cast<int>(GemSubType::Fire); i <= static_cast<int>(GemSubType::Grey); ++i) { // Use Grey as upper bound
        mana[static_cast<GemSubType>(i)] = 0;
    }
}

void Player::setAttunement(const Attunement& attunement, const DataManager& dataManager) {
    spells.clear();
    for (const std::string& spellId : attunement.spellIds) {
        const Spell* spell = dataManager.getSpellById(spellId);
        if (spell != nullptr) {
            spells.push_back(*spell);
        }
    }
    m_availableManaTypes = attunement.mana_types;
}

const std::vector<GemSubType>& Player::getManaTypes() const {
    return m_availableManaTypes;
}

int Player::getMana(GemSubType type) const {
    auto it = mana.find(type);
    if (it != mana.end()) {
        return it->second;
    }
    return 0;
}

int Player::getHp() const { return m_hp; }
int Player::getMaxHp() const { return m_maxHp; }
int Player::getMaxMana() const { return m_maxMana; }

const std::vector<Spell>& Player::getSpells() const { return spells; }

GemSubType Player::getPrimaryManaType() const {
    if (!m_availableManaTypes.empty()) {
        return m_availableManaTypes[0];
    }
    return GemSubType::Empty; // Or a suitable default/error value
}

void Player::takeDamage(int amount) {
    m_hp -= amount;
    if (m_hp < 0) m_hp = 0;
}

void Player::heal(int amount) {
    m_hp += amount;
    if (m_hp > m_maxHp) m_hp = m_maxHp;
}

void Player::addScore(int amount) {
    m_score += amount;
}

void Player::addMana(GemSubType type, int amount) {
    auto it = mana.find(type);
    if (it != mana.end()) {
        it->second += amount;
        if (it->second > m_maxMana) {
            it->second = m_maxMana;
        }
        std::cout << "[MANA GAIN] Gained " << amount << " " << gemSubtypeToString(type) << " mana." << '\n';
    }
}

const Spell* Player::castSpell(int spellIndex) {
    if (spellIndex < 0 || spellIndex >= spells.size()) {
        return nullptr; // Invalid index
    }
    const Spell& spell = spells[spellIndex];
    if (getMana(spell.costType) >= spell.manaCost) {
        mana[spell.costType] -= spell.manaCost;
        return &spell; // Return pointer to the cast spell
    }
    return nullptr; // Not enough mana
}

void Player::setStartingStats(int tactical_score, int mana_affinity_score) {
    m_maxHp = 100 + (tactical_score / 10);
    m_maxMana = 100 + (mana_affinity_score / 5);
    m_hp = m_maxHp; // Start with full health
}

std::string Player::determineAttunement(const JudgementResults& results, const DataManager& dataManager) const {
    if (results.powerScore >= results.hasteScore && results.powerScore >= results.controlScore) {
        return "executioner";
    } else if (results.hasteScore >= results.powerScore && results.hasteScore >= results.controlScore) {
        return "elementalist";
    } else {
        return "guardian";
    }
}

const std::string& Player::getAttunementId() const {
    return m_attunementId;
}

int Player::getVigor() const {
    return m_vigor + static_cast<int>(getStatModifier("vigor"));
}

void Player::finalizeJudgement(const JudgementResults& results, const DataManager& dataManager) {
    m_attunementId = determineAttunement(results, dataManager);
    const Attunement* finalAttunement = dataManager.getAttunementById(m_attunementId);

    if (finalAttunement != nullptr) {
        setAttunement(*finalAttunement, dataManager);
    } else {
        std::cerr << "Could not find final attunement with id: " << m_attunementId << '\n';
        const Attunement* fallback = dataManager.getAttunementById("adept");
            if (fallback != nullptr) {
                    setAttunement(*fallback, dataManager);
                }
            }
        }
        
void Player::addEffect(const ActiveEffect& newEffect) {
    for (auto& activeEffect : m_activeEffects) {
        if (activeEffect.effectId == newEffect.effectId) {
            float initialDuration = newEffect.maxDuration;
            activeEffect.maxDuration += (initialDuration / 2.0f);
            activeEffect.duration += initialDuration;
            if (activeEffect.duration > activeEffect.maxDuration) {
                activeEffect.duration = activeEffect.maxDuration;
            }
            activeEffect.justAppliedThisTurn = true;
            return;
        }
    }
    
    ActiveEffect effectToAdd = newEffect;
    effectToAdd.justAppliedThisTurn = true;
    m_activeEffects.push_back(effectToAdd);
}
float Player::getManaGainMultiplier() const {
    return m_manaGainMultiplier;
}

void Player::setManaGainMultiplier(float multiplier) {
    m_manaGainMultiplier = multiplier;
}
        
void Player::updateEffects(float speedCost) {
    // Reset temporary modifiers before recalculating
    m_manaGainMultiplier = 1.0f;

    for (int i = static_cast<int>(m_activeEffects.size()) - 1; i >= 0; --i) {
        auto& effect = m_activeEffects[i];
        
        if (effect.justAppliedThisTurn) {
            effect.justAppliedThisTurn = false;
            // continue; // Don't continue, apply buffs immediately
        }

        effect.duration -= speedCost;

        if (effect.duration <= 0) {
            m_activeEffects.erase(m_activeEffects.begin() + i);
            continue; // Skip to next effect as this one is gone
        }

        // Apply the effect's stat modifier
        if (effect.modifier == "mana_gain") {
            m_manaGainMultiplier = effect.value;
        }
    }
}

const std::vector<ActiveEffect>& Player::getActiveEffects() const {
    return m_activeEffects;
}

float Player::getStatModifier(const std::string& modifier) const {
    if (modifier == "skull_damage") {
        for (const auto& effect : m_activeEffects) {
            if (effect.modifier == modifier) {
                return effect.value; // Return the first multiplier found
            }
        }
        return 1.0f; // Default multiplier is 1.0 (no change)
    } else {
        float total = 0.0f;
        for (const auto& effect : m_activeEffects) {
            if (effect.modifier == modifier) {
                total += effect.value; // Sum additive bonuses like vigor
            }
        }
        return total; // Default additive bonus is 0.0
        }
    }
    
    