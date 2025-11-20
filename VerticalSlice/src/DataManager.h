#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "Structs.h"
#include "Judgement.h"
#include <SFML/Graphics/Font.hpp>

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
    const sf::Font& getFont() const;


private:
    bool loadJudgementTrials();

    std::vector<Attunement> attunements;
    std::vector<Spell> spells;
    int monsterHP;
    int monsterSpeed;
    int monsterAttackDamage;
    std::string monsterName;
    Floor currentFloor;
    std::vector<JudgementTrial> m_judgementTrials;
    sf::Font m_font;
};

#endif // DATAMANAGER_H