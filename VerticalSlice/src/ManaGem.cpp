#include "PCH.h"
#include "ManaGem.h"
#include "Player.h"

float ManaGem::onMatch(Board& board, Player& player, Monster& monster) {
    player.addMana(getSubType(), 10);
    return 0.0f;
}