#pragma once

#include "BaseGem.h"

class TreasureGem : public BaseGem {
public:
    TreasureGem(const GemCatalogEntry* catalogEntry, const sf::Texture& texture);

    float onMatch(Board& board, Player& player, Monster& monster) override;
};
