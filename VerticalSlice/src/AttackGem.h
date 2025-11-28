#pragma once

#include "BaseGem.h"

class AttackGem : public BaseGem {
public:
    AttackGem(const GemCatalogEntry* catalogEntry, const sf::Texture& texture)
        : BaseGem(catalogEntry, texture) {}

    float onMatch(Board& board, Player& player, Monster& monster) override;
};