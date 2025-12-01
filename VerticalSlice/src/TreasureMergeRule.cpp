#include "PCH.h"
#include "TreasureMergeRule.h"
#include "Board.h"
#include "BaseGem.h"
#include "GemSubType.h"
#include "PrimaryGemType.h"
#include "GemFactory.h"
#include "DataManager.h"
#include <algorithm> // For std::sort

// Custom comparator for sorting gem coordinates (bottom-most, then left-most)
bool compareCoords(const sf::Vector2i& a, const sf::Vector2i& b) {
    if (a.x > b.x) { return true; } // Higher row index (further down) comes first
    if (a.x < b.x) { return false; }
    return a.y < b.y; // Lower column index (further left) comes first
}

bool TreasureMergeRule::appliesTo(const std::vector<sf::Vector2i>& match, const Board& board, const DataManager& dataManager) const {
    if (match.empty()) {
        return false;
    }

    // First, get the subtype of the first gem to use as the reference for the whole match.
    const BaseGem* firstGem = board.getGemAt(match[0].x, match[0].y);
    if (firstGem == nullptr) { return false; }
    GemSubType referenceSubType = firstGem->getSubType();

    // Now, iterate through ALL gems in the match, including the first one.
    for (const auto& coord : match) {
        const BaseGem* gem = board.getGemAt(coord.x, coord.y);
        if (gem == nullptr) { return false; } // A gem in the match group must exist.

        // Check 1: Ensure all gems in the match have the same subtype.
        if (gem->getSubType() != referenceSubType) {
            return false;
        }

        // Check 2: Ensure the gem is actually a Treasure gem.
        const GemCatalogEntry* catalogEntry = gem->getCatalogEntry();
        if (catalogEntry == nullptr) { return false; }
        
        const SecondaryGemTypeData* secondaryData = dataManager.getSecondaryGemTypeData(catalogEntry->secondaryTypeId);
        if (secondaryData == nullptr || secondaryData->primaryType != PrimaryGemType::Treasure) {
            return false;
        }
    }

    // If we get here, all gems in the match are treasure gems of the same subtype.
    return true;
}

std::unique_ptr<MatchResolution> TreasureMergeRule::execute(const std::vector<sf::Vector2i>& match, Board& board, GemFactory& gemFactory, const DataManager& dataManager, const std::map<GemSubType, sf::Texture>& gemTextures) const {
    auto resolution = std::make_unique<MatchResolution>();
    if (match.empty()) { return resolution; }

    // Sort coordinates to get a predictable gem to upgrade (bottom-most, then left-most)
    std::vector<sf::Vector2i> sortedMatch = match;
    std::sort(sortedMatch.begin(), sortedMatch.end(), compareCoords);

    const BaseGem* firstGem = board.getGemAt(sortedMatch[0].x, sortedMatch[0].y);
    if (firstGem == nullptr) { return resolution; }

    GemSubType currentSubType = firstGem->getSubType();
    GemSubType nextSubType;

    // Determine the next tier of treasure
    switch (currentSubType) {
        case GemSubType::Coin:          nextSubType = GemSubType::CoinPile;       break;
        case GemSubType::CoinPile:      nextSubType = GemSubType::CoinBag;        break;
        case GemSubType::CoinBag:       nextSubType = GemSubType::CoinBagBundle;  break;

        case GemSubType::TreasureChest: nextSubType = GemSubType::TreasureChest;  break; // Already max tier
        default: return resolution; // Not a treasure type we can upgrade
    }
    
    // If we're already at the max tier, don't do anything special. Let the default rule handle it.
    if (currentSubType == nextSubType) {
        resolution->gemsToRemove = sortedMatch;
        return resolution;
    }

    // Determine how many gems to transform based on match size
    int gemsToTransformCount = 0;
    if (sortedMatch.size() >= 5) {
        gemsToTransformCount = 3;
    } else if (sortedMatch.size() == 4) {
        gemsToTransformCount = 2;
    } else {
        gemsToTransformCount = 1;
    }

    // Populate the resolution struct
    for (int i = 0; i < sortedMatch.size(); ++i) {
        if (i < gemsToTransformCount) {
            // These gems will be transformed
            resolution->gemsToTransform.emplace_back(sortedMatch[i], nextSubType);
        } else {
            // The rest will be removed
            resolution->gemsToRemove.push_back(sortedMatch[i]);
        }
    }

    return resolution;
}