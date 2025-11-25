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
    : m_window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Judgement"),
      dataManager(),
      m_uiManager(dataManager.getFont()),
      m_gemFactory(dataManager),
      m_board(BOARD_WIDTH, BOARD_HEIGHT, m_gemFactory),
      m_player(100, {}),
      m_monster(10, 10),
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
                m_monster = Monster(dataManager.getMonsterHP(), dataManager.getMonsterSpeed()); // Reset monster for new combat
                m_board.initialize({GemSubType::Fire, GemSubType::Water, GemSubType::Earth, GemSubType::Air, GemSubType::Skull}); // Basic board for now
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
        render();
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
                m_gameMode = GameMode::TOWER_CLIMB;
                // TODO: Set up the first combat of the tower climb
            }
        } else if (action.type == UIActionType::ChangeRoom) {
            moveToRoom(action.destinationRoomId);
        } else if (action.type == UIActionType::ChangeRoom) {
            moveToRoom(action.destinationRoomId);
        } else if (action.type == UIActionType::CastSpell) {
            int damage = m_player.castSpell(action.spellIndex);
            if (damage > 0) {
                m_monster.takeDamage(damage);
                if (m_monster.isTurnReady(70)) { // 70 is placeholder for SPELL_SPEED_COST
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

            if (m_selectedGem.x == -1) {
                m_selectedGem = sf::Vector2i(col, row);
            } else {
                if (m_board.canSwap(m_selectedGem.y, m_selectedGem.x, row, col)) {
                    m_isAnimating = true;
                    m_isAnimatingSwap = true;
                    m_animatingGems = { m_selectedGem, sf::Vector2i(col, row) };
                    m_animationClock.restart();
                }
                m_selectedGem = sf::Vector2i(-1, -1);
            }
        }
    }
}

void Game::resolveMatches(const std::vector<sf::Vector2i>& matches) {
    for (const auto& pos : matches) {
        BaseGem* gem = m_board.getGemAt(pos.x, pos.y);
        if (gem) {
            gem->onMatch(m_board, m_player, m_monster);
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

    if (isPlayerMove) {
        m_currentTurn++;
    }

    std::set<sf::Vector2i, Vector2iCompare> allRemovedGems;
    for (const auto& match : matchGroups) {
        resolveMatches(match); 

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

    if (allRemovedGems.empty()) return;

    m_currentScore += (allRemovedGems.size() * 100);

    if (m_gameMode == GameMode::TOWER_CLIMB && m_monster.isTurnReady(30)) { // Placeholder speed cost
        m_player.takeDamage(dataManager.getMonsterAttackDamage());
        showPlayerDamageEffect = true;
        playerDamageClock.restart();
    }

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
                    // This will be used for standard combat once implemented
                    m_fallInfo = m_board.applyGravityAndRefill({GemSubType::Fire, GemSubType::Water, GemSubType::Earth, GemSubType::Air, GemSubType::Skull});
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
    if (m_gameState == GameState::Judgement_TreasureRound && m_currentTurn >= 20) {
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

        // Assign attunement before starting the tower climb
        const auto& attunements = dataManager.getAttunements();
        auto it = std::find_if(attunements.begin(), attunements.end(), [&](const Attunement& a) {
            return a.id == "elementalist";
        });

        if (it != attunements.end()) {
            m_player.setAttunement(*it, dataManager);
        } else {
            // Handle error case where elementalist attunement is not found
            std::cerr << "CRITICAL: Elementalist attunement not found!" << std::endl;
            m_window.close();
        }

        startTowerClimb();
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
            m_uiManager.update(m_player, m_monster, m_gameMode, m_gameState, nullptr, Floor(), {}, dataManager, m_currentJudgementTrial, m_currentScore, m_currentTurn, std::nullopt, m_trialPerformance);
            break;
        }
        case GameMode::TOWER_CLIMB:
            m_uiManager.update(m_player, m_monster, m_gameMode, m_gameState, m_currentRoom, m_currentFloor, m_visitedRoomIds, dataManager, m_currentJudgementTrial, m_currentScore, m_currentTurn, std::nullopt, m_trialPerformance);
            break;
    }
}

void Game::render() {
    m_window.clear(sf::Color(30, 30, 30));

    // Only render the board and animations during combat-related states
    if (m_gameState == GameState::Playing || m_gameState == GameState::Trial || m_isAnimating || m_gameState == GameState::GameOver || m_gameState == GameState::Judgement_TreasureRound) {
        // Do not render the board or animations if the trial is over
        if (m_gameState != GameState::Summary && m_gameState != GameState::AttunementReveal) {
            m_board.render(m_window, m_boardOrigin, m_isAnimatingSwap, m_animatingGems, m_isAnimatingDestruction, m_destroyingGems, m_isAnimatingRefill, m_fallInfo);

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
    m_uiManager.render(m_window, m_gameMode, m_gameState, showPlayerDamageEffect, m_currentJudgementTrial, m_currentScore, m_currentTurn, std::nullopt, m_trialPerformance);

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