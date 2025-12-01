#pragma once
#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <vector>
#include <string>
#include <map>
#include <SFML/Graphics/Font.hpp>
#include "Structs.h"
#include "Judgement.h"

struct AnimationTimings {
    int swap_duration_ms = 200;
    int destroy_duration_ms = 300;
    int fall_duration_ms = 300;
    int rotate_duration_ms = 300;
};

class DataManager {
public:
    DataManager();

    // Loading functions
    bool loadPrimaryGemTypes(const std::string& path);
    bool loadSecondaryGemTypes(const std::string& path);
    bool loadGemCatalog(const std::string& path);
    bool loadAttunements(const std::string& path);
    bool loadSpells(const std::string& path);
    bool loadAllMonsters(const std::string& path);
    bool loadFloor(const std::string& path);
    bool loadConfig(const std::string& path);

    // Getters
    const GemCatalogEntry* getGemCatalogEntry(GemSubType subType) const;
    const std::map<GemSubType, GemCatalogEntry>& getGemCatalog() const;
    const SecondaryGemTypeData* getSecondaryGemTypeData(int secondaryTypeId) const;
    PrimaryGemType getPrimaryGemType(GemSubType subType) const;
    const std::vector<Attunement>& getAttunements() const;
    const Attunement* getAttunementById(const std::string& id) const;
    const std::vector<Spell>& getAllSpells() const;
    const Spell* getSpellById(const std::string& id) const;
    const MonsterData* getRandomMonsterByRank(int rank, bool isBoss = false) const;
    const Floor& getFloor() const;
    const Room* getRoomById(int roomId) const;
    const std::vector<JudgementTrial>& getJudgementTrials() const;
    const sf::Font& getFont() const;
    const AnimationTimings& getAnimationTimings() const;

private:
    bool loadJudgementTrials();

    // Data Maps
    std::map<PrimaryGemType, std::string> m_primaryGemTypes;
    std::map<int, SecondaryGemTypeData> m_secondaryGemTypes;
    std::map<GemSubType, GemCatalogEntry> m_gemCatalog;

    // Other Data Members
    std::vector<Attunement> attunements;
    std::vector<Spell> spells;
    std::vector<MonsterData> m_allMonsters;
    Floor currentFloor;
    std::vector<JudgementTrial> m_judgementTrials;
    sf::Font m_font;
    AnimationTimings m_animationTimings;
};


#endif // DATAMANAGER_H
