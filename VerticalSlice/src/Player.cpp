#include "PCH.h"
#include "Player.h"
#include "DataManager.h"
#include "Structs.h"
#include "StringUtils.h"

Player::Player(int initialHp, const std::vector<Spell*>& initialSpells)
    : m_hp(initialHp), m_maxHp(initialHp), m_maxMana(100), spells(initialSpells) {
    for (int i = static_cast<int>(GemSubType::Fire); i <= static_cast<int>(GemSubType::Grey); ++i) {
        mana[static_cast<GemSubType>(i)] = 0;
    }
}

void Player::setAttunement(const Attunement& attunement, const DataManager& dataManager) {
    spells.clear();
    for (const std::string& spellId : attunement.spellIds) {
        const Spell* spell = dataManager.getSpellById(spellId);
        if (spell != nullptr) {
            spells.push_back(const_cast<Spell*>(spell));
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

const std::vector<Spell*>& Player::getSpells() const { return spells; }

GemSubType Player::getPrimaryManaType() const {
    if (!m_availableManaTypes.empty()) {
        return m_availableManaTypes[0];
    }
    return GemSubType::Empty;
}

void Player::takeDamage(int amount) {
    m_hp -= amount;
    m_hp = std::max(0, m_hp);
}

void Player::heal(int amount) {
    m_hp += amount;
    m_hp = std::min(m_maxHp, m_hp);
}

void Player::addScore(int amount) {
    m_score += amount;
}

void Player::addMana(GemSubType type, int amount) {
    auto it = mana.find(type);
    if (it != mana.end()) {
        it->second += amount;
        it->second = std::min(it->second, m_maxMana);
        std::cout << "[MANA GAIN] Gained " << amount << " " << gemSubtypeToString(type) << " mana." << '\n';
    }
}

void Player::spendMana(GemSubType type, int amount) {
    auto it = mana.find(type);
    if (it != mana.end()) {
        it->second -= amount;
        it->second = std::max(0, it->second);
    }
}

const Spell* Player::castSpell(int spellIndex) {
    if (spellIndex < 0 || spellIndex >= spells.size()) {
        return nullptr;
    }
    const Spell* spell = spells[spellIndex];
    if (getMana(spell->costType) >= spell->manaCost) {
        spendMana(spell->costType, spell->manaCost);
        return spell;
    }
    return nullptr;
}

void Player::setStartingStats(int tactical_score, int mana_affinity_score) {
    m_maxHp = 100 + (tactical_score / 10);
    m_maxMana = 100 + (mana_affinity_score / 5);
    m_hp = m_maxHp;
}

std::string Player::determineAttunement(const JudgementResults& results, const DataManager& dataManager) const {
    if (results.powerScore >= results.hasteScore && results.powerScore >= results.controlScore) {
        return "executioner";
    }
    if (results.hasteScore >= results.powerScore && results.hasteScore >= results.controlScore) {
        return "elementalist";
    }
    return "guardian";
}

const std::string& Player::getAttunementId() const {
    return m_attunementId;
}

int Player::getVigor() const {
    return m_vigor + static_cast<int>(getStatModifier("vigor"));
}

float Player::getManaGainMultiplier() const {
    return m_manaGainMultiplier;
}

void Player::setManaGainMultiplier(float multiplier) {
    m_manaGainMultiplier = multiplier;
}

void Player::addEffect(const ActiveEffect& newEffect) {
    for (auto& activeEffect : m_activeEffects) {
        if (activeEffect.effectId == newEffect.effectId) {
            float initialDuration = newEffect.maxDuration;
            activeEffect.maxDuration += (initialDuration / 2.0f);
            activeEffect.duration += initialDuration;
            activeEffect.duration = std::min(activeEffect.duration, activeEffect.maxDuration);
            activeEffect.justAppliedThisTurn = true;
            return;
        }
    }
    
    ActiveEffect effectToAdd = newEffect;
    effectToAdd.justAppliedThisTurn = true;
    m_activeEffects.push_back(effectToAdd);
}

void Player::updateEffects(float speedCost) {
    m_manaGainMultiplier = 1.0f; // Reset temporary modifiers
    for (int i = static_cast<int>(m_activeEffects.size()) - 1; i >= 0; --i) {
        auto& effect = m_activeEffects[i];
        if (effect.justAppliedThisTurn) {
            effect.justAppliedThisTurn = false;
        }
        effect.duration -= speedCost;
        if (effect.duration <= 0) {
            m_activeEffects.erase(m_activeEffects.begin() + i);
            continue;
        }
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
                return effect.value;
            }
        }
        return 1.0f;
    }
    float total = 0.0f;
    for (const auto& effect : m_activeEffects) {
        if (effect.modifier == modifier) {
            total += effect.value;
        }
    }
    return total;
}