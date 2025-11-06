#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <vector>
#include <string>
#include "Player.h" // For Spell struct

class DataManager {
public:
    DataManager();

    bool loadSpells(const std::string& path);
    bool loadMonsterData(const std::string& path);

    const std::vector<Spell>& getAllSpells() const;
    int getMonsterHP() const;
    int getMonsterSpeed() const;
    int getMonsterAttackDamage() const;

private:
    std::vector<Spell> spells;
    int monsterHP;
    int monsterSpeed;
    int monsterAttackDamage;
};

#endif // DATAMANAGER_H
