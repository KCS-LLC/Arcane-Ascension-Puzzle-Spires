#include "PCH.h"
#include "TreasureMergeRule.h"
#include "Board.h"
#include "BaseGem.h"
#include "GemSubType.h"
#include "PrimaryGemType.h"
#include "GemFactory.h"
#include "DataManager.h"
#include <algorithm> // For std::sort
#include <iostream>

// Custom comparator for sorting gem coordinates (bottom-most, then left-most)
bool compareCoords(const sf::Vector2i& a, const sf::Vector2i& b) {
    if (a.x > b.x) return true; // Higher row index (further down) comes first
    if (a.x < b.x) return false;
    return a.y < b.y; // Lower column index (further left) comes first
}

bool TreasureMergeRule::appliesTo(const std::vector<sf::Vector2i>& match, const Board& board, const DataManager& dataManager) const {
    if (match.empty()) {
        return false;
    }

    const BaseGem* firstGem = board.getGemAt(match[0].x, match[0].y);
    if (!firstGem) return false;

    const GemCatalogEntry* firstCatalogEntry = firstGem->getCatalogEntry();
    if (!firstCatalogEntry) return false;

    const SecondaryGemTypeData* firstSecondaryData = dataManager.getSecondaryGemTypeData(firstCatalogEntry->secondaryTypeId);
    if (!firstSecondaryData || firstSecondaryData->primaryType != PrimaryGemType::Treasure) {
        return false;
    }

    // Check that all other gems in the match are of the same subtype.
    for (size_t i = 1; i < match.size(); ++i) {
        const BaseGem* gem = board.getGemAt(match[i].x, match[i].y);
        if (!gem || gem->getSubType() != firstGem->getSubType()) {
            return false;
        }
    }

    return true;
}

std::unique_ptr<MatchResolution> TreasureMergeRule::execute(const std::vector<sf::Vector2i>& match, Board& board, GemFactory& gemFactory, const DataManager& dataManager, const std::map<GemSubType, sf::Texture>& gemTextures) const {
    auto resolution = std::make_unique<MatchResolution>();
    if (match.empty()) return resolution;

    // Sort the matched coordinates to handle placement consistently
    std::vector<sf::Vector2i> sortedMatch = match;
    std::sort(sortedMatch.begin(), sortedMatch.end(), compareCoords);

    const BaseGem* firstGem = board.getGemAt(sortedMatch[0].x, sortedMatch[0].y);
    GemSubType currentSubType = firstGem->getSubType();
    GemSubType nextSubType;

    switch (currentSubType) {
        case GemSubType::Coin:          nextSubType = GemSubType::CoinPile;       break;
        case GemSubType::CoinPile:      nextSubType = GemSubType::CoinBag;        break;
        case GemSubType::CoinBag:       nextSubType = GemSubType::CoinBagBundle;  break;
        case GemSubType::CoinBagBundle: nextSubType = GemSubType::TreasureChest;  break;
        case GemSubType::TreasureChest: nextSubType = GemSubType::TreasureChest;  break; // Max tier
        default: return resolution;
    }

    resolution->gemsToRemove = sortedMatch;
    
    // --- Precise Match Size Logic ---
    if (sortedMatch.size() >= 5) {
        resolution->gemsToPlace.emplace_back(sortedMatch[0], gemFactory.createGem(nextSubType, gemTextures.at(nextSubType)));
        resolution->gemsToPlace.emplace_back(sortedMatch[1], gemFactory.createGem(nextSubType, gemTextures.at(nextSubType)));
        resolution->gemsToPlace.emplace_back(sortedMatch[2], gemFactory.createGem(nextSubType, gemTextures.at(nextSubType)));
    } else if (sortedMatch.size() == 4) {
        resolution->gemsToPlace.emplace_back(sortedMatch[0], gemFactory.createGem(nextSubType, gemTextures.at(nextSubType)));
        resolution->gemsToPlace.emplace_back(sortedMatch[1], gemFactory.createGem(nextSubType, gemTextures.at(nextSubType)));
    } else {
        resolution->gemsToPlace.emplace_back(sortedMatch[0], gemFactory.createGem(nextSubType, gemTextures.at(nextSubType)));
    }

    return resolution;
}