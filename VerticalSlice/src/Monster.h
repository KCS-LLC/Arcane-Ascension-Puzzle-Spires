#ifndef MONSTER_H
#define MONSTER_H

#include "Structs.h" // For MonsterData
#include <vector>
#include <string>
#include <map>

class Monster {
public:
    Monster(const MonsterData& data);

    void takeDamage(int damage);
    bool isTurnReady(int playerActionCost);
    void reset();

    // Getters for stats from MonsterData
    int getCurrentHp() const { return m_currentHp; }
    int getMaxHp() const { return m_data.stats.hp; }
    int getSpeed() const { return m_data.stats.speed; }
    int getVigor() const { return m_data.stats.vigor; }
    int getWit() const { return m_data.stats.wit; }
    int getAttackDamage() const { return m_data.stats.attackDamage; }
    std::string getName() const { return m_data.name; }
    const std::vector<GemSubType>& getManaAffinities() const { return m_data.manaAffinities; }
    const std::vector<std::string>& getAbilities() const { return m_data.abilities; }
    const std::map<std::string, float>& getResistances() const { return m_data.resistances; }
    const std::map<std::string, float>& getVulnerabilities() const { return m_data.vulnerabilities; }

    // Getter for runtime action counter
    int getActionCounter() const { return m_actionCounter; }

private:
    MonsterData m_data; // All static monster data
    int m_currentHp;    // Current HP (runtime stat)
    int m_actionCounter; // Current action counter (runtime stat)
};

#endif // MONSTER_H
