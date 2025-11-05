
#include "Game.h"
#include <iostream>

const int TILE_SIZE = 64; // Size of each gem sprite in pixels

Game::Game()
    : window(sf::VideoMode({BOARD_WIDTH * TILE_SIZE, BOARD_HEIGHT * TILE_SIZE + 100}), "Arcane Ascension - Prototype"),
      player(100),
      monster(250, 100), // HP, Speed
      currentState(GameState::Playing),
      playerHpText(font),
      monsterHpText(font),
      manaText(font),
      gameOverText(font)
{
    board.initialize();
    // Note: SFML's font loading is relative to the executable's location.
    // Assuming the executable is in the root of VerticalSlice, the path is correct.
    if (!font.openFromFile("assets/OpenSans-Regular.ttf")) {
        std::cerr << "Error loading font. Make sure OpenSans-Regular.ttf is in the assets directory." << std::endl;
    }
    setupText();
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        if (currentState == GameState::Playing) {
            update();
        }
        render();
    }
}

void Game::processEvents() {
    for (std::optional<sf::Event> event = window.pollEvent(); event; event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        if (currentState != GameState::Playing) {
            continue;
        }

        if (auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseButtonPressed->button != sf::Mouse::Button::Left) continue;

            const int c = mouseButtonPressed->position.x / TILE_SIZE;
            const int r = mouseButtonPressed->position.y / TILE_SIZE;

            if (c < 0 || c >= BOARD_WIDTH || r < 0 || r >= BOARD_HEIGHT) continue;

            dragStartTile = sf::Vector2i(c, r);

            if (selectedTile.has_value() && selectedTile.value() != dragStartTile.value()) {
                // This is the second click in a click-pair
                handleSwap(*selectedTile, *dragStartTile);
                selectedTile = std::nullopt;
                dragStartTile = std::nullopt;
            } else {
                // This is the first click, or a click on the same tile
                selectedTile = sf::Vector2i(c, r);
            }
        }

        if (auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseButtonReleased->button != sf::Mouse::Button::Left) continue;
            
            if (dragStartTile.has_value()) {
                const int c = mouseButtonReleased->position.x / TILE_SIZE;
                const int r = mouseButtonReleased->position.y / TILE_SIZE;
                sf::Vector2i endTile(c, r);

                if (endTile != *dragStartTile) {
                    // This was a drag and release on a different tile
                    handleSwap(*dragStartTile, endTile);
                    selectedTile = std::nullopt; // Clear selection after a drag
                }
                // If it was a release on the same tile, it's treated as the first part of a click-select, 
                // so we leave selectedTile as is.
                dragStartTile = std::nullopt;
            }
        }
    }
}

void Game::handleSwap(sf::Vector2i tile1, sf::Vector2i tile2) {
    if (board.canSwap(tile1.y, tile1.x, tile2.y, tile2.x)) {
        board.swapGems(tile1.y, tile1.x, tile2.y, tile2.x);
        
        if (board.findAndProcessMatches()) {
            if (monster.updateActionCounter(50)) {
                player.takeDamage(20);
            }
        } else {
            board.swapGems(tile1.y, tile1.x, tile2.y, tile2.x); // Swap back
        }
    }
}

// This function is now unused but kept for potential future needs.
void Game::handlePlayerInput(sf::Vector2i mousePos) {}


void Game::update() {
    // Check for win/loss conditions
    if (player.getCurrentHp() <= 0 || monster.getCurrentHp() <= 0) {
        currentState = GameState::GameOver;
        if (player.getCurrentHp() <= 0) {
            gameOverText.setString("You have been defeated!");
        } else {
            gameOverText.setString("You are victorious!");
        }
    }

    // Update UI text
    playerHpText.setString("Player HP: " + std::to_string(player.getCurrentHp()));
    monsterHpText.setString("Monster HP: " + std::to_string(monster.getCurrentHp()));
    manaText.setString("Fire: " + std::to_string(player.getMana(GemType::Fire)) + " | Water: " + std::to_string(player.getMana(GemType::Water)));
}

void Game::render() {
    window.clear(sf::Color(50, 50, 50)); // Dark grey background

    // Draw the board
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE - 2, TILE_SIZE - 2));
            tile.setPosition({static_cast<float>(c * TILE_SIZE + 1), static_cast<float>(r * TILE_SIZE + 1)});

            Gem gem = board.getGem(r, c);
            switch (gem.type) {
                case GemType::Skull: tile.setFillColor(sf::Color::White); break;
                case GemType::Fire:  tile.setFillColor(sf::Color::Red); break;
                case GemType::Water: tile.setFillColor(sf::Color::Blue); break;
            }
            window.draw(tile);

            // Draw highlight for selected tile
            if (selectedTile.has_value() && selectedTile->x == c && selectedTile->y == r) {
                sf::RectangleShape highlight(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                highlight.setPosition(sf::Vector2f(c * TILE_SIZE, r * TILE_SIZE));
                highlight.setFillColor(sf::Color::Transparent);
                highlight.setOutlineColor(sf::Color::Yellow);
                highlight.setOutlineThickness(2.f);
                window.draw(highlight);
            }
        }
    }

    // Draw UI
    window.draw(playerHpText);
    window.draw(monsterHpText);
    window.draw(manaText);

    if (currentState == GameState::GameOver) {
        window.draw(gameOverText);
    }

    window.display();
}

void Game::setupText() {
    playerHpText.setFont(font);
    playerHpText.setCharacterSize(24);
    playerHpText.setFillColor(sf::Color::White);
    playerHpText.setPosition({10, static_cast<float>(BOARD_HEIGHT * TILE_SIZE + 10)});

    monsterHpText.setFont(font);
    monsterHpText.setCharacterSize(24);
    monsterHpText.setFillColor(sf::Color::White);
    monsterHpText.setPosition({static_cast<float>(window.getSize().x - 200), static_cast<float>(BOARD_HEIGHT * TILE_SIZE + 10)});

    manaText.setFont(font);
    manaText.setCharacterSize(24);
    manaText.setFillColor(sf::Color::White);
    manaText.setPosition({10, static_cast<float>(BOARD_HEIGHT * TILE_SIZE + 40)});

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Yellow);
    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
    gameOverText.setPosition({window.getSize().x / 2.0f, window.getSize().y / 2.0f});
}
