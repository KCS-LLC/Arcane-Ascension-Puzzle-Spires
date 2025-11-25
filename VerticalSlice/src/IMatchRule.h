#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <map>
#include "GemSubType.h" 

// Forward declaration
class BaseGem;
class Board;
class GemFactory;
class DataManager;

struct MatchResolution {
    std::vector<sf::Vector2i> gemsToRemove;
    std::vector<std::pair<sf::Vector2i, GemSubType>> gemsToTransform;
};

class IMatchRule {
public:
    virtual ~IMatchRule() = default;
    virtual bool appliesTo(const std::vector<sf::Vector2i>& match, const class Board& board, const class DataManager& dataManager) const = 0;
    virtual std::unique_ptr<MatchResolution> execute(const std::vector<sf::Vector2i>& match, class Board& board, class GemFactory& gemFactory, const class DataManager& dataManager, const std::map<GemSubType, sf::Texture>& gemTextures) const = 0;
};
