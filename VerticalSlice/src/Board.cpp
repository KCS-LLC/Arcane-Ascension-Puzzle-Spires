#include "PCH.h"
#include "Board.h"
#include "Game.h" // For gemTextures
#include <random>
#include <iostream>

Board::Board(int width, int height, GemFactory& factory)
    : m_width(width), m_height(height), m_gemFactory(factory) {
    m_grid.resize(m_height);
    for (int r = 0; r < m_height; ++r) {
        m_grid[r].resize(m_width);
    }
}

void Board::initialize(const std::vector<GemSubType>& possibleGems) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, possibleGems.size() - 1);

    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width; ++c) {
            GemSubType type;
            do {
                type = possibleGems[distrib(gen)];
                m_grid[r][c] = m_gemFactory.createGem(type, gemTextures.at(type));
            } while (findMatches().count({r, c})); // Ensure no matches on creation
             if (m_grid[r][c]) {
                m_grid[r][c]->setPosition(c * 64, r * 64); // Assuming 64x64 gems
            }
        }
    }
}

void Board::initializeForPowerTrial() {
    // Simplified for now - will need the robust logic later
    std::vector<GemSubType> trialGems = { GemSubType::Fire, GemSubType::Skull };
     std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, trialGems.size() - 1);

    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width; ++c) {
            GemSubType type = trialGems[(r + c) % 2]; // Checkerboard for now
            m_grid[r][c] = m_gemFactory.createGem(type, gemTextures.at(type));
            if (m_grid[r][c]) {
                m_grid[r][c]->setPosition(c * 64, r * 64);
            }
        }
    }
}

void Board::render(sf::RenderWindow& window) {
    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width; ++c) {
            if (m_grid[r][c]) {
                m_grid[r][c]->render(window);
            }
        }
    }
}

BaseGem* Board::getGemAt(int r, int c) {
    if (r >= 0 && r < m_height && c >= 0 && c < m_width) {
        return m_grid[r][c].get();
    }
    return nullptr;
}

bool Board::canSwap(int r1, int c1, int r2, int c2) {
    // Basic adjacency check
    return (std::abs(r1 - r2) == 1 && c1 == c2) || (std::abs(c1 - c2) == 1 && r1 == r2);
}

void Board::swapGems(int r1, int c1, int r2, int c2) {
    m_grid[r1][c1].swap(m_grid[r2][c2]);
    if (m_grid[r1][c1]) {
        m_grid[r1][c1]->setPosition(c1 * 64, r1 * 64);
    }
    if (m_grid[r2][c2]) {
        m_grid[r2][c2]->setPosition(c2 * 64, r2 * 64);
    }
}

std::set<std::pair<int, int>> Board::findMatches() {
    std::set<std::pair<int, int>> matches;
    // Horizontal matches
    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width - 2; ++c) {
            BaseGem* gem1 = getGemAt(r, c);
            BaseGem* gem2 = getGemAt(r, c + 1);
            BaseGem* gem3 = getGemAt(r, c + 2);
            if (gem1 && gem2 && gem3 && gem1->getSubType() == gem2->getSubType() && gem2->getSubType() == gem3->getSubType()) {
                matches.insert({r, c});
                matches.insert({r, c + 1});
                matches.insert({r, c + 2});
            }
        }
    }
    // Vertical matches
    for (int c = 0; c < m_width; ++c) {
        for (int r = 0; r < m_height - 2; ++r) {
             BaseGem* gem1 = getGemAt(r, c);
             BaseGem* gem2 = getGemAt(r + 1, c);
             BaseGem* gem3 = getGemAt(r + 2, c);
             if (gem1 && gem2 && gem3 && gem1->getSubType() == gem2->getSubType() && gem2->getSubType() == gem3->getSubType()) {
                matches.insert({r, c});
                matches.insert({r + 1, c});
                matches.insert({r + 2, c});
            }
        }
    }
    return matches;
}

void Board::removeGems(const std::set<std::pair<int, int>>& matches) {
    for (const auto& pos : matches) {
        m_grid[pos.first][pos.second].reset(); // unique_ptr reset() deletes the object
    }
}

std::vector<Board::FallInfo> Board::applyGravity() {
    std::vector<FallInfo> fallInfo;
    for (int c = 0; c < m_width; ++c) {
        int emptyRow = -1;
        for (int r = m_height - 1; r >= 0; --r) {
            if (!m_grid[r][c] && emptyRow == -1) {
                emptyRow = r;
            }
            if (m_grid[r][c] && emptyRow != -1) {
                m_grid[emptyRow][c] = std::move(m_grid[r][c]);
                fallInfo.push_back({r, c, emptyRow});
                m_grid[emptyRow][c]->setPosition(c * 64, emptyRow * 64);
                emptyRow--;
            }
        }
    }
    return fallInfo;
}

void Board::refill(const std::vector<GemSubType>& possibleGems) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, possibleGems.size() - 1);

    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width; ++c) {
            if (!m_grid[r][c]) {
                GemSubType type = possibleGems[distrib(gen)];
                m_grid[r][c] = m_gemFactory.createGem(type, gemTextures.at(type));
                 if (m_grid[r][c]) {
                    m_grid[r][c]->setPosition(c * 64, r * 64);
                }
            }
        }
    }
}
