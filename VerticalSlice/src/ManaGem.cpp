#include "ManaGem.h"
#include "Player.h" // To add mana

void ManaGem::onMatch(Board& board, Player& player, Monster& monster) {
    // Monster parameter is unused here, but required by the interface
    player.addMana(getSubType(), 10);
}
