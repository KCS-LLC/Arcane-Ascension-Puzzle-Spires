#include "PCH.h"
#include "Board.h"
#include "Player.h" // Still needed for the full Player definition

Board::Board() {
    grid.resize(BOARD_HEIGHT, std::vector<Gem>(BOARD_WIDTH));
    srand(time(0));
}

void Board::initialize(const Player& player) {
    do {
        fillBoard(player);
    } while (hasMatches() || findAllValidSwaps().empty());
}

void Board::initialize(const std::vector<std::vector<Gem>>& layout) {
    if (layout.size() != BOARD_HEIGHT || (BOARD_HEIGHT > 0 && layout[0].size() != BOARD_WIDTH)) {
        std::cerr << "Error: Provided board layout has incorrect dimensions." << std::endl;
        // Fallback to random initialization or handle error appropriately
        // For now, let's just resize and then it will crash or be wrong if dimensions are off
        grid.resize(BOARD_HEIGHT, std::vector<Gem>(BOARD_WIDTH));
    } else {
        grid = layout;
    }
}

void Board::fillBoard(const Player& player) {
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            grid[r][c] = getRandomGem(player);
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
            if (grid[r][c].primaryType == PrimaryGemType::Empty) continue;
            
            auto isMatch = [&](const Gem& g1, const Gem& g2) {
                return g1.primaryType == g2.primaryType && g1.subType == g2.subType;
            };

            // Horizontal matches
            if (c + 2 < BOARD_WIDTH && isMatch(grid[r][c], grid[r][c+1]) && isMatch(grid[r][c], grid[r][c+2])) {
                matchedGems.insert({r, c});
                matchedGems.insert({r, c+1});
                matchedGems.insert({r, c+2});
            }
            // Vertical matches
            if (r + 2 < BOARD_HEIGHT && isMatch(grid[r][c], grid[r+1][c]) && isMatch(grid[r][c], grid[r+2][c])) {
                matchedGems.insert({r, c});
                matchedGems.insert({r+1, c});
                matchedGems.insert({r+2, c});
            }
        }
    }
    return matchedGems;
}

void Board::processMatches(const std::set<std::pair<int, int>>& matches, std::vector<Gem>& matchedGems) {
    matchedGems.clear();
    for (const auto& pos : matches) {
        matchedGems.push_back(grid[pos.first][pos.second]);
        grid[pos.first][pos.second] = Gem(); // Use default constructor for empty
    }
}

std::vector<Board::FallInfo> Board::applyGravityAndRefill(const Player& player) {
    std::vector<FallInfo> fallInfos;
    for (int c = 0; c < BOARD_WIDTH; ++c) {
        int emptyRow = BOARD_HEIGHT - 1;
        for (int r = BOARD_HEIGHT - 1; r >= 0; --r) {
            if (grid[r][c].primaryType != PrimaryGemType::Empty) {
                if (emptyRow != r) {
                    fallInfos.push_back({c, r, emptyRow, grid[r][c]});
                    grid[emptyRow][c] = grid[r][c];
                    grid[r][c] = Gem();
                }
                emptyRow--;
            }
        }
        
        for (int r = emptyRow; r >= 0; --r) {
            grid[r][c] = getRandomGem(player);
            fallInfos.push_back({c, r - (emptyRow + 1), r, grid[r][c]});
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

Gem Board::getRandomGem(const Player& player) {
    const auto& manaSubTypes = player.getManaTypes();
    
    std::vector<Gem> possibleGems;

    if (manaSubTypes.empty()) {
        // Fallback for player with no attunement selected yet
        possibleGems.push_back(Gem(GemSubType::Skull, 1));
        possibleGems.push_back(Gem(GemSubType::Coin, 1));
        possibleGems.push_back(Gem(GemSubType::Fire, 1));
        possibleGems.push_back(Gem(GemSubType::Water, 1));
    } else {
        // Create a pool of possible gems: Player's mana types + Skull + Coin
        for (const auto& subType : manaSubTypes) {
            possibleGems.push_back(Gem(subType, 1));
        }
        possibleGems.push_back(Gem(GemSubType::Skull, 1));
        possibleGems.push_back(Gem(GemSubType::Coin, 1));
    }
    
    return possibleGems[rand() % possibleGems.size()];
}
