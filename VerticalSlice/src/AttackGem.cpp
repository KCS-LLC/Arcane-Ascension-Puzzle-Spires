#include "PCH.h"
#include "AttackGem.h"
#include "Player.h"
#include "Monster.h"

float AttackGem::onMatch(Board& board, Player& player, Monster& monster) {
    float baseDamage = 1.0f + (player.getVigor() / 10.0f); // Every 10 vigor adds 1 base damage
    float multiplier = player.getStatModifier("skull_damage");
    float damage = (baseDamage + getLevel() - 1) * multiplier;
    std::cout << "[ATTACK GEM] Matched Skull Gem (Level " << getLevel() << ") dealt " << damage << " damage." << std::endl;
    return damage; // Add level bonus
}