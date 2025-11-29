#include "PCH.h"
#include "ManaGem.h"
#include "Player.h"

float ManaGem::onMatch(Board& board, Player& player, Monster& monster) {
    int manaGained = static_cast<int>(10 * player.getManaGainMultiplier());
    player.addMana(getSubType(), manaGained);
    return 0.0f;
}