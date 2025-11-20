#include "PCH.h"
#include "AttackGem.h"
#include "Monster.h"

void AttackGem::onMatch(Board& board, Player& player, Monster& monster) {
    monster.takeDamage(1);
}