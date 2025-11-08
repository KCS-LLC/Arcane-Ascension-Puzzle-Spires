
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <set>
#include <utility>
#include <SFML/System/Vector2.hpp>
#include "Player.h" // We need GemType
#include "Constants.h"

struct Gem {
    GemType type;
    // In the full game, this would also store Aura rank, etc.
};

class Board {
public:
    Board();

    void initialize(const Player& player); // Fills the board with random gems

    // Core gameplay methods
    bool canSwap(int r1, int c1, int r2, int c2);
    void swapGems(int r1, int c1, int r2, int c2);
    
    // Match detection and resolution
    std::set<std::pair<int, int>> findMatches();
    void processMatches(const std::set<std::pair<int, int>>& matches);

    struct FallInfo {
        int col;
        int startRow;
        int endRow;
        GemType type;
    };
    std::vector<FallInfo> applyGravityAndRefill(const Player& player);
    std::vector<std::pair<sf::Vector2i, sf::Vector2i>> findAllValidSwaps();

    // Getters
    const Gem& getGem(int r, int c) const;

private:
    std::vector<std::vector<Gem>> grid;

    void fillBoard(const Player& player);
    bool hasMatches();
    GemType getRandomGemType(const Player& player);
};

#endif // BOARD_H
