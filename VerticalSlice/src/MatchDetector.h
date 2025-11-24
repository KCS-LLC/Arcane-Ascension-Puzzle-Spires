#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>

// Forward declaration
class Board;

class MatchDetector {
public:
    std::vector<std::vector<sf::Vector2i>> findAllMatches(const Board& board) const;
};
