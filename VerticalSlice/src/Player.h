#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "GemSubType.h"
#include "Structs.h"
#include "Judgement.h"

// Forward declarations
class DataManager;

class Player {
public:
    Player(int initialHp, const std::vector<Spell>& initialSpells);

    void setAttunement(const Attunement& attunement, const DataManager& dataManager);

    int getHp() const;
    int getMaxHp() const;
    int getMana(GemSubType type) const;
    int getMaxMana() const;
    GemSubType getPrimaryManaType() const;
    const std::vector<Spell>& getSpells() const;
    const std::vector<GemSubType>& getManaTypes() const;
    const std::string& getAttunementId() const;

    void takeDamage(int amount);
    void heal(int amount);
    void addScore(int amount);
    void addMana(GemSubType type, int amount);
    const Spell* castSpell(int spellIndex);

    void finalizeJudgement(const JudgementResults& results, const DataManager& dataManager);

private:
    void setStartingStats(int tactical_score, int mana_affinity_score);
    std::string determineAttunement(const JudgementResults& results, const DataManager& dataManager) const;

    int m_hp;
    int m_maxHp;
    int m_maxMana;
    std::vector<GemSubType> m_availableManaTypes; // Renamed to use m_ prefix
    std::map<GemSubType, int> mana;
    std::vector<Spell> spells;
    std::string m_attunementId;
    int m_score = 0;
};