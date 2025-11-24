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
#include "Judgement.h"
#include "MatchDetector.h"
#include "MatchProcessor.h"

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
    void resolveMatches(const std::vector<sf::Vector2i>& matches);
    void handleMatches(bool isPlayerMove);
    void setupJudgementTrial(const JudgementTrial& trial);
    void startTowerClimb();
    void setupTreasureRound();
    bool processTreasureMerges();
    void moveToRoom(int destinationRoomId);

    sf::RenderWindow m_window;
    DataManager dataManager;
    UIManager m_uiManager;
    GemFactory m_gemFactory;
        Board m_board;
    Player m_player;
    Monster m_monster;
    MatchDetector m_matchDetector;
    MatchProcessor m_matchProcessor;

    GameMode m_gameMode;
    GameState m_gameState;
    
    // Tower Climb / Exploration state
    Floor m_currentFloor;
    const Room* m_currentRoom = nullptr;
    std::set<int> m_visitedRoomIds;
    std::vector<JudgementTrial> m_judgementTrials; // Keep as vector of objects for ownership
    std::vector<int> m_trialOrder; // Stores the shuffled indices
    int m_currentTrialOrderIndex; // Tracks which trial we are on in the shuffled order
    JudgementTrial m_currentJudgementTrial;
    TrialPerformance m_trialPerformance;
    std::vector<GemSubType> m_treasureRoundGems; // Gems used in the treasure round
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
    bool m_isSwappingBack = false; // Flag for the reverse animation
    bool m_isAnimatingDestruction = false;
    bool m_isAnimatingRefill = false;
    sf::Clock m_animationClock;
    std::pair<sf::Vector2i, sf::Vector2i> m_animatingGems;
    std::set<sf::Vector2i, Vector2iCompare> m_destroyingGems;
    std::vector<Board::FallInfo> m_fallInfo;

    // Combat UI state
    bool showPlayerDamageEffect = false;
    sf::Clock playerDamageClock;

    // Board positioning
    sf::Vector2f m_boardOrigin;
};