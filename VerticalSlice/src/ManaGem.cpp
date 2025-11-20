#include "PCH.h"
#include "ManaGem.h"
#include "Player.h"

void ManaGem::onMatch(Board& board, Player& player, Monster& monster) {
    player.addMana(getSubType(), 10);
}