#pragma once

#include "BaseGem.h"

class ManaGem : public BaseGem {
public:
    ManaGem(const GemCatalogEntry* catalogEntry, const sf::Texture& texture) 
        : BaseGem(catalogEntry, texture) {}

    void onMatch(Board& board, Player& player, Monster& monster) override;
};
