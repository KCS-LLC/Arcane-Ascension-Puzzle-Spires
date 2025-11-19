#ifndef BOARD_H
#define BOARD_H

#include "Gem.h" // Include the new Gem struct definition

// Forward declaration for Player class
class Player;

class Board {
public:
    Board();

    void initialize(const Player& player); // Fills the board with random gems

    // Core gameplay methods
    bool canSwap(int r1, int c1, int r2, int c2);
    void swapGems(int r1, int c1, int r2, int c2);
    
    // Match detection and resolution
    std::set<std::pair<int, int>> findMatches();
    void processMatches(const std::set<std::pair<int, int>>& matches, std::vector<Gem>& matchedGems);

    struct FallInfo {
        int col;
        int startRow;
        int endRow;
        Gem gem;
    };
    std::vector<FallInfo> applyGravityAndRefill(const Player& player);
    std::vector<std::pair<sf::Vector2i, sf::Vector2i>> findAllValidSwaps();

    // Getters
    const Gem& getGem(int r, int c) const;

private:
    std::vector<std::vector<Gem>> grid;

    void fillBoard(const Player& player);
    bool hasMatches();
    Gem getRandomGem(const Player& player);
};

#endif // BOARD_H
