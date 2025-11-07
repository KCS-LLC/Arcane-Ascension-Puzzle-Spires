#include "Game.h"
#include "SpireData.h"
#include "StringUtils.h"
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
      currentState(GameState::Exploration),
      isAnimatingSwap(false),
      isAnimatingDestruction(false),
      isAnimatingRefill(false),
      isReshuffling(false)
{
    if (!font.openFromFile("assets/OpenSans-Regular.ttf")) {
        std::cerr << "Error loading font." << std::endl;
    }
    uiManager = std::make_unique<UIManager>(font);

    if (!dataManager.loadSpells("data/spells.json") || 
        !dataManager.loadMonsterData("data/monster.json") ||
        !dataManager.loadFloor("data/floor1.json")) {
        std::cerr << "Failed to load game data." << std::endl;
        window.close();
        return;
    }

    currentFloor = dataManager.getFloor();
    currentRoom = dataManager.getRoomById(currentFloor.startRoomId);
    visitedRoomIds.insert(currentRoom->id);

    player = Player(100, dataManager.getAllSpells());
    monster = Monster(dataManager.getMonsterHP(), dataManager.getMonsterSpeed());
    monster.name = dataManager.getMonsterName();

    board.initialize();
    
    const int boardPixelWidth = BOARD_WIDTH * TILE_SIZE;
    const int boardPixelHeight = BOARD_HEIGHT * TILE_SIZE;
    boardOrigin.x = (WINDOW_WIDTH - boardPixelWidth) / 2;
    boardOrigin.y = WINDOW_HEIGHT - boardPixelHeight - 20;

    loadTextures();
    uiManager->setup(player, window.getSize(), boardOrigin);
}

void Game::loadTextures() {
    if (!gemTextures[GemType::Fire].loadFromFile("assets/gem_fire.png")) std::cerr << "Failed to load fire gem texture\n";
    if (!gemTextures[GemType::Water].loadFromFile("assets/gem_water.png")) std::cerr << "Failed to load water gem texture\n";
    if (!gemTextures[GemType::Earth].loadFromFile("assets/gem_earth.png")) std::cerr << "Failed to load earth gem texture\n";
    if (!gemTextures[GemType::Light].loadFromFile("assets/gem_light.png")) std::cerr << "Failed to load light gem texture\n";
    if (!gemTextures[GemType::Skull].loadFromFile("assets/fist.png")) std::cerr << "Failed to load skull/fist texture\n";
    // Load other textures as needed, even if not on the board yet
    if (!gemTextures[GemType::Air].loadFromFile("assets/gem_air.png")) std::cerr << "Failed to load air gem texture\n";
    if (!gemTextures[GemType::Umbral].loadFromFile("assets/gem_umbral.png")) std::cerr << "Failed to load umbral gem texture\n";
    if (!gemTextures[GemType::Life].loadFromFile("assets/gem_life.png")) std::cerr << "Failed to load life gem texture\n";
    if (!gemTextures[GemType::Death].loadFromFile("assets/gem_death.png")) std::cerr << "Failed to load death gem texture\n";
    if (!gemTextures[GemType::Mental].loadFromFile("assets/gem_mental.png")) std::cerr << "Failed to load mental gem texture\n";
    if (!gemTextures[GemType::Perception].loadFromFile("assets/gem_perception.png")) std::cerr << "Failed to load perception gem texture\n";
    if (!gemTextures[GemType::Transference].loadFromFile("assets/gem_transference.png")) std::cerr << "Failed to load transference gem texture\n";
    if (!gemTextures[GemType::Enhancement].loadFromFile("assets/gem_enhancement.png")) std::cerr << "Failed to load enhancement gem texture\n";
    if (!gemTextures[GemType::Coin].loadFromFile("assets/gem_coin.png")) std::cerr << "Failed to load coin gem texture\n";
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
        // Ignore input if the game is in a non-interactive state
        if (currentState == GameState::Animating || currentState == GameState::GameOver) continue;

        if (auto* mbp = event->getIf<sf::Event::MouseButtonPressed>()) {
            // Handle click-through for placeholder screens
            if (currentState == GameState::Treasure || currentState == GameState::Special ||
                currentState == GameState::Puzzle || currentState == GameState::Trap ||
                currentState == GameState::Sanctuary || currentState == GameState::AgilityChallenge ||
                currentState == GameState::EnduranceChallenge || currentState == GameState::MagicChallenge)
            {
                currentState = GameState::Exploration;
                continue;
            }

            if (mbp->button != sf::Mouse::Button::Left) continue;
            
            // 1. Let the UIManager handle the event first.
            UIAction uiAction;
            if (uiManager->handleEvent(*event, currentState, currentRoom, uiAction)) {
                // UI has consumed the event. Check if it was a specific action.
                if (uiAction.type == UIActionType::CastSpell) {
                    int damage = player.castSpell(uiAction.spellIndex);
                    if (damage > 0) {
                        monster.takeDamage(damage);
                        std::cout << "Casted spell, dealing " << damage << " damage!\n";
                        if (monster.isTurnReady(SPELL_SPEED_COST)) {
                            player.takeDamage(dataManager.getMonsterAttackDamage());
                            showPlayerDamageEffect = true;
                            playerDamageClock.restart();
                            std::cout << "Monster's turn! Player takes " << dataManager.getMonsterAttackDamage() << " damage!\n";
                        }
                    } else {
                        std::cout << "Not enough mana to cast spell!\n";
                    }
                } else if (uiAction.type == UIActionType::ChangeRoom) {
                    moveToRoom(uiAction.destinationRoomId);
                }
                // If it was just a click on a panel, we do nothing.
                continue; // In any case, the UI handled it, so we're done with this event.
            }

            // 2. If the UI didn't handle it, check for board clicks (only in combat)
            if (currentState == GameState::Playing) {
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
        }

        // Handle drag-and-drop swaps (only in combat)
        if (currentState == GameState::Playing) {
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
                        showPlayerDamageEffect = true;
                        playerDamageClock.restart();
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
        if (player.getCurrentHp() <= 0) {
            currentState = GameState::GameOver;
        } else if (monster.getCurrentHp() <= 0) {
            clearedRoomIds.insert(currentRoom->id); // Mark room as cleared
            currentState = GameState::Exploration; // Player wins, go back to exploring
        }
    }

    // --- Damage Effect Logic ---
    if (showPlayerDamageEffect && playerDamageClock.getElapsedTime().asMilliseconds() > 200) {
        showPlayerDamageEffect = false;
    }

    // --- UI Updates ---
    uiManager->update(player, monster, currentState, currentRoom, visitedRoomIds, dataManager);
}

// Main render function: draws all game elements to the screen.
void Game::render() {
    window.clear(sf::Color(50, 50, 50));

    // 1. Render UI via UIManager
    uiManager->render(window, currentState, showPlayerDamageEffect);

    // 2. Render the game board and animations ONLY during combat states
    if (currentState == GameState::Playing || currentState == GameState::Animating || currentState == GameState::GameOver) {
        if (!isReshuffling) {
            // Render static gems
            for (int r = 0; r < BOARD_HEIGHT; ++r) {
                for (int c = 0; c < BOARD_WIDTH; ++c) {
                    bool shouldDraw = true;
                    if (isAnimatingSwap && ((r == animatingGems.first.y && c == animatingGems.first.x) || (r == animatingGems.second.y && c == animatingGems.second.x))) shouldDraw = false;
                    if (isAnimatingDestruction && destroyingGems.count({r, c})) shouldDraw = false;
                    if (isAnimatingRefill) {
                        for (const auto& info : fallInfo) if (info.col == c && info.endRow == r) { shouldDraw = false; break; }
                    }
                    if (shouldDraw) {
                        Gem gem = board.getGem(r, c);
                        if (gem.type != GemType::Empty) {
                            const sf::Texture& texture = gemTextures.at(gem.type);
                            sf::Sprite sprite(texture);
                            float scale = (float)TILE_SIZE / texture.getSize().x;
                            sprite.setScale({scale, scale});
                            sprite.setPosition({(float)boardOrigin.x + c * TILE_SIZE, (float)boardOrigin.y + r * TILE_SIZE});
                            window.draw(sprite);
                        }
                    }
                }
            }
        }

        // Render swap animation
        if (isAnimatingSwap) {
            float p = std::min(1.f, animationClock.getElapsedTime().asSeconds() / 0.2f);
            sf::Vector2f p1((float)animatingGems.first.x * TILE_SIZE + boardOrigin.x, (float)animatingGems.first.y * TILE_SIZE + boardOrigin.y);
            sf::Vector2f p2((float)animatingGems.second.x * TILE_SIZE + boardOrigin.x, (float)animatingGems.second.y * TILE_SIZE + boardOrigin.y);
            
            Gem g1 = board.getGem(animatingGems.first.y, animatingGems.first.x);
            const sf::Texture& tex1 = gemTextures.at(g1.type);
            sf::Sprite s1(tex1);
            float scale1 = (float)TILE_SIZE / tex1.getSize().x;
            s1.setScale({scale1, scale1});
            s1.setPosition(p1 + (p2 - p1) * p);
            window.draw(s1);

            Gem g2 = board.getGem(animatingGems.second.y, animatingGems.second.x);
            const sf::Texture& tex2 = gemTextures.at(g2.type);
            sf::Sprite s2(tex2);
            float scale2 = (float)TILE_SIZE / tex2.getSize().x;
            s2.setScale({scale2, scale2});
            s2.setPosition(p2 + (p1 - p2) * p);
            window.draw(s2);
        }

        // Render destruction animation
        if (isAnimatingDestruction) {
            float animProgress = std::min(1.f, animationClock.getElapsedTime().asSeconds() / 0.3f);
            float scale = 1.f - animProgress;
            for (const auto& pos : destroyingGems) {
                Gem gem = board.getGem(pos.first, pos.second);
                const sf::Texture& texture = gemTextures.at(gem.type);
                sf::Sprite sprite(texture);
                float base_scale = (float)TILE_SIZE / texture.getSize().x;
                sprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
                sprite.setScale({base_scale * scale, base_scale * scale});
                sprite.setPosition({(float)boardOrigin.x + pos.second * TILE_SIZE + TILE_SIZE / 2.f, (float)boardOrigin.y + pos.first * TILE_SIZE + TILE_SIZE / 2.f});
                window.draw(sprite);
            }
        }

        // Render refill animation
        if (isAnimatingRefill) {
            float p = std::min(1.f, animationClock.getElapsedTime().asSeconds() / 0.3f);
            for (const auto& info : fallInfo) {
                sf::Vector2f start((float)boardOrigin.x + info.col * TILE_SIZE, (float)boardOrigin.y + info.startRow * TILE_SIZE);
                sf::Vector2f end((float)boardOrigin.x + info.col * TILE_SIZE, (float)boardOrigin.y + info.endRow * TILE_SIZE);
                const sf::Texture& texture = gemTextures.at(info.type);
                sf::Sprite sprite(texture);
                float scale = (float)TILE_SIZE / texture.getSize().x;
                sprite.setScale({scale, scale});
                sprite.setPosition(start + (end - start) * p);
                window.draw(sprite);
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
    }

    window.display();
}

void Game::moveToRoom(int destinationRoomId) {
    // Find the destination room in the floor data
    const Room* destination = nullptr;
    for (const auto& room : currentFloor.rooms) {
        if (room.id == destinationRoomId) {
            destination = &room;
            break;
        }
    }

    if (destination) {
        // Add current room to history before moving
        if (currentRoom) {
            roomHistory.push_back(currentRoom->id);
        }
        visitedRoomIds.insert(destinationRoomId);
        currentRoom = destination;
        std::cout << "Moving to room " << destinationRoomId << " (" << roomTypeToString(currentRoom->type) << ")" << std::endl;
        
        // If room is already cleared, just explore it.
        if (clearedRoomIds.count(destination->id)) {
            currentState = GameState::Exploration;
            return;
        }

        switch (currentRoom->type) {
            case RoomType::Combat:
            case RoomType::Boss:
                // TODO: Load the correct monster for this room
                monster.reset(); // For now, just reset the current monster
                board.initialize();
                currentState = GameState::Playing;
                break;
            case RoomType::Treasure:
                currentState = GameState::Treasure;
                break;
            case RoomType::Special:
                currentState = GameState::Special;
                break;
            case RoomType::Puzzle:
                currentState = GameState::Puzzle;
                break;
            case RoomType::Trap:
                currentState = GameState::Trap;
                break;
            case RoomType::Sanctuary: currentState = GameState::Sanctuary; break;
            case RoomType::AgilityChallenge: currentState = GameState::AgilityChallenge; break;
            case RoomType::EnduranceChallenge: currentState = GameState::EnduranceChallenge; break;
            case RoomType::MagicChallenge: currentState = GameState::MagicChallenge; break;
            default:
                // For any other room type, just go back to exploration for now
                currentState = GameState::Exploration;
                break;
        }
    } else {
        std::cerr << "Error: Tried to move to a non-existent room ID: " << destinationRoomId << std::endl;
    }
}