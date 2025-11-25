#include "PCH.h"
#include "Player.h"
#include "DataManager.h"
#include "Structs.h"

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
        if (spell) {
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

void Player::finalizeJudgement(const JudgementResults& results, const DataManager& dataManager) {
    m_attunementId = determineAttunement(results, dataManager);
    const Attunement* finalAttunement = dataManager.getAttunementById(m_attunementId);

    if (finalAttunement) {
        setAttunement(*finalAttunement, dataManager);
    } else {
        std::cerr << "Could not find final attunement with id: " << m_attunementId << std::endl;
        const Attunement* fallback = dataManager.getAttunementById("adept");
        if (fallback) {
            setAttunement(*fallback, dataManager);
        }
    }
}