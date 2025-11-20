#include "PCH.h"
#include "TreasureGem.h"
#include "Player.h"

void TreasureGem::onMatch(Board& board, Player& player, Monster& monster) {
    player.addScore(1);
}