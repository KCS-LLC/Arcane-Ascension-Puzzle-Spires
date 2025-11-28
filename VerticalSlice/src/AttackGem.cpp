#include "PCH.h"
#include "AttackGem.h"
#include "Player.h"
#include "Monster.h"

void AttackGem::onMatch(Board& board, Player& player, Monster& monster) {
    float baseDamage = 1.0f + (player.getVigor() / 10.0f); // Every 10 vigor adds 1 base damage
    float multiplier = player.getStatModifier("skull_damage");
    int finalDamage = static_cast<int>(baseDamage * multiplier);

    monster.takeDamage(finalDamage);

    std::cout << "[COMBAT] Skull Damage Dealt! Vigor: " << player.getVigor() 
              << ", Base Damage: " << baseDamage 
              << ", Multiplier: " << multiplier 
              << ", Final Damage: " << finalDamage 
              << ". Monster HP after: " << monster.getCurrentHp() << std::endl;
}