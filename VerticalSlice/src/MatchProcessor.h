#pragma once

#include <vector>
#include <optional>
#include <memory>
#include "IMatchRule.h"
#include "PCH.h"
#include "Structs.h"

class Board; // Forward declaration

class MatchProcessor {
public:
    MatchProcessor();
    std::optional<std::unique_ptr<MatchResolution>> process(const std::vector<sf::Vector2i>& match, Board& board, GemFactory& gemFactory, const DataManager& dataManager, const std::map<GemSubType, sf::Texture>& gemTextures) const;

private:
    std::vector<std::unique_ptr<IMatchRule>> m_rules;
};