#pragma once

#include <vector>
#include <memory>
#include <set>
#include "BaseGem.h"
#include "GemFactory.h"

// Forward declarations
class Player; 
class Monster;

class Board {
public:
    struct FallInfo {
        int row;
        int col;
        int fallToRow;
    };

    Board(int width, int height, GemFactory& factory);

    void initialize(const std::vector<GemSubType>& possibleGems);
    void initializeForPowerTrial();

    void render(sf::RenderWindow& window, const sf::Vector2f& boardOrigin, bool isAnimatingSwap, const std::pair<sf::Vector2i, sf::Vector2i>& animatingGems, bool isAnimatingDestruction, const std::set<std::pair<int, int>>& destroyingGems, bool isAnimatingRefill, const std::vector<Board::FallInfo>& fallInfo);
    BaseGem* getGemAt(int r, int c);

    // Gameplay logic
    bool canSwap(int r1, int c1, int r2, int c2);
    void swapGems(int r1, int c1, int r2, int c2);
    std::set<std::pair<int, int>> findMatches();
    void removeGems(const std::set<std::pair<int, int>>& matches);
    
    std::vector<FallInfo> applyGravity();
    void refill(const std::vector<GemSubType>& possibleGems);


private:
    int m_width;
    int m_height;
    GemFactory& m_gemFactory;
    std::vector<std::vector<std::unique_ptr<BaseGem>>> m_grid;
};