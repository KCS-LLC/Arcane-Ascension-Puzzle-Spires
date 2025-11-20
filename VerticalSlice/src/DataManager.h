#include "SpireData.h"
#include "Structs.h"
#include "Gem.h"
#include "Judgement.h"

// Forward declarations

class DataManager {
public:
    DataManager();

    bool loadAttunements(const std::string& path);
    bool loadSpells(const std::string& path);
    bool loadMonsterData(const std::string& path);
    bool loadFloor(const std::string& path);

    const std::vector<Attunement>& getAttunements() const;
    const std::vector<Spell>& getAllSpells() const;
    const Spell* getSpellById(int id) const;
    int getMonsterHP() const;
    int getMonsterSpeed() const;
    int getMonsterAttackDamage() const;
    std::string getMonsterName() const;
    const Floor& getFloor() const;
    const Room* getRoomById(int roomId) const;
    const std::vector<JudgementTrial>& getJudgementTrials() const;

private:
    std::vector<Attunement> attunements;
    std::vector<Spell> spells;
    int monsterHP;
    int monsterSpeed;
    int monsterAttackDamage;
    std::string monsterName;
    Floor currentFloor;
    std::vector<JudgementTrial> m_judgementTrials;
    void loadJudgementTrials();
};
