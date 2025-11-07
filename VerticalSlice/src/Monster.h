
#ifndef MONSTER_H
#define MONSTER_H

#include <string>

class Monster {
public:
    Monster(int maxHp, int speed);

    void takeDamage(int damage);
    bool isTurnReady(int playerActionCost);

    int getCurrentHp() const;
    int getMaxHp() const;
    int getSpeed() const;
    int getActionCounter() const;
    std::string name;

private:
    int maxHp;
    int currentHp;
    int speed;
    int actionCounter;
};

#endif // MONSTER_H
