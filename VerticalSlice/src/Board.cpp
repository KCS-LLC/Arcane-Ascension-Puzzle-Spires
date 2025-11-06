
#include "Board.h"
#include <random>
#include <ctime>

// Helper function to get a random gem type
GemType getRandomGemType() {
    int randValue = rand() % 5;
    switch (randValue) {
        case 0: return GemType::Skull;
        case 1: return GemType::Fire;
        case 2: return GemType::Water;
        case 3: return GemType::Earth;
        case 4: return GemType::Light;
        default: return GemType::Skull;
    }
}

Board::Board() {
    grid.resize(BOARD_HEIGHT, std::vector<Gem>(BOARD_WIDTH));
    srand(time(0));
}

void Board::initialize() {
    do {
        fillBoard();
    } while (hasMatches() || findAllValidSwaps().empty());
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

std::set<std::pair<int, int>> Board::findMatches() {
    std::set<std::pair<int, int>> matchedGems;
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            // Horizontal matches
            if (c + 2 < BOARD_WIDTH && grid[r][c].type != GemType::Empty && grid[r][c].type == grid[r][c+1].type && grid[r][c].type == grid[r][c+2].type) {
                matchedGems.insert({r, c});
                matchedGems.insert({r, c+1});
                matchedGems.insert({r, c+2});
            }
            // Vertical matches
            if (r + 2 < BOARD_HEIGHT && grid[r][c].type != GemType::Empty && grid[r][c].type == grid[r+1][c].type && grid[r][c].type == grid[r+2][c].type) {
                matchedGems.insert({r, c});
                matchedGems.insert({r+1, c});
                matchedGems.insert({r+2, c});
            }
        }
    }
    return matchedGems;
}

void Board::processMatches(const std::set<std::pair<int, int>>& matches) {
    for (const auto& pos : matches) {
        grid[pos.first][pos.second].type = GemType::Empty;
    }
}

std::vector<Board::FallInfo> Board::applyGravityAndRefill() {
    std::vector<FallInfo> fallInfos;
    for (int c = 0; c < BOARD_WIDTH; ++c) {
        int emptyRow = BOARD_HEIGHT - 1;
        for (int r = BOARD_HEIGHT - 1; r >= 0; --r) {
            if (grid[r][c].type != GemType::Empty) {
                if (emptyRow != r) {
                    fallInfos.push_back({c, r, emptyRow, grid[r][c].type});
                    grid[emptyRow][c] = grid[r][c];
                    grid[r][c].type = GemType::Empty;
                }
                emptyRow--;
            }
        }
        
        for (int r = emptyRow; r >= 0; --r) {
            grid[r][c] = { getRandomGemType() };
            fallInfos.push_back({c, r - (emptyRow + 1), r, grid[r][c].type});
        }
    }
    return fallInfos;
}

std::vector<std::pair<sf::Vector2i, sf::Vector2i>> Board::findAllValidSwaps() {
    std::vector<std::pair<sf::Vector2i, sf::Vector2i>> validSwaps;

    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            // Test swap with right neighbor
            if (c + 1 < BOARD_WIDTH) {
                swapGems(r, c, r, c + 1);
                if (hasMatches()) {
                    validSwaps.push_back({sf::Vector2i(c, r), sf::Vector2i(c + 1, r)});
                }
                swapGems(r, c, r, c + 1); // Swap back
            }
            // Test swap with bottom neighbor
            if (r + 1 < BOARD_HEIGHT) {
                swapGems(r, c, r + 1, c);
                if (hasMatches()) {
                    validSwaps.push_back({sf::Vector2i(c, r), sf::Vector2i(c, r + 1)});
                }
                swapGems(r, c, r + 1, c); // Swap back
            }
        }
    }
    return validSwaps;
}

bool Board::hasMatches() {

    return !findMatches().empty();
}
