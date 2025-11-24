#include "PCH.h"
#include "TreasureGem.h"
#include "Board.h"
#include "Player.h"
#include "Monster.h"

TreasureGem::TreasureGem(const GemCatalogEntry* catalogEntry, const sf::Texture& texture)
    : BaseGem(catalogEntry, texture) {
}

void TreasureGem::onMatch(Board& board, Player& player, Monster& monster) {
    // Merging logic is handled centrally in Game::processTreasureMerges()
    // This function can be used for animations or sound effects later.
}
