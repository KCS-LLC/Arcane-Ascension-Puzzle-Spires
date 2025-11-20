#include "PCH.h"
#include "Board.h"
#include "Player.h"
#include <random>

Board::Board() {
    m_grid.resize(BOARD_HEIGHT, std::vector<Gem>(BOARD_WIDTH));
}

void Board::initialize(const Player& player) {
    fillBoard(player);
    while (hasMatches() || findAllValidSwaps().empty()) {
        fillBoard(player);
    }
}

void Board::initialize(const std::vector<std::vector<Gem>>& layout, const Player& player) {
    std::cout << "Initializing board with layout..." << std::endl;
    m_grid = layout;

    // Keep regenerating the board until no matches are present.
    while (hasMatches() || findAllValidSwaps().empty()) {
        fillBoard(player);
    }
}

void Board::initializeForPowerTrial() {
    std::cout << "Initializing board for power trial..." << std::endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dist(0.5); // 50% chance for Skull

    const int max_retries = 10000;
    for (int i = 0; i < max_retries; ++i) {
        for (int r = 0; r < BOARD_HEIGHT; ++r) {
            for (int c = 0; c < BOARD_WIDTH; ++c) {
                GemSubType type = dist(gen) ? GemSubType::Skull : GemSubType::Fire;
                m_grid[r][c] = Gem(type, 1);
            }
        }
        if (!hasMatches() && !findAllValidSwaps().empty()) {
            return; // Found a valid board
        }
    }

    // Fallback to a guaranteed valid pattern if random generation fails
    std::cout << "Random generation failed, falling back to checkerboard." << std::endl;
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            if ((r + c) % 2 == 0) {
                m_grid[r][c] = Gem(GemSubType::Fire, 1);
            } else {
                m_grid[r][c] = Gem(GemSubType::Skull, 1);
            }
        }
    }
}

void Board::fillBoard(const Player& player) {
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            m_grid[r][c] = getRandomGem(player);
        }
    }
}

const Gem& Board::getGem(int r, int c) const {
    return m_grid[r][c];
}

bool Board::canSwap(int r1, int c1, int r2, int c2) {
    if (r1 < 0 || r1 >= BOARD_HEIGHT || c1 < 0 || c1 >= BOARD_WIDTH ||
        r2 < 0 || r2 >= BOARD_HEIGHT || c2 < 0 || c2 >= BOARD_WIDTH) {
        return false;
    }
    return abs(r1 - r2) + abs(c1 - c2) == 1;
}

void Board::swapGems(int r1, int c1, int r2, int c2) {
    std::swap(m_grid[r1][c1], m_grid[r2][c2]);
}

std::set<std::pair<int, int>> Board::findMatches() {
    std::set<std::pair<int, int>> matches;
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            if (m_grid[r][c].primaryType == PrimaryGemType::Empty) continue;
            if (c > 1 && m_grid[r][c].subType == m_grid[r][c - 1].subType && m_grid[r][c].subType == m_grid[r][c - 2].subType) {
                matches.insert({r, c}); matches.insert({r, c - 1}); matches.insert({r, c - 2});
            }
            if (r > 1 && m_grid[r][c].subType == m_grid[r - 1][c].subType && m_grid[r][c].subType == m_grid[r - 2][c].subType) {
                matches.insert({r, c}); matches.insert({r - 1, c}); matches.insert({r - 2, c});
            }
        }
    }
    return matches;
}

void Board::processMatches(const std::set<std::pair<int, int>>& matches, std::vector<Gem>& matchedGems) {
    for (const auto& pos : matches) {
        matchedGems.push_back(m_grid[pos.first][pos.second]);
        m_grid[pos.first][pos.second] = Gem(GemSubType::Generic, 0);
    }
}

std::vector<Board::FallInfo> Board::applyGravityAndRefill(const Player& player) {
    return applyGravityAndRefill(player.getManaTypes());
}

std::vector<Board::FallInfo> Board::applyGravityAndRefill(const std::vector<GemSubType>& availableGemSubTypes) {
    std::vector<FallInfo> fallInfo;
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int c = 0; c < BOARD_WIDTH; ++c) {
        int emptyRow = BOARD_HEIGHT - 1;
        for (int r = BOARD_HEIGHT - 1; r >= 0; --r) {
            if (m_grid[r][c].primaryType != PrimaryGemType::Empty) {
                if (r != emptyRow) {
                    m_grid[emptyRow][c] = m_grid[r][c];
                    fallInfo.emplace_back(m_grid[r][c], r, emptyRow, c);
                    m_grid[r][c] = Gem(GemSubType::Generic, 0); 
                }
                emptyRow--;
            }
        }

        for (int r = emptyRow; r >= 0; --r) {
            GemSubType newSubType = GemSubType::Generic;
            if (!availableGemSubTypes.empty()) {
                std::uniform_int_distribution<> distrib(0, availableGemSubTypes.size() - 1);
                newSubType = availableGemSubTypes[distrib(gen)];
            } else {
                std::uniform_int_distribution<> distrib(1, 6);
                newSubType = static_cast<GemSubType>(distrib(gen));
            }
            m_grid[r][c] = Gem(newSubType, 1);
            fallInfo.emplace_back(m_grid[r][c], - (emptyRow - r + 1), r, c);
        }
    }
    return fallInfo;
}

std::vector<std::pair<sf::Vector2i, sf::Vector2i>> Board::findAllValidSwaps() const {
    std::vector<std::pair<sf::Vector2i, sf::Vector2i>> validSwaps;
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            // Swap right
            if (c + 1 < BOARD_WIDTH) {
                Board temp = *this;
                temp.swapGems(r, c, r, c + 1);
                if (temp.hasMatches()) validSwaps.push_back({{c, r}, {c + 1, r}});
            }
            // Swap down
            if (r + 1 < BOARD_HEIGHT) {
                Board temp = *this;
                temp.swapGems(r, c, r + 1, c);
                if (temp.hasMatches()) validSwaps.push_back({{c, r}, {c, r + 1}});
            }
        }
    }
    return validSwaps;
}

bool Board::hasMatches() {
    return findMatches().size() > 0;
}

Gem Board::getRandomGem(const Player& player) {
    std::random_device rd;
    std::mt19937 gen(rd());
    double skullChance = 0.1; 
    std::bernoulli_distribution skull_dist(skullChance);

    if (skull_dist(gen)) {
        return Gem(GemSubType::Skull, 1);
    } else {
        const auto& manaTypes = player.getManaTypes();
        if (manaTypes.empty()) {
             // Fallback for player without attunement
            const std::vector<GemSubType> defaultManaTypes = {
                GemSubType::Fire, GemSubType::Water, GemSubType::Earth,
                GemSubType::Air, GemSubType::Light, GemSubType::Umbral
            };
            std::uniform_int_distribution<> distrib(0, defaultManaTypes.size() - 1);
            return Gem(defaultManaTypes[distrib(gen)], 1);
        }
        std::uniform_int_distribution<> distrib(0, manaTypes.size() - 1);
        return Gem(manaTypes[distrib(gen)], 1);
    }
}