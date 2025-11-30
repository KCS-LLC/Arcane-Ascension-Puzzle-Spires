#include "PCH.h"
#include "Board.h"
#include "Game.h" // For gemTextures
#include "StringUtils.h"
#include "TimeManager.h" // Include for TimeManager
#include <cstdint> // For std::uint8_t
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

void Board::unloadBoard() {
    m_grid.clear();
    m_grid.resize(m_height);
    for (int r = 0; r < m_height; ++r) {
        m_grid[r].resize(m_width);
    }
}

void Board::render(sf::RenderWindow& window, const sf::Vector2f& boardOrigin, const sf::Font& font, sf::Clock& pulseClock, const TimeManager& timeManager, bool isAnimatingSwap, const std::pair<sf::Vector2i, sf::Vector2i>& animatingGems, bool isAnimatingDestruction, const std::set<sf::Vector2i, Vector2iCompare>& destroyingGems, bool isAnimatingRefill, const std::vector<Board::FallInfo>& fallInfo, const std::map<std::string, sf::Texture>& effectIconTextures, bool isAnimatingRowRotation, int rotatingRow, bool isAnimatingColumnRotation, int rotatingColumn) {
    sf::RectangleShape background(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    sf::Text counterText(font, "", 18);
    counterText.setFillColor(sf::Color::White);

    sf::RectangleShape frame(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    frame.setFillColor(sf::Color::Transparent);
    frame.setOutlineThickness(3.0f);

    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width; ++c) {
            bool shouldDraw = true;

            if (isAnimatingDestruction && destroyingGems.count({r, c})) {
                shouldDraw = false;
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

            // NEW: Rotation animation check
            if (isAnimatingRowRotation && r == rotatingRow) {
                shouldDraw = false;
            }
            if (isAnimatingColumnRotation && c == rotatingColumn) {
                shouldDraw = false;
            }

            if (m_grid[r][c] && shouldDraw) {
                sf::Vector2f tilePosition(boardOrigin.x + c * TILE_SIZE, boardOrigin.y + r * TILE_SIZE);

                // 1. Draw Background Layer
                background.setFillColor(gemLevelToColor(m_grid[r][c]->getLevel()));
                background.setPosition(tilePosition);
                window.draw(background);

                // 2. Draw Gem Layer
                sf::Sprite sprite = m_grid[r][c]->getSprite();
                sprite.setPosition(tilePosition);
                window.draw(sprite);

                // 3. Draw Front Effect Layer
                if (m_grid[r][c]->getStatusEffect() == StatusEffect::Burning) {
                    auto it = effectIconTextures.find("burning_tile_effect");
                    if (it != effectIconTextures.end()) {
                        sf::Sprite effectSprite(it->second);
                        
                        // Scale the icon to fit the tile
                        sf::Vector2u textureSize = it->second.getSize();
                        effectSprite.setScale(sf::Vector2f(static_cast<float>(TILE_SIZE) / textureSize.x, static_cast<float>(TILE_SIZE) / textureSize.y));
                        
                        effectSprite.setPosition(tilePosition);
                        window.draw(effectSprite);

                        // Find the relevant BurningTile_Expire event and draw the duration bar
                        const std::vector<TimeEvent>& events = timeManager.getEvents();
                        for (const auto& event : events) {
                            if (event.type == TimeEventType::BurningTile_Expire && event.coordinates == sf::Vector2i(r, c)) {
                                long long startTime = event.triggerTime - BURNING_TILE_DURATION; // Infer start time
                                long long currentTime = timeManager.getCurrentTime().totalTimeUnits;
                                float durationPercent = 1.0f - static_cast<float>(currentTime - startTime) / BURNING_TILE_DURATION;
                                durationPercent = std::max(0.0f, std::min(1.0f, durationPercent)); // Clamp between 0 and 1

                                if (durationPercent > 0) {
                                    sf::RectangleShape durationBarBack(sf::Vector2f(TILE_SIZE, 4));
                                    durationBarBack.setFillColor(sf::Color(50, 50, 50));
                                    durationBarBack.setPosition(sf::Vector2f(tilePosition.x, tilePosition.y));
                                    window.draw(durationBarBack);

                                    sf::RectangleShape durationBarFront(sf::Vector2f(TILE_SIZE * durationPercent, 4));
                                    durationBarFront.setFillColor(sf::Color::Red);
                                    durationBarFront.setPosition(sf::Vector2f(tilePosition.x, tilePosition.y));
                                    window.draw(durationBarFront);
                                }
                                break;
                            }
                        }
                    }
                }

                // 5. Draw Frame Layer
                if (m_grid[r][c]->getActionState() == ActionState::ValidMoveHint) {
                    float alpha = 128 + 127 * std::sin(pulseClock.getElapsedTime().asSeconds() * 10);
                    sf::Color highlightColor = sf::Color::White;
                    highlightColor.a = static_cast<std::uint8_t>(alpha);
                    frame.setOutlineColor(highlightColor);
                    frame.setPosition(tilePosition);
                    window.draw(frame);
                } else if (m_grid[r][c]->getActionState() == ActionState::Selected) {
                    frame.setOutlineColor(sf::Color::Yellow);
                    frame.setPosition(tilePosition);
                    window.draw(frame);
                }
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

void Board::rotateRow(int rowIndex, int direction) {
    if (rowIndex < 0 || rowIndex >= m_height) return;

    auto& row = m_grid[rowIndex];
    if (direction > 0) { // Rotate right
        std::rotate(row.rbegin(), row.rbegin() + 1, row.rend());
    } else { // Rotate left
        std::rotate(row.begin(), row.begin() + 1, row.end());
    }
}

void Board::rotateColumn(int colIndex, int direction) {
    if (colIndex < 0 || colIndex >= m_width) return;

    // Since grid is row-major, we have to copy to a temp vector
    std::vector<std::unique_ptr<BaseGem>> column;
    for (int i = 0; i < m_height; ++i) {
        column.push_back(std::move(m_grid[i][colIndex]));
    }

    if (direction > 0) { // Rotate down
        std::rotate(column.rbegin(), column.rbegin() + 1, column.rend());
    } else { // Rotate up
        std::rotate(column.begin(), column.begin() + 1, column.end());
    }

    // Move gems back into the grid
    for (int i = 0; i < m_height; ++i) {
        m_grid[i][colIndex] = std::move(column[i]);
    }
}

std::vector<sf::Vector2i> Board::getRandomGemCoords(int count, bool nonAttackGemsOnly, std::optional<GemSubType> specificType) {
    std::vector<sf::Vector2i> filteredCoords;
    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width; ++c) {
            if (m_grid[r][c]) { // Ensure there's a gem at the position
                if (specificType) {
                    if (m_grid[r][c]->getSubType() == *specificType) {
                        filteredCoords.push_back({r, c});
                    }
                } else if (nonAttackGemsOnly) {
                    const GemCatalogEntry* entry = m_grid[r][c]->getCatalogEntry();
                    if (entry && entry->secondaryTypeId != 1001) { // 1001 is Skull
                        filteredCoords.push_back({r, c});
                    }
                } else {
                    filteredCoords.push_back({r, c});
                }
            }
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(filteredCoords.begin(), filteredCoords.end(), g);

    if (filteredCoords.size() > count) {
        filteredCoords.resize(count);
    }

    return filteredCoords;
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

void Board::clearActionStates() {
    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width; ++c) {
            if (m_grid[r][c]) {
                m_grid[r][c]->setActionState(ActionState::None);
            }
        }
    }
}

std::optional<std::pair<sf::Vector2i, sf::Vector2i>> Board::findValidMove() const {
    // This method needs to be mutable for the temporary swap, so we cast away constness.
    Board* mutableThis = const_cast<Board*>(this);

    // Check for horizontal swaps
    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width - 1; ++c) {
            mutableThis->swapGems(r, c, r, c + 1);
            if (mutableThis->findMatches().size() > 0) {
                mutableThis->swapGems(r, c, r, c + 1); // Swap back
                return std::make_pair(sf::Vector2i(c, r), sf::Vector2i(c + 1, r));
            }
            mutableThis->swapGems(r, c, r, c + 1); // Swap back
        }
    }

    // Check for vertical swaps
    for (int r = 0; r < m_height - 1; ++r) {
        for (int c = 0; c < m_width; ++c) {
            mutableThis->swapGems(r, c, r + 1, c);
            if (mutableThis->findMatches().size() > 0) {
                mutableThis->swapGems(r, c, r + 1, c); // Swap back
                return std::make_pair(sf::Vector2i(c, r), sf::Vector2i(c, r + 1));
            }
            mutableThis->swapGems(r, c, r + 1, c); // Swap back
        }
    }

    return std::nullopt; // No valid move found
}

bool Board::isAdjacent(sf::Vector2i pos1, sf::Vector2i pos2) const {
    return (std::abs(pos1.x - pos2.x) == 1 && pos1.y == pos2.y) ||
           (std::abs(pos1.y - pos2.y) == 1 && pos1.x == pos2.x);
}
