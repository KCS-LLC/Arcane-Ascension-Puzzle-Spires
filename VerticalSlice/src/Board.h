#ifndef BOARD_H
#define BOARD_H

#include "Gem.h" // Include the new Gem struct definition

// Forward declaration for Player class
class Player;

class Board {
public:
    struct FallInfo {
        Gem gem;
        int startRow, endRow, col;

        FallInfo(const Gem& g, int sr, int er, int c) : gem(g), startRow(sr), endRow(er), col(c) {}
    };

    Board();
    void initialize(const Player& player);
    void initialize(const std::vector<std::vector<Gem>>& layout, const Player& player);
    void initializeForPowerTrial();

    const Gem& getGem(int r, int c) const;
    bool canSwap(int r1, int c1, int r2, int c2);
    void swapGems(int r1, int c1, int r2, int c2);

    std::set<std::pair<int, int>> findMatches();
    void processMatches(const std::set<std::pair<int, int>>& matches, std::vector<Gem>& matchedGems);
    std::vector<FallInfo> applyGravityAndRefill(const Player& player);
    std::vector<FallInfo> applyGravityAndRefill(const std::vector<GemSubType>& availableGemSubTypes);
    std::vector<std::pair<sf::Vector2i, sf::Vector2i>> findAllValidSwaps() const;

private:
    std::vector<std::vector<Gem>> m_grid;
    void fillBoard(const Player& player);
    bool hasMatches();
    Gem getRandomGem(const Player& player);
};

#endif // BOARD_H
