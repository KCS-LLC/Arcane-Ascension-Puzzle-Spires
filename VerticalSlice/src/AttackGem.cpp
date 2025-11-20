#include "AttackGem.h"
#include "Monster.h" 

void AttackGem::onMatch(Board& board, Player& player, Monster& monster) {
    // Now we can directly interact with the monster passed to us
    monster.takeDamage(1); 
}
