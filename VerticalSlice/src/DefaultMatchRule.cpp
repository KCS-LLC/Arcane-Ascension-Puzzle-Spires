#include "PCH.h"
#include "DefaultMatchRule.h"
#include "Board.h"

bool DefaultMatchRule::appliesTo(const std::vector<sf::Vector2i>& match, const Board& board, const DataManager& dataManager) const {
    // This is the fallback rule, so it applies to any match that isn't handled by a more specific rule.
    return true;
}

std::unique_ptr<MatchResolution> DefaultMatchRule::execute(const std::vector<sf::Vector2i>& match, Board& board, GemFactory& gemFactory, const DataManager& dataManager, const std::map<GemSubType, sf::Texture>& gemTextures) const {
    auto resolution = std::make_unique<MatchResolution>();
    resolution->gemsToRemove = match; // Mark all matched gems for removal
    resolution->gemsToPlace.clear(); // No new gems are created by this default rule
    return resolution;
}
