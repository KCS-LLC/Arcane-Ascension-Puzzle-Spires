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
      m_gameMode(GameMode::Judgement),
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
    if (auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left) {
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
                    m_gameState = GameState::AttunementReveal; // All trials finished
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

void Game::resolveMatches(const std::set<std::pair<int, int>>& matches) {
    for (const auto& pos : matches) {
        BaseGem* gem = m_board.getGemAt(pos.first, pos.second);
        if (gem) {
            gem->onMatch(m_board, m_player, m_monster);
        }
    }
}

void Game::update(sf::Time deltaTime) {
    const float swapAnimationDuration = 0.2f;
    const float destructionAnimationDuration = 0.3f;
    const float refillAnimationDuration = 0.3f;

    if (m_isAnimating) {
        if (m_isAnimatingSwap) {
            if (m_animationClock.getElapsedTime().asSeconds() >= swapAnimationDuration) {
                sf::Vector2i p1 = {m_animatingGems.first.y, m_animatingGems.first.x};
                sf::Vector2i p2 = {m_animatingGems.second.y, m_animatingGems.second.x};

                m_board.swapGems(p1.x, p1.y, p2.x, p2.y);
                auto matches = m_board.findMatches();
                if (!matches.empty()) { // Swap resulted in a match
                    resolveMatches(matches);
                    m_currentTurn++; // Increment turn after a valid move
                    m_currentScore += (matches.size() * 100); // Add score for destroyed gems
                    m_isAnimatingSwap = false;
                    m_isAnimatingDestruction = true;
                    m_destroyingGems = matches;
                    m_animationClock.restart();
                } else { // Invalid swap, animate back
                    m_animatingGems = { m_animatingGems.second, m_animatingGems.first };
                    m_animationClock.restart();
                    m_board.swapGems(p1.x, p1.y, p2.x, p2.y); // Swap back immediately data-wise
                    // Reset to a non-animating state after the swap-back animation finishes
                    if (m_animationClock.getElapsedTime().asSeconds() >= swapAnimationDuration) {
                       m_isAnimating = false;
                       m_isAnimatingSwap = false;
                    }
                }
            }
        } else if (m_isAnimatingDestruction) {
            if (m_animationClock.getElapsedTime().asSeconds() >= destructionAnimationDuration) {
                m_isAnimatingDestruction = false;
                m_board.removeGems(m_destroyingGems);
                m_fallInfo = m_board.applyGravity();
                m_board.refill({GemSubType::Fire, GemSubType::Water, GemSubType::Earth, GemSubType::Air});
                m_isAnimatingRefill = true;
                m_animationClock.restart();
            }
        } else if (m_isAnimatingRefill) {
             if (m_animationClock.getElapsedTime().asSeconds() >= refillAnimationDuration) {
                auto newMatches = m_board.findMatches(); // Check for cascades
                if (!newMatches.empty()) {
                    resolveMatches(newMatches);
                    m_isAnimatingRefill = false;
                    m_isAnimatingDestruction = true; // Chain into another destruction
                    m_destroyingGems = newMatches;
                    m_animationClock.restart();
                } else {
                    m_isAnimating = false;
                    m_isAnimatingRefill = false;
                }
            }
        }
        return; 
    }

    // Check for Judgement trial win/loss conditions only if not animating
    if (m_gameMode == GameMode::Judgement && m_gameState == GameState::Trial) {
        std::cout << "Update Check - Score: " << m_currentScore << "/" << m_currentJudgementTrial.scoreGoal
                  << ", Turn: " << m_currentTurn << "/" << m_currentJudgementTrial.turnLimit << std::endl;

        if (m_currentScore >= m_currentJudgementTrial.scoreGoal) {
            std::cout << "!!! STATE CHANGE: Win condition met. Changing to Summary." << std::endl;
            m_gameState = GameState::Summary; // Win condition
        } else if (m_currentTurn >= m_currentJudgementTrial.turnLimit) {
            std::cout << "!!! STATE CHANGE: Loss condition met. Changing to Summary." << std::endl;
            m_gameState = GameState::Summary; // Loss condition
        }
    }

    // Main game logic updates
    switch (m_gameMode) {
        case GameMode::Judgement:
        {
            m_uiManager.update(m_player, m_monster, m_gameState, nullptr, {}, dataManager, m_currentJudgementTrial, m_currentScore, m_currentTurn, std::nullopt, m_trialPerformance);
            break;
        }
        case GameMode::Exploration:
            // Exploration-specific update logic here
            break;
        case GameMode::Combat:
            // Combat-specific update logic here
            break;
    }
}

void Game::render() {
    m_window.clear(sf::Color(30, 30, 30));

    // Do not render the board or animations if the trial is over
    if (m_gameState != GameState::Summary && m_gameState != GameState::AttunementReveal) {
        std::cout << "Game::render - Calling m_board.render()" << std::endl;
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
                BaseGem* gem = m_board.getGemAt(pos.first, pos.second);
                if (gem) {
                    sf::Sprite sprite = gem->getSprite();
                    const sf::Texture* tex = &sprite.getTexture();
                    sf::Vector2u texSize = tex->getSize();
                    sprite.setOrigin({texSize.x / 2.f, texSize.y / 2.f});
                    
                    float baseScaleX = static_cast<float>(TILE_SIZE) / texSize.x;
                    float baseScaleY = static_cast<float>(TILE_SIZE) / texSize.y;
                    sprite.setScale({baseScaleX * scale, baseScaleY * scale});
                    
                    sprite.setPosition({pos.second * TILE_SIZE + TILE_SIZE / 2.f + m_boardOrigin.x, pos.first * TILE_SIZE + TILE_SIZE / 2.f + m_boardOrigin.y});
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

    // UI Rendering
    m_uiManager.render(m_window, m_gameState, false, m_currentJudgementTrial, m_currentScore, m_currentTurn, std::nullopt, m_trialPerformance);

    m_window.display();
}