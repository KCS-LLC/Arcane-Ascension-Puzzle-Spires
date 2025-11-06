
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <optional>
#include "Board.h"
#include "Player.h"
#include "Monster.h"
#include <map>

// New Window Dimensions
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int TILE_SIZE = 64; // Size of each gem sprite in pixels

enum class GameState { Playing, Animating, GameOver };

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

    // Game State
    GameState currentState;
    sf::Vector2i boardOrigin; // Top-left corner of the board in pixels

    // Input
    std::optional<sf::Vector2i> selectedTile;
    std::optional<sf::Vector2i> dragStartTile;

    // Animation
    sf::Clock animationClock;
    std::pair<sf::Vector2i, sf::Vector2i> animatingGems;
    bool isAnimatingSwap;
    bool isAnimatingDestruction;
    std::set<std::pair<int, int>> destroyingGems;
    bool isAnimatingRefill;
    std::vector<Board::FallInfo> fallInfo;
    bool isReshuffling;

    // UI Elements
    sf::Font font;
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
