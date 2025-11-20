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
    bool loadGemDefinitions(const std::string& path);

    const std::vector<Attunement>& getAttunements() const;
    const Attunement* getAttunementById(const std::string& id) const;
    const std::vector<Spell>& getAllSpells() const;
    const Spell* getSpellById(int id) const;
    int getMonsterHP() const;
    int getMonsterSpeed() const;
    int getMonsterAttackDamage() const;
    std::string getMonsterName() const;
    const Floor& getFloor() const;
    const Room* getRoomById(int roomId) const;
    const std::vector<JudgementTrial>& getJudgementTrials() const;
    const std::map<GemSubType, GemDefinition>& getGemDefinitions() const;

private:
    bool loadJudgementTrials(); // Declare this here
    std::vector<Attunement> attunements;
    std::vector<Spell> spells;
    int monsterHP;
    int monsterSpeed;
    int monsterAttackDamage;
    std::string monsterName;
    Floor currentFloor; // Add this missing member
    std::vector<JudgementTrial> m_judgementTrials; // Add this missing member

    std::map<GemSubType, GemDefinition> m_gemDefinitions;
};
