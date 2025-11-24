#pragma once

#include "IMatchRule.h"
#include <vector>
#include "PCH.h"
#include "Structs.h"

class TreasureMergeRule : public IMatchRule {
public:
    bool appliesTo(const std::vector<sf::Vector2i>& match, const Board& board, const DataManager& dataManager) const override;
    std::unique_ptr<MatchResolution> execute(const std::vector<sf::Vector2i>& match, Board& board, GemFactory& gemFactory, const DataManager& dataManager, const std::map<GemSubType, sf::Texture>& gemTextures) const override;
};
