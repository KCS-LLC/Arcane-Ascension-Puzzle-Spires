
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <optional>
#include "Board.h"
#include "Player.h"
#include "Monster.h"

class Game {
public:
    Game();
    void run(); // This will contain the main game loop

private:
    void processEvents();
    void update();
    void render();

    void handlePlayerInput(sf::Vector2i mousePos);
    void handleSwap(sf::Vector2i tile1, sf::Vector2i tile2);

    sf::RenderWindow window;
    Board board;
    Player player;
    Monster monster;

    // Game state
    enum class GameState { Playing, GameOver };
    GameState currentState;

    // Input handling
    std::optional<sf::Vector2i> selectedTile; // For click-to-select
    std::optional<sf::Vector2i> dragStartTile; // For drag-and-drop

    // Visuals
    sf::Font font;
    sf::Text playerHpText;
    sf::Text monsterHpText;
    sf::Text manaText;
    sf::Text gameOverText;

    void setupText();
};

#endif // GAME_H
