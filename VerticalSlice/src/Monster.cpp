#include "PCH.h"
#include "Monster.h"
#include "Structs.h"

Monster::Monster(const MonsterData& data) 
    : maxHp(data.hp), 
      currentHp(data.hp), 
      speed(data.speed), 
      actionCounter(0),
      name(data.name),
      manaAffinities(data.manaAffinities)
{}

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

const std::vector<GemSubType>& Monster::getManaAffinities() const {
    return manaAffinities;
}
