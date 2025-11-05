
#include "Board.h"
#include <random>
#include <vector>
#include <set>
#include <ctime>

// Helper function to get a random gem type
GemType getRandomGemType() {
    // For the prototype, we'll have a simple distribution.
    // 1/5 chance for Skull, 2/5 for Fire, 2/5 for Water.
    int randValue = rand() % 5;
    if (randValue == 0) {
        return GemType::Skull;
    } else if (randValue <= 2) {
        return GemType::Fire;
    } else {
        return GemType::Water;
    }
}

Board::Board() {
    grid.resize(BOARD_HEIGHT, std::vector<Gem>(BOARD_WIDTH));
    srand(time(0)); // Seed the random number generator
}

void Board::initialize() {
    fillBoard();

    // Ensure the starting board has no matches
    while (checkForMatches()) {
        fillBoard();
    }
}

void Board::fillBoard() {
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            grid[r][c] = { getRandomGemType() };
        }
    }
}

const Gem& Board::getGem(int r, int c) const {
    return grid[r][c];
}

// A simple check to see if a swap is valid (adjacent and within bounds)
bool Board::canSwap(int r1, int c1, int r2, int c2) {
    if (r1 < 0 || r1 >= BOARD_HEIGHT || c1 < 0 || c1 >= BOARD_WIDTH ||
        r2 < 0 || r2 >= BOARD_HEIGHT || c2 < 0 || c2 >= BOARD_WIDTH) {
        return false;
    }
    int dr = abs(r1 - r2);
    int dc = abs(c1 - c2);
    return (dr == 1 && dc == 0) || (dr == 0 && dc == 1);
}

void Board::swapGems(int r1, int c1, int r2, int c2) {
    std::swap(grid[r1][c1], grid[r2][c2]);
}

// This is the main public function to handle the match-3 cascade logic
bool Board::findAndProcessMatches() {
    if (!checkForMatches()) {
        return false;
    }

    while (checkForMatches()) {
        processMatches();
        refillBoard();
    }
    return true;
}

// Private helper to just check if any matches exist
bool Board::checkForMatches() {
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            if (c + 2 < BOARD_WIDTH && grid[r][c].type == grid[r][c+1].type && grid[r][c].type == grid[r][c+2].type) {
                return true;
            }
            if (r + 2 < BOARD_HEIGHT && grid[r][c].type == grid[r+1][c].type && grid[r][c].type == grid[r+2][c].type) {
                return true;
            }
        }
    }
    return false;
}

// Private helper that finds all matches, marks them for deletion, and returns the count
int Board::processMatches() {
    std::set<std::pair<int, int>> matchedGems;

    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            // Horizontal match check
            if (c + 2 < BOARD_WIDTH && grid[r][c].type == grid[r][c+1].type && grid[r][c].type == grid[r][c+2].type) {
                matchedGems.insert({r, c});
                matchedGems.insert({r, c+1});
                matchedGems.insert({r, c+2});
            }
            // Vertical match check
            if (r + 2 < BOARD_HEIGHT && grid[r][c].type == grid[r+1][c].type && grid[r][c].type == grid[r+2][c].type) {
                matchedGems.insert({r, c});
                matchedGems.insert({r+1, c});
                matchedGems.insert({r+2, c});
            }
        }
    }

    for (const auto& pos : matchedGems) {
        // For now, we'll just mark them as skulls to be replaced.
        // A more robust system would use a "ToBeCleared" type.
        grid[pos.first][pos.second].type = GemType::Skull; 
    }

    return matchedGems.size();
}

// Private helper to make gems "fall down" and fill empty spaces
void Board::refillBoard() {
    for (int c = 0; c < BOARD_WIDTH; ++c) {
        int emptyRow = BOARD_HEIGHT - 1;
        for (int r = BOARD_HEIGHT - 1; r >= 0; --r) {
            // In our simple system, "Skull" is the cleared type.
            // This is a placeholder for a proper "empty" state.
            if (grid[r][c].type != GemType::Skull) {
                std::swap(grid[emptyRow][c], grid[r][c]);
                emptyRow--;
            }
        }
        // Fill the remaining empty spaces at the top
        for (int r = emptyRow; r >= 0; --r) {
            grid[r][c] = { getRandomGemType() };
        }
    }
}
