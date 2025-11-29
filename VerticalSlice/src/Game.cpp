#include "PCH.h"
#include "Game.h"
#include <iostream>
#include <cstdint>
#include <cmath> // For sin()
#include "GemSubType.h"
#include "BaseGem.h"
#include "Constants.h"
#include "Structs.h"
#include "StringUtils.h"
#include <numeric>
#include <algorithm>
#include <random>

// Global texture map, defined here
std::map<GemSubType, sf::Texture> gemTextures;

Game::Game()
    : m_window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Judgement", sf::Style::Titlebar | sf::Style::Close),
      dataManager(),
      m_uiManager(dataManager.getFont()),
      m_gemFactory(dataManager),
      m_board(BOARD_WIDTH, BOARD_HEIGHT, m_gemFactory),
      m_player(100, {}),
      m_monster(dataManager.getMonsterData()),
      m_gameMode(GameMode::JUDGEMENT),
      m_gameState(GameState::Intro),
      m_currentTrialOrderIndex(0),
      m_isAnimating(false),
      m_isAnimatingSwap(false),
      m_isAnimatingDestruction(false),
      m_isAnimatingRefill(false),
      m_currentTurn(0),
      m_currentScore(0)
{
    m_window.setFramerateLimit(60);
    loadTextures();

    const int boardPixelWidth = BOARD_WIDTH * TILE_SIZE;
    const int boardPixelHeight = BOARD_HEIGHT * TILE_SIZE;
    m_boardOrigin.x = (WINDOW_WIDTH - boardPixelWidth) / 2.0f;
    m_boardOrigin.y = WINDOW_HEIGHT - boardPixelHeight - 20.0f;

    m_uiManager.setup(m_player, m_window.getSize(), m_boardOrigin, {}); // Passing empty attunements for now

    dataManager.loadAttunements("data/attunements.json");
    const Attunement* defaultAttunement = dataManager.getAttunementById("diviner");
    if (defaultAttunement) {
        m_player.setAttunement(*defaultAttunement, dataManager);
    } else {
        std::cerr << "Error: Could not find default attunement 'adept'." << std::endl;
    }
    m_judgementTrials = dataManager.getJudgementTrials();
    if (!m_judgementTrials.empty()) {
        m_trialOrder.resize(m_judgementTrials.size());
        std::iota(m_trialOrder.begin(), m_trialOrder.end(), 0); // Fill with 0, 1, 2...
        
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(m_trialOrder.begin(), m_trialOrder.end(), g);
        
        m_currentTrialOrderIndex = 0;
        setupJudgementTrial(m_judgementTrials[m_trialOrder[m_currentTrialOrderIndex]]);
    }
}

void Game::loadTextures() {
    const auto& catalog = dataManager.getGemCatalog();
    for (const auto& pair : catalog) {
        const GemCatalogEntry& entry = pair.second;
        if (!entry.texturePath.empty()) {
            if (!gemTextures[pair.first].loadFromFile(entry.texturePath)) {
                std::cerr << "Failed to load texture for " << entry.name 
                          << " from " << entry.texturePath << std::endl;
            }
        }
    }
    if (!m_effectIconTextures["burning_tile_effect"].loadFromFile("assets/effect_burning.png")) {
        std::cerr << "Failed to load burning tile effect icon" << std::endl;
    }
}

void Game::setupJudgementTrial(const JudgementTrial& trial) {
    m_currentJudgementTrial = trial;
    m_gameState = GameState::Trial;
    m_currentScore = 0;
    m_currentTurn = 0;
    m_uiManager.setupTrial(trial); 

    if (trial.type == JudgementTrialType::Power) {
        m_board.initializeForPowerTrial();
    } else {
        // Default gems for now, as trial.gems is removed
        m_board.initialize({GemSubType::Fire, GemSubType::Water, GemSubType::Earth, GemSubType::Air});
    }
    m_trialClock.restart();
}

void Game::startTowerClimb() {
    m_gameMode = GameMode::TOWER_CLIMB;
    m_gameState = GameState::Exploration;
    m_currentFloor = dataManager.getFloor();
    m_currentRoom = &m_currentFloor.rooms[0];
    if (m_currentRoom) {
        m_visitedRoomIds.insert(m_currentRoom->id);
    }
    // TODO: Potentially load monster data for the first room here if it's a combat room
}

void Game::moveToRoom(int destinationRoomId) {
    // Find the destination room in the floor data
    const Room* destination = nullptr;
    for (const auto& room : m_currentFloor.rooms) {
        if (room.id == destinationRoomId) {
            destination = &room;
            break;
        }
    }

    if (destination) {
        m_currentRoom = destination;
        m_visitedRoomIds.insert(destinationRoomId);
        std::cout << "Moving to room " << destinationRoomId << " (" << roomTypeToString(m_currentRoom->type) << ")" << std::endl;
        
        // If room is already cleared, just explore it.
        // if (clearedRoomIds.count(destination->id)) {
        //     m_gameState = GameState::Exploration;
        //     return;
        // }

        switch (m_currentRoom->type) {
            case RoomType::Combat:
            case RoomType::Boss:
                // TODO: Load the correct monster for this room
                m_monster = Monster(dataManager.getMonsterData());

                // Create the dynamic gem pool for combat
                {
                    std::set<GemSubType> gemPoolSet;
                    for (GemSubType type : m_player.getManaTypes()) {
                        gemPoolSet.insert(type);
                    }
                    for (GemSubType type : m_monster.getManaAffinities()) {
                        gemPoolSet.insert(type);
                    }
                    gemPoolSet.insert(GemSubType::Skull); // Always include skulls
                    
                    m_combatGemPool = std::vector<GemSubType>(gemPoolSet.begin(), gemPoolSet.end());
                    m_board.initialize(m_combatGemPool);
                }
                m_gameState = GameState::Playing;
                break;
            case RoomType::Treasure:
                m_gameState = GameState::Treasure;
                break;
            case RoomType::Special:
                m_gameState = GameState::Special;
                break;
            case RoomType::Puzzle:
                m_gameState = GameState::Puzzle;
                break;
            case RoomType::Trap:
                m_gameState = GameState::Trap;
                break;
            case RoomType::Sanctuary: 
                m_gameState = GameState::Sanctuary; 
                break;
            case RoomType::AgilityChallenge: 
                m_gameState = GameState::AgilityChallenge; 
                break;
            case RoomType::EnduranceChallenge: 
                m_gameState = GameState::EnduranceChallenge; 
                break;
            case RoomType::MagicChallenge: 
                m_gameState = GameState::MagicChallenge; 
                break;
            default:
                // For any other room type, just go back to exploration for now
                m_gameState = GameState::Exploration;
                break;
        }
    } else {
        std::cerr << "Error: Tried to move to a non-existent room ID: " << destinationRoomId << std::endl;
    }
}

void Game::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime);
        render(dataManager.getFont(), m_pulseClock);
    }
}

void Game::processEvents() {
    for (auto event = m_window.pollEvent(); event; event = m_window.pollEvent()) {
        if (m_isAnimating) continue; // Ignore input during animations
        handleInput(*event);
    }
}

void Game::handleInput(sf::Event event) {
    if (event.is<sf::Event::Closed>()) {
        m_window.close();
    }

    UIAction action;
    bool uiHandled = m_uiManager.handleEvent(event, m_gameMode, m_gameState, m_currentRoom, dataManager.getAttunements(), action);
    if (uiHandled) {
        if (action.type == UIActionType::SelectAttunement) {
            const auto& attunements = dataManager.getAttunements();
            auto it = std::find_if(attunements.begin(), attunements.end(), [&](const Attunement& a) {
                return a.id == action.attunementId;
            });
            if (it != attunements.end()) {
                m_player.setAttunement(*it, dataManager);
                startTowerClimb();
            }
        } else if (action.type == UIActionType::ChangeRoom) {
            moveToRoom(action.destinationRoomId);
        } else if (action.type == UIActionType::ChangeRoom) {
            moveToRoom(action.destinationRoomId);
        } else if (action.type == UIActionType::CastSpell) {
            m_board.clearActionStates(); // Clear hints on new action
            const Spell* spell = m_player.castSpell(action.spellIndex);
            if (spell) {
                m_playerActionPerformedThisTurn = true;
                m_timeManager.advanceTime(spell->speedCost, *this);
                std::vector<sf::Vector2i> gemsToRemove;
                // The spell was successfully cast. Process its effects.
                for (const auto& effect : spell->effects) {
                    auto removed = m_effectProcessor.processEffect(*spell, effect, m_player, m_monster, m_board, m_gemFactory, m_timeManager, gemTextures);
                    gemsToRemove.insert(gemsToRemove.end(), removed.begin(), removed.end());
                }

                if (!gemsToRemove.empty()) {
                    m_destroyingGems.insert(gemsToRemove.begin(), gemsToRemove.end());
                    m_isAnimating = true;
                    m_isAnimatingDestruction = true;
                    m_animationClock.restart();
                }

                // Now check if the monster gets a turn.
                if (m_monster.isTurnReady(spell->speedCost)) {
                    m_player.takeDamage(dataManager.getMonsterAttackDamage());
                    showPlayerDamageEffect = true;
                    playerDamageClock.restart();
                }
            }
        }
        return; // UI handled the event
    }

    if (auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left) {
            // Handle click-through for placeholder screens
            if (m_gameState == GameState::Treasure || m_gameState == GameState::Special ||
                m_gameState == GameState::Puzzle || m_gameState == GameState::Trap ||
                m_gameState == GameState::Sanctuary || m_gameState == GameState::AgilityChallenge ||
                m_gameState == GameState::EnduranceChallenge || m_gameState == GameState::MagicChallenge)
            {
                m_gameState = GameState::Exploration;
                return; // Consume the click and do nothing else
            }

            if (m_gameState == GameState::Summary) {
                // Store performance
                switch (m_currentJudgementTrial.type) {
                    case JudgementTrialType::Power:
                        m_trialPerformance.powerScore = m_currentScore;
                        break;
                    case JudgementTrialType::Haste:
                        m_trialPerformance.hasteScore = m_currentScore;
                        break;
                    case JudgementTrialType::Control:
                        m_trialPerformance.controlScore = m_currentScore;
                        break;
                    default: break;
                }

                m_currentTrialOrderIndex++;
                if (m_currentTrialOrderIndex < m_trialOrder.size()) {
                    setupJudgementTrial(m_judgementTrials[m_trialOrder[m_currentTrialOrderIndex]]);
                } else {
                    // All trials complete, transition to the treasure round
                    m_gameState = GameState::Judgement_TreasureRound;
                    setupTreasureRound();
                }
                return; // Consume the click
            }

            int col = (mb->position.x - static_cast<int>(m_boardOrigin.x)) / TILE_SIZE;
            int row = (mb->position.y - static_cast<int>(m_boardOrigin.y)) / TILE_SIZE;

            m_board.clearActionStates();

            if (m_player.hasFreeSwap()) {
                sf::Vector2i clickedGem = sf::Vector2i(col, row);
                if (m_quickSwapFirstSelectedGem.x == -1) {
                    m_quickSwapFirstSelectedGem = clickedGem;
                    m_board.getGemAt(row, col)->setActionState(ActionState::Selected);
                } else {
                    if (m_quickSwapFirstSelectedGem == clickedGem) {
                        m_board.getGemAt(row, col)->setActionState(ActionState::None);
                        m_quickSwapFirstSelectedGem = {-1, -1};
                    } else {
                        m_isAnimating = true;
                        m_isAnimatingSwap = true;
                        m_animatingGems = { m_quickSwapFirstSelectedGem, sf::Vector2i(col, row) };
                        m_animationClock.restart();
                        
                        m_board.getGemAt(m_quickSwapFirstSelectedGem.y, m_quickSwapFirstSelectedGem.x)->setActionState(ActionState::None);
                        m_player.useFreeSwap();
                        m_quickSwapFirstSelectedGem = {-1, -1};
                    }
                }
                return; // Consume the event
            }
            
            // --- Normal Swap Logic ---
            if (m_selectedGem.x == -1) {
                // First gem selection
                m_selectedGem = sf::Vector2i(col, row);
                m_board.getGemAt(row, col)->setActionState(ActionState::Selected);
            } else {
                // Second gem selection
                if (m_board.isAdjacent(sf::Vector2i(m_selectedGem.y, m_selectedGem.x), sf::Vector2i(row, col))) {
                    if (m_board.canSwap(m_selectedGem.y, m_selectedGem.x, row, col)) {
                        m_isAnimating = true;
                        m_isAnimatingSwap = true;
                        m_animatingGems = { m_selectedGem, sf::Vector2i(col, row) };
                        m_animationClock.restart();
                    }
                }
                
                // Always clear selection on second click, whether it was a swap or a cancel
                m_board.getGemAt(m_selectedGem.y, m_selectedGem.x)->setActionState(ActionState::None);
                m_selectedGem = sf::Vector2i(-1, -1);
            }
        }
    }
}



void Game::handleMatches(bool isPlayerMove) {
    auto matchGroups = m_matchDetector.findAllMatches(m_board);

    if (matchGroups.empty()) {
        if (isPlayerMove && !m_isSwappingBack) {
            m_isSwappingBack = true;
            m_animatingGems = { m_animatingGems.second, m_animatingGems.first };
            m_animationClock.restart();
            m_board.swapGems(m_animatingGems.first.y, m_animatingGems.first.x, m_animatingGems.second.y, m_animatingGems.second.x);
        }
        return;
    }

    m_playerActionPerformedThisTurn = true;

    if (isPlayerMove) {
        m_currentTurn++;
        m_timeManager.advanceTime(BASE_SWAP_SPEED, *this);

        if (m_gameMode == GameMode::TOWER_CLIMB && m_monster.isTurnReady(30)) { // Placeholder speed cost for a match
            m_player.takeDamage(dataManager.getMonsterAttackDamage());
            showPlayerDamageEffect = true;
            playerDamageClock.restart();
        }
    }

    std::set<sf::Vector2i, Vector2iCompare> allRemovedGems;
    float totalSkullDamageThisTurn = 0.0f; // New centralized damage accumulator

    for (const auto& match : matchGroups) {
        // Instead of calling resolveMatches, process each gem in the match here
        for (const auto& pos : match) {
            BaseGem* gem = m_board.getGemAt(pos.x, pos.y);
            if (gem) {
                totalSkullDamageThisTurn += gem->onMatch(m_board, m_player, m_monster);
            }
        }

        auto resolutionOpt = m_matchProcessor.process(match, m_board, m_gemFactory, dataManager, gemTextures);
        if (resolutionOpt && *resolutionOpt) {
            // Handle transformations first
            for (const auto& transform : (*resolutionOpt)->gemsToTransform) {
                BaseGem* gem = m_board.getGemAt(transform.first.x, transform.first.y);
                if (gem) {
                    const GemCatalogEntry* newCatalogEntry = dataManager.getGemCatalogEntry(transform.second);
                    if (newCatalogEntry && gemTextures.count(transform.second)) {
                        gem->transform(newCatalogEntry, gemTextures.at(transform.second));
                    }
                }
            }

            // Handle removals
            for (const auto& pos : (*resolutionOpt)->gemsToRemove) {
                allRemovedGems.insert(pos);
            }
        }
    }

    // Apply accumulated damage once
    if (totalSkullDamageThisTurn > 0.0f) {
        int finalDamage = static_cast<int>(totalSkullDamageThisTurn);
        m_monster.takeDamage(finalDamage);
        std::cout << "[COMBAT TURN] Total Damage Applied: " << finalDamage << ". Monster HP after: " << m_monster.getCurrentHp() << std::endl;
    }

    if (allRemovedGems.empty()) return;

    m_currentScore += (allRemovedGems.size() * 100);

    m_isAnimatingSwap = false;
    m_isAnimatingDestruction = true;
    m_destroyingGems = allRemovedGems;
    m_animationClock.restart();
}

void Game::update(sf::Time deltaTime) {
    const float swapAnimationDuration = 0.2f;
    const float destructionAnimationDuration = 0.3f;
    const float refillAnimationDuration = 0.3f;

    if (m_isAnimating) {
        if (m_isAnimatingSwap) {
            if (m_animationClock.getElapsedTime().asSeconds() >= swapAnimationDuration) {
                if (!m_isSwappingBack) {
                    sf::Vector2i p1 = {m_animatingGems.first.y, m_animatingGems.first.x};
                    sf::Vector2i p2 = {m_animatingGems.second.y, m_animatingGems.second.x};

                    m_board.swapGems(p1.x, p1.y, p2.x, p2.y);
                    handleMatches(true); // Player-initiated move
                } else {
                    // Swap-back animation finished
                    m_isAnimating = false;
                    m_isAnimatingSwap = false;
                    m_isSwappingBack = false;
                }
            }
        } else if (m_isAnimatingDestruction) {
            if (m_animationClock.getElapsedTime().asSeconds() >= destructionAnimationDuration) {
                m_isAnimatingDestruction = false;
                
                m_board.removeGems(m_destroyingGems);

                // Corrected Refill Logic
                if (m_gameState == GameState::Judgement_TreasureRound) {
                    m_fallInfo = m_board.applyGravityAndRefill(m_treasureRoundGems);
                } else if (m_gameMode == GameMode::TOWER_CLIMB) {
                    m_fallInfo = m_board.applyGravityAndRefill(m_combatGemPool);
                } else if (m_gameState == GameState::Trial) {
                    // This is the default for the initial Judgement trials (Power, Haste, etc.)
                    m_fallInfo = m_board.applyGravityAndRefill({GemSubType::Fire, GemSubType::Water, GemSubType::Earth, GemSubType::Air});
                }

                m_isAnimatingRefill = true;
                m_animationClock.restart();
            }
        } else if (m_isAnimatingRefill) {
             if (m_animationClock.getElapsedTime().asSeconds() >= refillAnimationDuration) {
                m_isAnimatingRefill = false;
                handleMatches(false); // Cascade-initiated move
                
                // If no new matches were found, end animation cycle
                if (!m_isAnimatingDestruction) {
                    m_isAnimating = false;
                }
            }
        }
        return; 
    }

    if (!m_isAnimating && m_playerActionPerformedThisTurn) {
        m_player.updateEffects(BASE_SWAP_SPEED);
        m_playerActionPerformedThisTurn = false; // Reset for the next turn
    }

    if (showPlayerDamageEffect && playerDamageClock.getElapsedTime().asMilliseconds() > 200) {
        showPlayerDamageEffect = false;
    }

    // Check for Judgement trial win/loss conditions only if not animating AND in a trial
    if (m_gameMode == GameMode::JUDGEMENT && m_gameState == GameState::Trial) {
        if (m_currentScore >= m_currentJudgementTrial.scoreGoal) {
            m_gameState = GameState::Summary; // Win condition
        } else if (m_currentTurn >= m_currentJudgementTrial.turnLimit) {
            m_gameState = GameState::Summary; // Loss condition
        }
    }

    // Check for Treasure Round end condition
    // TODO: Restore this to a higher value (e.g., 20) for the final game.
    if (m_gameState == GameState::Judgement_TreasureRound && m_currentTurn >= 5) {
        int finalTreasureValue = 0;
        for (int r = 0; r < BOARD_HEIGHT; ++r) {
            for (int c = 0; c < BOARD_WIDTH; ++c) {
                BaseGem* gem = m_board.getGemAt(r, c);
                if (gem) {
                    // This is a simplified scoring logic. A more robust solution
                    // would involve checking the gem's catalog entry for its value.
                    switch (gem->getSubType()) {
                        case GemSubType::Coin: finalTreasureValue += 1; break;
                        case GemSubType::CoinPile: finalTreasureValue += 5; break;
                        case GemSubType::CoinBag: finalTreasureValue += 25; break;
                        case GemSubType::CoinBagBundle: finalTreasureValue += 100; break;
                        case GemSubType::TreasureChest: finalTreasureValue += 500; break;
                        default: break;
                    }
                }
            }
        }
        std::cout << "Final Treasure Value: " << finalTreasureValue << std::endl;

        // Transition to the attunement selection screen
        m_gameState = GameState::Judgement_AttunementSelection;
    }

    if (m_gameMode == GameMode::TOWER_CLIMB && m_gameState == GameState::Playing) {
        if (m_player.getHp() <= 0) {
            m_gameState = GameState::GameOver;
        } else if (m_monster.getCurrentHp() <= 0) {
            // clearedRoomIds.insert(m_currentRoom->id); // Mark room as cleared
            m_gameState = GameState::Exploration; // Player wins, go back to exploring
        }
    }

    // Main game logic updates
    switch (m_gameMode) {
        case GameMode::JUDGEMENT:
        {
            m_uiManager.update(m_player, m_monster, m_timeManager, m_gameMode, m_gameState, nullptr, Floor(), {}, dataManager, m_currentJudgementTrial, m_currentScore, m_currentTurn, std::nullopt, m_trialPerformance, m_player.getActiveEffects());
            break;
        }
        case GameMode::TOWER_CLIMB:
            m_uiManager.update(m_player, m_monster, m_timeManager, m_gameMode, m_gameState, m_currentRoom, m_currentFloor, m_visitedRoomIds, dataManager, m_currentJudgementTrial, m_currentScore, m_currentTurn, std::nullopt, m_trialPerformance, m_player.getActiveEffects());
            break;
    }
}

void Game::render(const sf::Font& font, sf::Clock& highlightClock) {
    m_window.clear(sf::Color(30, 30, 30));

    // Only render the board and animations during combat-related states
    if (m_gameState == GameState::Playing || m_gameState == GameState::Trial || m_isAnimating || m_gameState == GameState::GameOver || m_gameState == GameState::Judgement_TreasureRound) {
        // Do not render the board or animations if the trial is over
        if (m_gameState != GameState::Summary && m_gameState != GameState::AttunementReveal) {
            m_board.render(m_window, m_boardOrigin, font, highlightClock, m_timeManager, m_isAnimatingSwap, m_animatingGems, m_isAnimatingDestruction, m_destroyingGems, m_isAnimatingRefill, m_fallInfo, m_effectIconTextures);

            const float swapAnimationDuration = 0.2f;
            const float destructionAnimationDuration = 0.3f;
            const float refillAnimationDuration = 0.3f;
            float p = 0.0f;

            // Render swap animation
            if (m_isAnimatingSwap) {
                p = std::min(1.f, m_animationClock.getElapsedTime().asSeconds() / swapAnimationDuration);
                sf::Vector2f p1_local((float)m_animatingGems.first.x * TILE_SIZE, (float)m_animatingGems.first.y * TILE_SIZE);
                sf::Vector2f p2_local((float)m_animatingGems.second.x * TILE_SIZE, (float)m_animatingGems.second.y * TILE_SIZE);

                BaseGem* g1 = m_board.getGemAt(m_animatingGems.first.y, m_animatingGems.first.x);
                if (g1) {
                    sf::Sprite s1 = g1->getSprite();
                    s1.setPosition(p1_local + (p2_local - p1_local) * p + m_boardOrigin);
                    m_window.draw(s1);
                }

                BaseGem* g2 = m_board.getGemAt(m_animatingGems.second.y, m_animatingGems.second.x);
                if (g2) {
                    sf::Sprite s2 = g2->getSprite();
                    s2.setPosition(p2_local + (p1_local - p2_local) * p + m_boardOrigin);
                    m_window.draw(s2);
                }
            }

            // Render destruction animation
            if (m_isAnimatingDestruction) {
                float animProgress = std::min(1.f, m_animationClock.getElapsedTime().asSeconds() / destructionAnimationDuration);
                float scale = 1.f - animProgress;
                for (const auto& pos : m_destroyingGems) {
                    BaseGem* gem = m_board.getGemAt(pos.x, pos.y);
                    if (gem) {
                        sf::Sprite sprite = gem->getSprite();
                        const sf::Texture* tex = &sprite.getTexture();
                        sf::Vector2u texSize = tex->getSize();
                        sprite.setOrigin({texSize.x / 2.f, texSize.y / 2.f});
                        
                        float baseScaleX = static_cast<float>(TILE_SIZE) / texSize.x;
                        float baseScaleY = static_cast<float>(TILE_SIZE) / texSize.y;
                        sprite.setScale({baseScaleX * scale, baseScaleY * scale});
                        
                        sprite.setPosition({pos.y * TILE_SIZE + TILE_SIZE / 2.f + m_boardOrigin.x, pos.x * TILE_SIZE + TILE_SIZE / 2.f + m_boardOrigin.y});
                        m_window.draw(sprite);
                    }
                }
            }

            // Render refill animation (falling gems)
            if (m_isAnimatingRefill) {
                p = std::min(1.f, m_animationClock.getElapsedTime().asSeconds() / refillAnimationDuration);
                for (const auto& info : m_fallInfo) {
                    BaseGem* gem = m_board.getGemAt(info.fallToRow, info.col);
                    if (gem) {
                        sf::Vector2f start_local((float)info.col * TILE_SIZE, (float)info.startRow * TILE_SIZE);
                        sf::Vector2f end_local((float)info.col * TILE_SIZE, (float)info.fallToRow * TILE_SIZE);
                        
                        sf::Sprite sprite = gem->getSprite();
                        sprite.setPosition(start_local + (end_local - start_local) * p + m_boardOrigin);
                        m_window.draw(sprite);
                    }
                }
            }
        }
    }

    // UI Rendering
    m_uiManager.render(m_window, m_gameMode, m_gameState, showPlayerDamageEffect, m_currentJudgementTrial, m_currentScore, m_currentTurn, std::nullopt, m_trialPerformance, gemTextures, m_effectIconTextures);

    m_window.display();
}

void Game::setupTreasureRound() {
    m_gameState = GameState::Judgement_TreasureRound;
    m_currentTurn = 0;
    m_currentScore = 0; // Reset score for the treasure round
    m_uiManager.setupTreasureRound();

    // The treasure round board needs a few extra gem types to prevent
    // the board initializer from getting stuck in an infinite loop.
    m_treasureRoundGems = { GemSubType::Coin, GemSubType::Coin, GemSubType::Coin, GemSubType::Fire, GemSubType::Water };

    m_board.initialize(m_treasureRoundGems);
}

bool Game::processTreasureMerges() {

    // TODO: Implement the treasure merge logic here.

    return false;

}



void Game::handleTimeEvent(const TimeEvent& event) {
    switch (event.type) {
        case TimeEventType::BurningTile_Activation: {
            BaseGem* gem = m_board.getGemAt(event.coordinates.x, event.coordinates.y);
            if (gem && gem->getStatusEffect() == StatusEffect::Burning) {
                float damage = (m_player.getMaxMana() * 0.10f) * gem->getLevel();
                m_monster.takeDamage(static_cast<int>(damage));
                std::cout << "[EVENT] Burning tile dealt " << static_cast<int>(damage) << " damage. Monster HP: " << m_monster.getCurrentHp() << std::endl;
            }
            break;
        }
        case TimeEventType::BurningTile_Expire: {
            BaseGem* gem = m_board.getGemAt(event.coordinates.x, event.coordinates.y);
            if (gem && gem->getStatusEffect() == StatusEffect::Burning) {
                float damage = (m_player.getMaxMana() * 0.10f) * gem->getLevel();
                m_monster.takeDamage(static_cast<int>(damage));
                std::cout << "[EVENT] Burning tile dealt final " << static_cast<int>(damage) << " damage and expired. Monster HP: " << m_monster.getCurrentHp() << std::endl;
                
                // Add the gem to be destroyed
                m_destroyingGems.insert(event.coordinates);
                m_isAnimating = true;
                m_isAnimatingDestruction = true;
                m_animationClock.restart();
            }
            break;
        }
    }
}
