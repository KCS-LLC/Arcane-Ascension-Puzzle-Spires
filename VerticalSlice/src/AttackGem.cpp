#include "PCH.h"
#include "AttackGem.h"
#include "Player.h"
#include "Monster.h"

float AttackGem::onMatch(Board& board, Player& player, Monster& monster) {
    float baseDamage = 1.0f + (player.getVigor() / 10.0f); // Every 10 vigor adds 1 base damage
    float multiplier = player.getStatModifier("skull_damage");
    return baseDamage * multiplier; // Return the damage as a float
}