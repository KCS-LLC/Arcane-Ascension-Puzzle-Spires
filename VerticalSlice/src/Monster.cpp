#include "Monster.h"

Monster::Monster(int maxHp, int speed) 
    : maxHp(maxHp), currentHp(maxHp), speed(speed), actionCounter(0) {}

void Monster::takeDamage(int damage) {
    currentHp -= damage;
    if (currentHp < 0) {
        currentHp = 0;
    }
}

bool Monster::isTurnReady(int speedCost) {
    actionCounter += speedCost;
    if (actionCounter >= speed) {
        actionCounter -= speed;
        return true;
    }
    return false;
}

void Monster::reset() {
    currentHp = maxHp;
    actionCounter = 0;
}

int Monster::getCurrentHp() const { 
    return currentHp; 
}

int Monster::getMaxHp() const { 
    return maxHp; 
}

int Monster::getSpeed() const { 
    return speed; 
}

int Monster::getActionCounter() const { 
    return actionCounter; 
}
