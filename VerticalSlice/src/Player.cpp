#include "Player.h"

Player::Player(int initialHp) : currentHp(initialHp) {
    mana[GemType::Fire] = 0;
    mana[GemType::Water] = 0;
    mana[GemType::Earth] = 0;
    mana[GemType::Light] = 0;

    // Add four basic spells
    spells.push_back({"Fireball", GemType::Fire, 10, 25});
    spells.push_back({"Ice Lance", GemType::Water, 10, 25});
    spells.push_back({"Rock Throw", GemType::Earth, 10, 25});
    spells.push_back({"Smite", GemType::Light, 10, 25});
}

int Player::getCurrentHp() const { return currentHp; }
int Player::getMana(GemType type) const { return mana.at(type); }
const std::vector<Spell>& Player::getSpells() const { return spells; }

void Player::takeDamage(int amount) { currentHp -= amount; }

void Player::addMana(GemType type, int amount) {
    if (type != GemType::Skull && type != GemType::Empty) {
        mana[type] += amount;
    }
}

int Player::castSpell(int spellIndex) {
    if (spellIndex < 0 || spellIndex >= spells.size()) {
        return 0; // Invalid index
    }
    const Spell& spell = spells[spellIndex];
    if (mana[spell.costType] >= spell.costAmount) {
        mana[spell.costType] -= spell.costAmount;
        return spell.damage;
    }
    return 0; // Not enough mana
}
