#include "PCH.h"
#include "Monster.h"
#include "Structs.h"

Monster::Monster(const MonsterData& data) 
    : m_data(data), 
      m_currentHp(data.stats.hp), 
      m_actionCounter(0)
{}

void Monster::takeDamage(int damage) {
    m_currentHp -= damage;
    if (m_currentHp < 0) {
        m_currentHp = 0;
    }
}

bool Monster::isTurnReady(int playerActionCost) {
    m_actionCounter += playerActionCost;
    if (m_actionCounter >= m_data.stats.speed) {
        m_actionCounter -= m_data.stats.speed;
        return true;
    }
    return false;
}

void Monster::reset() {
    m_currentHp = m_data.stats.hp;
    m_actionCounter = 0;
}
