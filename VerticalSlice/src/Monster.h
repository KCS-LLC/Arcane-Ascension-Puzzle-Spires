#ifndef MONSTER_H
#define MONSTER_H

#include "Structs.h" // For MonsterData

class Monster {
public:
    Monster(const MonsterData& data);

    void takeDamage(int damage);
    bool isTurnReady(int playerActionCost);

    int getCurrentHp() const;
    int getMaxHp() const;
    int getSpeed() const;
    int getActionCounter() const;
    const std::vector<GemSubType>& getManaAffinities() const;
    std::string name;
    void reset();

private:
    int maxHp;
    int currentHp;
    int speed;
    int actionCounter;
    std::vector<GemSubType> manaAffinities;
};

#endif // MONSTER_H
