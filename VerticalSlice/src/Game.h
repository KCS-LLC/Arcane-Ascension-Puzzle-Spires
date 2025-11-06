
#ifndef GAME_H
#define GAME_H

#include "Constants.h"
#include "Board.h"
#include "Player.h"
#include "Monster.h"
#include "DataManager.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <map>

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
    void setupText();
    void handleSwap(sf::Vector2i tile1, sf::Vector2i tile2);
    void applyMatchConsequences(const std::map<GemType, int>& matchResults);

    sf::RenderWindow window;
    Board board;
    Player player;
    Monster monster;
    DataManager dataManager;

    GameState currentState;
    sf::Clock animationClock;

    bool isAnimatingSwap;
    bool isAnimatingDestruction;
    bool isAnimatingRefill;
    bool isReshuffling;
    std::pair<sf::Vector2i, sf::Vector2i> animatingGems;
    std::set<std::pair<int, int>> destroyingGems;
    std::vector<Board::FallInfo> fallInfo;

    std::optional<sf::Vector2i> selectedTile;
    std::optional<sf::Vector2i> dragStartTile;

    sf::Font font;
    sf::Vector2f boardOrigin;
    sf::Text playerHpText;
    sf::Text monsterHpText;
    sf::Text manaText;
    sf::Text gameOverText;
    std::vector<sf::RectangleShape> spellButtons;
    std::vector<sf::Text> spellButtonTexts;
    sf::RectangleShape leftPanel;
    sf::RectangleShape rightPanel;
    sf::RectangleShape boardFrame;
    sf::RectangleShape monsterSpeedGaugeBackground;
    sf::RectangleShape monsterSpeedGaugeForeground;
};

#endif // GAME_H
