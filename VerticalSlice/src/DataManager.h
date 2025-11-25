#pragma once
#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <vector>
#include <string>
#include <map>
#include <SFML/Graphics/Font.hpp>
#include "Structs.h"
#include "Judgement.h"

class DataManager {
public:
    DataManager();

    // Loading functions
    bool loadPrimaryGemTypes(const std::string& path);
    bool loadSecondaryGemTypes(const std::string& path);
    bool loadGemCatalog(const std::string& path);
    bool loadAttunements(const std::string& path);
    bool loadSpells(const std::string& path);
    bool loadMonsterData(const std::string& path);
    bool loadFloor(const std::string& path);

    // Getters
    const GemCatalogEntry* getGemCatalogEntry(GemSubType subType) const;
    const std::map<GemSubType, GemCatalogEntry>& getGemCatalog() const;
    const SecondaryGemTypeData* getSecondaryGemTypeData(int secondaryTypeId) const;
    PrimaryGemType getPrimaryGemType(GemSubType subType) const;
    const std::vector<Attunement>& getAttunements() const;
    const Attunement* getAttunementById(const std::string& id) const;
    const std::vector<Spell>& getAllSpells() const;
    const Spell* getSpellById(const std::string& id) const;
    const MonsterData& getMonsterData() const;
    int getMonsterHP() const;
    int getMonsterSpeed() const;
    int getMonsterAttackDamage() const;
    std::string getMonsterName() const;
    const std::vector<GemSubType>& getMonsterManaAffinities() const;
    const Floor& getFloor() const;
    const Room* getRoomById(int roomId) const;
    const std::vector<JudgementTrial>& getJudgementTrials() const;
    const sf::Font& getFont() const;

private:
    bool loadJudgementTrials();

    // Data Maps
    std::map<PrimaryGemType, std::string> m_primaryGemTypes;
    std::map<int, SecondaryGemTypeData> m_secondaryGemTypes;
    std::map<GemSubType, GemCatalogEntry> m_gemCatalog;

    // Other Data Members
    std::vector<Attunement> attunements;
    std::vector<Spell> spells;
    MonsterData m_monsterData;
    Floor currentFloor;
    std::vector<JudgementTrial> m_judgementTrials;
    sf::Font m_font;
};

#endif // DATAMANAGER_H
