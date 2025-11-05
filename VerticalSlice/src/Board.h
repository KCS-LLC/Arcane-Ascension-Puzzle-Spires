
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Player.h" // We need GemType

const int BOARD_WIDTH = 8;
const int BOARD_HEIGHT = 8;

struct Gem {
    GemType type;
    // In the full game, this would also store Aura rank, etc.
};

class Board {
public:
    Board();

    void initialize(); // Fills the board with random gems

    // Core gameplay methods
    bool canSwap(int r1, int c1, int r2, int c2);
    void swapGems(int r1, int c1, int r2, int c2);
    
    // Match detection and resolution
    bool findAndProcessMatches();

    // Getters
    const Gem& getGem(int r, int c) const;

private:
    std::vector<std::vector<Gem>> grid;

    void fillBoard();
    void refillBoard();
    bool checkForMatches();
    int processMatches(); // Returns number of gems cleared
    
    std::vector<std::pair<int, int>> findMatchesInRow(int r);
    std::vector<std::pair<int, int>> findMatchesInCol(int c);
};

#endif // BOARD_H
