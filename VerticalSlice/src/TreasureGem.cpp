#include "TreasureGem.h"
#include "Player.h"

void TreasureGem::onMatch(Board& board, Player& player, Monster& monster) {
    // Monster parameter is unused here
    player.addScore(1);
}
