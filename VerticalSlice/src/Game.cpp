#include "Game.h"
#include <iostream>
#include <algorithm>
#include <string>

// Constants for the turn-based speed system
const int MATCH_SPEED_COST = 30;
const int SPELL_SPEED_COST = 70;

// Game Constructor: Initializes the window, game objects, and UI layout.
Game::Game()
    : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Arcane Ascension - Prototype"),
      dataManager(),
      player(100, {}), 
      monster(0, 0),
      uiManager(),
      currentState(GameState::Playing),
      isAnimatingSwap(false),
      isAnimatingDestruction(false),
      isAnimatingRefill(false),
      isReshuffling(false)
{
    if (!font.openFromFile("assets/OpenSans-Regular.ttf")) {
        std::cerr << "Error loading font." << std::endl;
    }

    if (!dataManager.loadSpells("data/spells.json") || !dataManager.loadMonsterData("data/monster.json")) {
        std::cerr << "Failed to load game data." << std::endl;
        window.close();
        return;
    }

    player = Player(100, dataManager.getAllSpells());
    monster = Monster(dataManager.getMonsterHP(), dataManager.getMonsterSpeed());

    board.initialize();
    
    const int boardPixelWidth = BOARD_WIDTH * TILE_SIZE;
    const int boardPixelHeight = BOARD_HEIGHT * TILE_SIZE;
    boardOrigin.x = (WINDOW_WIDTH - boardPixelWidth) / 2;
    boardOrigin.y = WINDOW_HEIGHT - boardPixelHeight - 20;

    uiManager.setup(font, player, window.getSize(), boardOrigin);
}

// Main game loop
void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

// Handles all user input, such as closing the window and interacting with the UI.
void Game::processEvents() {
    for (std::optional<sf::Event> event = window.pollEvent(); event; event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        // Ignore input if the game is not in the 'Playing' state (i.e., during animations)
        if (currentState != GameState::Playing) continue;

        if (auto* mbp = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mbp->button != sf::Mouse::Button::Left) continue;
            
            // 1. Let the UIManager handle the event first.
            auto uiAction = uiManager.handleEvent(*event);
            if (uiAction.has_value()) {
                if (uiAction->type == UIActionType::CastSpell) {
                    int damage = player.castSpell(uiAction->spellIndex);
                    if (damage > 0) {
                        monster.takeDamage(damage);
                        std::cout << "Casted spell, dealing " << damage << " damage!\n";
                        if (monster.isTurnReady(SPELL_SPEED_COST)) {
                            player.takeDamage(dataManager.getMonsterAttackDamage());
                            std::cout << "Monster's turn! Player takes " << dataManager.getMonsterAttackDamage() << " damage!\n";
                        }
                    } else {
                        std::cout << "Not enough mana to cast spell!\n";
                    }
                }
                continue; // UI handled the event, so we're done.
            }

            // 2. If the UI didn't handle it, check for board clicks.
            const int c = (mbp->position.x - boardOrigin.x) / TILE_SIZE;
            const int r = (mbp->position.y - boardOrigin.y) / TILE_SIZE;
            if (c < 0 || c >= BOARD_WIDTH || r < 0 || r >= BOARD_HEIGHT) continue;
            
            dragStartTile = sf::Vector2i(c, r);
            if (selectedTile.has_value() && selectedTile.value() != dragStartTile.value()) {
                handleSwap(*selectedTile, *dragStartTile);
                selectedTile = std::nullopt;
                dragStartTile = std::nullopt;
            } else {
                selectedTile = sf::Vector2i(c, r);
            }
        }

        // Handle drag-and-drop swaps
        if (auto* mbr = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mbr->button != sf::Mouse::Button::Left) continue;
            if (dragStartTile.has_value()) {
                const int c = (mbr->position.x - boardOrigin.x) / TILE_SIZE;
                const int r = (mbr->position.y - boardOrigin.y) / TILE_SIZE;
                sf::Vector2i endTile(c, r);
                if (endTile != *dragStartTile) {
                    handleSwap(*dragStartTile, endTile);
                    selectedTile = std::nullopt;
                }
                dragStartTile = std::nullopt;
            }
        }
    }
}

// Initiates a swap animation if the move is valid.
void Game::handleSwap(sf::Vector2i tile1, sf::Vector2i tile2) {
    if (board.canSwap(tile1.y, tile1.x, tile2.y, tile2.x)) {
        currentState = GameState::Animating;
        isAnimatingSwap = true;
        animatingGems = {tile1, tile2};
        animationClock.restart();
    }
}

// Applies the consequences of a match (damage for skulls, mana for colors).
void Game::applyMatchConsequences(const std::map<GemType, int>& matchResults) {
    for (auto const& [gemType, count] : matchResults) {
        if (gemType == GemType::Skull) {
            int damage = count * 10;
            monster.takeDamage(damage);
            std::cout << "Matched " << count << " skulls. Dealt " << damage << " damage.\n";
        } else {
            player.addMana(gemType, count);
            std::cout << "Matched " << count << " gems of type " << (int)gemType << ". Gained " << count << " mana!\n";
        }
    }
}

// Main update function: handles game logic, animations, and state transitions.
void Game::update() {
    const float swapAnimationDuration = 0.2f;
    const float destructionAnimationDuration = 0.3f;
    const float refillAnimationDuration = 0.3f;
    const float reshuffleAnimationDuration = 0.5f;

    // --- Animation State Machine ---
    if (currentState == GameState::Animating) {
        // 1. SWAP ANIMATION
        if (isAnimatingSwap) {
            if (animationClock.getElapsedTime().asSeconds() >= swapAnimationDuration) {
                isAnimatingSwap = false;
                sf::Vector2i t1 = animatingGems.first;
                sf::Vector2i t2 = animatingGems.second;
                board.swapGems(t1.y, t1.x, t2.y, t2.x);
                auto matches = board.findMatches();
                if (!matches.empty()) { // Swap resulted in a match
                    std::map<GemType, int> results;
                    for (const auto& pos : matches) results[board.getGem(pos.first, pos.second).type]++;
                    applyMatchConsequences(results);
                    // This was a direct player action, so check if the monster gets a turn.
                    if (monster.isTurnReady(MATCH_SPEED_COST)) {
                        player.takeDamage(dataManager.getMonsterAttackDamage());
                        std::cout << "Monster's turn! Player takes " << dataManager.getMonsterAttackDamage() << " damage!\n";
                    }
                    isAnimatingDestruction = true;
                    destroyingGems = matches;
                    animationClock.restart();
                } else { // Invalid swap, animate back
                    isAnimatingSwap = true;
                    animatingGems = {t2, t1};
                    animationClock.restart();
                    board.swapGems(t1.y, t1.x, t2.y, t2.x);
                }
                if (!isAnimatingSwap && !isAnimatingDestruction) currentState = GameState::Playing;
            }
        // 2. DESTRUCTION ANIMATION
        } else if (isAnimatingDestruction) {
            if (animationClock.getElapsedTime().asSeconds() >= destructionAnimationDuration) {
                isAnimatingDestruction = false;
                board.processMatches(destroyingGems);
                fallInfo = board.applyGravityAndRefill();
                isAnimatingRefill = true;
                animationClock.restart();
            }
        // 3. REFILL ANIMATION
        } else if (isAnimatingRefill) {
            if (animationClock.getElapsedTime().asSeconds() >= refillAnimationDuration) {
                isAnimatingRefill = false;
                auto newMatches = board.findMatches(); // Check for cascades
                if (!newMatches.empty()) {
                    std::map<GemType, int> results;
                    for (const auto& pos : newMatches) results[board.getGem(pos.first, pos.second).type]++;
                    applyMatchConsequences(results);
                    isAnimatingDestruction = true; // Chain into another destruction
                    destroyingGems = newMatches;
                    animationClock.restart();
                } else if (board.findAllValidSwaps().empty()) { // Check for deadlock
                    isReshuffling = true;
                    animationClock.restart();
                } else {
                    currentState = GameState::Playing; // Return to player control
                }
            }
        // 4. RESHUFFLE ANIMATION
        } else if (isReshuffling) {
            if (animationClock.getElapsedTime().asSeconds() >= reshuffleAnimationDuration) {
                isReshuffling = false;
                board.initialize();
                currentState = GameState::Playing;
            }
        }
    }

    // --- Game State Logic ---
    if (currentState == GameState::Playing) {
        if (player.getCurrentHp() <= 0 || monster.getCurrentHp() <= 0) {
            currentState = GameState::GameOver;
        }
    }

    // --- UI Updates ---
    uiManager.update(player, monster);
}

// Main render function: draws all game elements to the screen.
void Game::render() {
    window.clear(sf::Color(50, 50, 50));

    // 1. Render static UI panels and frames (now handled by UIManager)
    // window.draw(leftPanel);
    // window.draw(rightPanel);
    // window.draw(boardFrame);

    // 2. Render the game board and animations
    if (!isReshuffling) {
        // Render static gems (gems not currently involved in an animation)
        for (int r = 0; r < BOARD_HEIGHT; ++r) {
            for (int c = 0; c < BOARD_WIDTH; ++c) {
                bool shouldDraw = true;
                if (isAnimatingSwap && ((r == animatingGems.first.y && c == animatingGems.first.x) || (r == animatingGems.second.y && c == animatingGems.second.x))) shouldDraw = false;
                if (isAnimatingDestruction && destroyingGems.count({r, c})) shouldDraw = false;
                if (isAnimatingRefill) {
                    for (const auto& info : fallInfo) if (info.col == c && info.endRow == r) { shouldDraw = false; break; }
                }
                if (shouldDraw) {
                    sf::RectangleShape tile({(float)TILE_SIZE - 2, (float)TILE_SIZE - 2});
                    tile.setPosition({(float)boardOrigin.x + c * TILE_SIZE + 1, (float)boardOrigin.y + r * TILE_SIZE + 1});
                    Gem gem = board.getGem(r, c);
                    switch (gem.type) {
                        case GemType::Skull: tile.setFillColor(sf::Color::White); break;
                        case GemType::Fire:  tile.setFillColor(sf::Color::Red); break;
                        case GemType::Water: tile.setFillColor(sf::Color::Blue); break;
                        case GemType::Earth: tile.setFillColor(sf::Color::Green); break;
                        case GemType::Light: tile.setFillColor(sf::Color::Yellow); break;
                        case GemType::Empty: default: shouldDraw = false; break;
                    }
                    if(shouldDraw) window.draw(tile);
                }
            }
        }
    }

    // Render swap animation
    if (isAnimatingSwap) {
        float p = std::min(1.f, animationClock.getElapsedTime().asSeconds() / 0.2f);
        sf::Vector2f p1((float)animatingGems.first.x * TILE_SIZE + 1.f + boardOrigin.x, (float)animatingGems.first.y * TILE_SIZE + 1.f + boardOrigin.y);
        sf::Vector2f p2((float)animatingGems.second.x * TILE_SIZE + 1.f + boardOrigin.x, (float)animatingGems.second.y * TILE_SIZE + 1.f + boardOrigin.y);
        sf::RectangleShape t1({(float)TILE_SIZE - 2, (float)TILE_SIZE - 2});
        t1.setPosition(p1 + (p2 - p1) * p);
        Gem g1 = board.getGem(animatingGems.first.y, animatingGems.first.x);
        switch (g1.type) {
            case GemType::Skull: t1.setFillColor(sf::Color::White); break;
            case GemType::Fire:  t1.setFillColor(sf::Color::Red); break;
            case GemType::Water: t1.setFillColor(sf::Color::Blue); break;
            case GemType::Earth: t1.setFillColor(sf::Color::Green); break;
            case GemType::Light: t1.setFillColor(sf::Color::Yellow); break;
            default: break;
        }
        window.draw(t1);
        sf::RectangleShape t2({(float)TILE_SIZE - 2, (float)TILE_SIZE - 2});
        t2.setPosition(p2 + (p1 - p2) * p);
        Gem g2 = board.getGem(animatingGems.second.y, animatingGems.second.x);
        switch (g2.type) {
            case GemType::Skull: t2.setFillColor(sf::Color::White); break;
            case GemType::Fire:  t2.setFillColor(sf::Color::Red); break;
            case GemType::Water: t2.setFillColor(sf::Color::Blue); break;
            case GemType::Earth: t2.setFillColor(sf::Color::Green); break;
            case GemType::Light: t2.setFillColor(sf::Color::Yellow); break;
            default: break;
        }
        window.draw(t2);
    }

    // Render destruction animation
    if (isAnimatingDestruction) {
        float scale = 1.f - std::min(1.f, animationClock.getElapsedTime().asSeconds() / 0.3f);
        for (const auto& pos : destroyingGems) {
            sf::RectangleShape tile({(float)(TILE_SIZE - 2) * scale, (float)(TILE_SIZE - 2) * scale});
            tile.setOrigin({(float)(TILE_SIZE - 2) / 2.f, (float)(TILE_SIZE - 2) / 2.f});
            tile.setPosition({(float)boardOrigin.x + pos.second * TILE_SIZE + TILE_SIZE / 2.f, (float)boardOrigin.y + pos.first * TILE_SIZE + TILE_SIZE / 2.f});
            Gem gem = board.getGem(pos.first, pos.second);
            switch (gem.type) {
                case GemType::Skull: tile.setFillColor(sf::Color::White); break;
                case GemType::Fire:  tile.setFillColor(sf::Color::Red); break;
                case GemType::Water: tile.setFillColor(sf::Color::Blue); break;
                case GemType::Earth: tile.setFillColor(sf::Color::Green); break;
                case GemType::Light: tile.setFillColor(sf::Color::Yellow); break;
                default: break;
            }
            window.draw(tile);
        }
    }

    // Render refill animation
    if (isAnimatingRefill) {
        float p = std::min(1.f, animationClock.getElapsedTime().asSeconds() / 0.3f);
        for (const auto& info : fallInfo) {
            sf::Vector2f start((float)boardOrigin.x + info.col * TILE_SIZE + 1.f, (float)boardOrigin.y + info.startRow * TILE_SIZE + 1.f);
            sf::Vector2f end((float)boardOrigin.x + info.col * TILE_SIZE + 1.f, (float)boardOrigin.y + info.endRow * TILE_SIZE + 1.f);
            sf::RectangleShape tile({(float)TILE_SIZE - 2, (float)TILE_SIZE - 2});
            tile.setPosition(start + (end - start) * p);
            switch (info.type) {
                case GemType::Skull: tile.setFillColor(sf::Color::White); break;
                case GemType::Fire:  tile.setFillColor(sf::Color::Red); break;
                case GemType::Water: tile.setFillColor(sf::Color::Blue); break;
                case GemType::Earth: tile.setFillColor(sf::Color::Green); break;
                case GemType::Light: tile.setFillColor(sf::Color::Yellow); break;
                default: break;
            }
            window.draw(tile);
        }
    }

    // Render selected tile highlight
    if (!isAnimatingSwap && !isAnimatingDestruction && !isAnimatingRefill && selectedTile.has_value()) {
        sf::RectangleShape h({(float)TILE_SIZE, (float)TILE_SIZE});
        h.setPosition({(float)boardOrigin.x + selectedTile->x * TILE_SIZE, (float)boardOrigin.y + selectedTile->y * TILE_SIZE});
        h.setFillColor(sf::Color::Transparent);
        h.setOutlineColor(sf::Color::Yellow);
        h.setOutlineThickness(2.f);
        window.draw(h);
    }

    // 3. Render UI Text and Gauges (now handled by UIManager)
    uiManager.render(window, currentState);

    window.display();
}


// Initializes and positions all SFML Text objects.
