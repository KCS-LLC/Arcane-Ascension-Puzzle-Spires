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

// Global texture map, defined here
std::map<GemSubType, sf::Texture> gemTextures;

Game::Game()
    : m_window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Judgement"),
      dataManager(),
      m_player(100, {}),
      m_monster(10, 10),
      m_uiManager(dataManager.getFont()),
      m_gemFactory(dataManager),
      m_board(BOARD_WIDTH, BOARD_HEIGHT, m_gemFactory),
      m_gameState(GameState::Judgement_Intro),
      m_currentTrialIndex(0)
{
    m_window.setFramerateLimit(60);
    loadTextures();

    m_judgementTrials = dataManager.getJudgementTrials();
    if (!m_judgementTrials.empty()) {
        setupJudgementTrial(m_judgementTrials[0]);
    }
}

void Game::loadTextures() {
    // Load textures based on the GemCatalogEntry in DataManager
    // This will require an iterator over the m_gemCatalog map
    // For now, let's just load a few known textures to get it compiling
    // We'll need a way to get the GemCatalog from DataManager. This will be added later.

    // Placeholder: Need to get actual texture paths from DataManager's gem catalog
    gemTextures[GemSubType::Fire].loadFromFile("assets/gem_fire.png");
    gemTextures[GemSubType::Water].loadFromFile("assets/gem_water.png");
    gemTextures[GemSubType::Earth].loadFromFile("assets/gem_earth.png");
    gemTextures[GemSubType::Air].loadFromFile("assets/gem_air.png");
    gemTextures[GemSubType::Light].loadFromFile("assets/gem_light.png");
    gemTextures[GemSubType::Life].loadFromFile("assets/gem_life.png");
    gemTextures[GemSubType::Enhancement].loadFromFile("assets/gem_enhancement.png");
    gemTextures[GemSubType::Transference].loadFromFile("assets/gem_transference.png");
    gemTextures[GemSubType::Death].loadFromFile("assets/gem_death.png");
    gemTextures[GemSubType::Umbral].loadFromFile("assets/gem_umbral.png");
    gemTextures[GemSubType::Mental].loadFromFile("assets/gem_mental.png");
    gemTextures[GemSubType::Perception].loadFromFile("assets/gem_perception.png");
    gemTextures[GemSubType::Coin].loadFromFile("assets/gem_coin.png");
    gemTextures[GemSubType::Skull].loadFromFile("assets/gem_skull.png");
    gemTextures[GemSubType::Raw].loadFromFile("assets/gem_raw.png");
    gemTextures[GemSubType::Grey].loadFromFile("assets/gem_raw.png"); // Placeholder for Grey
}

void Game::setupJudgementTrial(const JudgementTrial& trial) {
    m_currentJudgementTrial = trial;
    // m_uiManager.setupTrial(trial); // Will need to re-enable after UIManager is updated
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
        handleInput(*event);
    }
}

void Game::handleInput(sf::Event event) {
    if (event.is<sf::Event::Closed>()) {
        m_window.close();
    }
    if (auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left) {
            int col = mb->position.x / TILE_SIZE;
            int row = mb->position.y / TILE_SIZE;

            if (m_selectedGem.x == -1) {
                m_selectedGem = sf::Vector2i(col, row);
            } else {
                if (m_board.canSwap(m_selectedGem.y, m_selectedGem.x, row, col)) {
                    m_board.swapGems(m_selectedGem.y, m_selectedGem.x, row, col);
                    auto matches = m_board.findMatches();
                    if (!matches.empty()) {
                        resolveMatches(matches);
                    } else {
                        // Invalid swap, swap back
                        m_board.swapGems(m_selectedGem.y, m_selectedGem.x, row, col);
                    }
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
    m_board.removeGems(matches);
    m_board.applyGravity();
    // Default gems for refill, as trial.gems is removed
    m_board.refill({GemSubType::Fire, GemSubType::Water, GemSubType::Earth, GemSubType::Air});
}

void Game::update(sf::Time deltaTime) {
    if (m_gameState == GameState::Judgement_Trial) {
        float elapsed = m_trialClock.getElapsedTime().asSeconds();

        JudgementResults results; // Placeholder for now
        std::set<int> visitedRoomIds; // Placeholder

        // m_uiManager.update(m_player, m_monster, m_gameState, nullptr, visitedRoomIds, dataManager, m_currentJudgementTrial, 0, 0, std::nullopt, results); // Re-enable later
    }
}

void Game::render() {
    m_window.clear(sf::Color(30, 30, 30));
    
    m_board.render(m_window);

    JudgementResults results; // Placeholder for now
    
    // m_uiManager.render(m_window, m_gameState, m_gameState, false, m_currentJudgementTrial, 0, 0, std::nullopt, results); // Re-enable later

    if (m_currentJudgementTrial.type == JudgementTrialType::Control) {
         float alpha = 128 + 127 * std::sin(m_pulseClock.getElapsedTime().asSeconds() * 5);
         for (int r = 0; r < BOARD_HEIGHT; ++r) {
            for (int c = 0; c < BOARD_WIDTH; ++c) {
                BaseGem* gem = m_board.getGemAt(r, c);
                if (gem && gem->getSubType() == GemSubType::Transference) {
                    sf::Sprite& sprite = gem->getSprite();
                    sprite.setColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(alpha)));
                }
            }
        }
    }
    m_window.display();
}