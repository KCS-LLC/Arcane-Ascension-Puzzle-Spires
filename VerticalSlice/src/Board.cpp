#include "PCH.h"
#include "Board.h"
#include "Game.h" // For gemTextures
#include <random>
#include <iostream>
#include <algorithm>
#include <vector>

Board::Board(int width, int height, GemFactory& factory)
    : m_width(width), m_height(height), m_gemFactory(factory) {
    m_grid.resize(m_height);
    for (int r = 0; r < m_height; ++r) {
        m_grid[r].resize(m_width);
    }
}

void Board::initialize(const std::vector<GemSubType>& gemTypes) {
    if (gemTypes.empty()) {
        std::cerr << "Board::initialize - Error: gemTypes vector is empty." << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, gemTypes.size() - 1);

    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width; ++c) {
            GemSubType type;
            do {
                type = gemTypes[distrib(gen)];
            } while ((c >= 2 && getGemAt(r, c - 1)->getSubType() == type && getGemAt(r, c - 2)->getSubType() == type) ||
                     (r >= 2 && getGemAt(r - 1, c)->getSubType() == type && getGemAt(r - 2, c)->getSubType() == type));
            m_grid[r][c] = m_gemFactory.createGem(type, gemTextures.at(type));
        }
    }
}

void Board::initializeForPowerTrial() {
    std::vector<GemSubType> allowedGems = {
        GemSubType::Fire, GemSubType::Umbral, GemSubType::Light, GemSubType::Skull
    };
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, allowedGems.size() - 1);

    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width; ++c) {
            GemSubType type;
            do {
                type = allowedGems[distrib(gen)];
            } while ((c >= 2 && getGemAt(r, c - 1)->getSubType() == type && getGemAt(r, c - 2)->getSubType() == type) ||
                     (r >= 2 && getGemAt(r - 1, c)->getSubType() == type && getGemAt(r - 2, c)->getSubType() == type));
            m_grid[r][c] = m_gemFactory.createGem(type, gemTextures.at(type));
        }
    }
}





void Board::render(sf::RenderWindow& window, const sf::Vector2f& boardOrigin, bool isAnimatingSwap, const std::pair<sf::Vector2i, sf::Vector2i>& animatingGems, bool isAnimatingDestruction, const std::set<sf::Vector2i, Vector2iCompare>& destroyingGems, bool isAnimatingRefill, const std::vector<Board::FallInfo>& fallInfo) {
    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width; ++c) {
            bool shouldDraw = true;

            if (isAnimatingDestruction) {
                if (destroyingGems.count({r, c})) {
                    shouldDraw = false;
                }
            }
            
            if (isAnimatingSwap && ((r == animatingGems.first.y && c == animatingGems.first.x) || (r == animatingGems.second.y && c == animatingGems.second.x))) {
                shouldDraw = false;
            }

            if (isAnimatingRefill) {
                for (const auto& info : fallInfo) {
                    if (info.col == c && info.fallToRow == r) {
                        shouldDraw = false;
                        break;
                    }
                }
            }

            if (m_grid[r][c] && shouldDraw) {
                sf::Sprite sprite = m_grid[r][c]->getSprite();
                sprite.setPosition(sf::Vector2f(boardOrigin.x + c * TILE_SIZE, boardOrigin.y + r * TILE_SIZE));
                window.draw(sprite);
            }
        }
    }
}

BaseGem* Board::getGemAt(int r, int c) const {
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
        m_grid[r1][c1]->setPosition(c1 * TILE_SIZE, r1 * TILE_SIZE);
    }
    if (m_grid[r2][c2]) {
        m_grid[r2][c2]->setPosition(c2 * TILE_SIZE, r2 * TILE_SIZE);
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

void Board::removeGems(const std::set<sf::Vector2i, Vector2iCompare>& matches) {
    for (const auto& pos : matches) {
        m_grid[pos.x][pos.y].reset(); // unique_ptr reset() deletes the object
    }
}

std::vector<Board::FallInfo> Board::applyGravityAndRefill(const std::vector<GemSubType>& possibleGems) {
    std::vector<FallInfo> fallInfo;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, possibleGems.size() - 1);

    // Gravity for existing gems
    for (int c = 0; c < m_width; ++c) {
        int emptyRow = -1;
        for (int r = m_height - 1; r >= 0; --r) {
            if (!m_grid[r][c] && emptyRow == -1) {
                emptyRow = r;
            }
            if (m_grid[r][c] && emptyRow != -1) {
                m_grid[emptyRow][c] = std::move(m_grid[r][c]);
                fallInfo.push_back({r, c, emptyRow, m_grid[emptyRow][c]->getSubType()});
                m_grid[emptyRow][c]->setPosition(c * TILE_SIZE, emptyRow * TILE_SIZE);
                emptyRow--;
            }
        }
    }

    // Refill new gems
    for (int c = 0; c < m_width; ++c) {
        int newGems = 0;
        for (int r = m_height - 1; r >= 0; --r) {
            if (!m_grid[r][c]) {
                newGems++;
                GemSubType type = possibleGems[distrib(gen)];
                m_grid[r][c] = m_gemFactory.createGem(type, gemTextures.at(type));
                if (m_grid[r][c]) {
                    m_grid[r][c]->setPosition(c * TILE_SIZE, r * TILE_SIZE);
                    fallInfo.push_back({-newGems, c, r, type});
                }
            }
        }
    }

    return fallInfo;
}

void Board::setGemAt(int r, int c, std::unique_ptr<BaseGem> gem) {
    if (isInBounds(r, c)) {
        m_grid[r][c] = std::move(gem);
    }
}

bool Board::isInBounds(int r, int c) const {
    return r >= 0 && r < m_height && c >= 0 && c < m_width;
}

int Board::getWidth() const {
    return m_width;
}

int Board::getHeight() const {
    return m_height;
}
