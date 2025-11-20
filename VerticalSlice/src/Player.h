#ifndef PLAYER_H
#define PLAYER_H

#include "GemSubType.h"
#include "Structs.h"
#include "Judgement.h"

// Forward declarations
class DataManager;

class Player {
public:
    Player(int initialHp, const std::vector<Spell>& initialSpells);

    void setAttunement(const Attunement& attunement, const DataManager& dataManager);

    int getCurrentHp() const;
    int getMaxHp() const { return maxHp; }
    int getMana(GemSubType type) const;
    int getMaxMana() const { return maxMana; }
    const std::vector<Spell>& getSpells() const;
    const std::vector<GemSubType>& getManaTypes() const;
    const std::string& getAttunementId() const;

    void takeDamage(int amount);
    void addMana(GemSubType type, int amount);
    int castSpell(int spellIndex);

    void finalizeJudgement(const JudgementResults& results, const DataManager& dataManager);

private:
    void setStartingStats(int tactical_score, int mana_affinity_score);
    std::string determineAttunement(const JudgementResults& results, const DataManager& dataManager) const;

    int currentHp;
    int maxHp;
    int maxMana;
    std::map<GemSubType, int> mana;
    std::vector<Spell> spells;
    std::vector<GemSubType> availableManaTypes;
    std::string m_attunementId;
};

#endif // PLAYER_H
