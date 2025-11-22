#pragma once

#include <SFML/Graphics.hpp>
#include "Board.h"
#include "DataManager.h"
#include "Player.h"
#include "Monster.h"
#include "UIManager.h"
#include "Constants.h"
#include "GemFactory.h"
#include "Structs.h"

// Forward-declaration of the global texture map
extern std::map<GemSubType, sf::Texture> gemTextures;

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void loadTextures();
    void handleInput(sf::Event event);
    void resolveMatches(const std::set<std::pair<int, int>>& matches);
    void setupJudgementTrial(const JudgementTrial& trial);

    sf::RenderWindow m_window;
    DataManager dataManager;
    Player m_player;
    Monster m_monster;
    UIManager m_uiManager;
    GemFactory m_gemFactory;
    Board m_board;

    GameMode m_gameMode;
    GameState m_gameState;
    
    // Judgement trial state
    std::vector<JudgementTrial> m_judgementTrials;
    int m_currentTrialIndex;
    JudgementTrial m_currentJudgementTrial;
    sf::Clock m_trialClock;
    sf::Clock m_pulseClock;
    // Game Stats
    int m_currentTurn;
    int m_currentScore;

    // Gem selection
    sf::Vector2i m_selectedGem = sf::Vector2i(-1, -1);

    // Animation state
    bool m_isAnimating = false;
    bool m_isAnimatingSwap = false;
    bool m_isAnimatingDestruction = false;
    bool m_isAnimatingRefill = false;
    sf::Clock m_animationClock;
    std::pair<sf::Vector2i, sf::Vector2i> m_animatingGems;
    std::set<std::pair<int, int>> m_destroyingGems;
    std::vector<Board::FallInfo> m_fallInfo;

    // Board positioning
    sf::Vector2f m_boardOrigin;
};