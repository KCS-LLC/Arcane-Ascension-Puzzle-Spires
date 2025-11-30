#pragma once

#include <SFML/Graphics.hpp>
// removed <memory> include
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
#include "EffectProcessor.h"
#include "TimeManager.h"

// Forward-declaration of the global texture map
extern std::map<GemSubType, sf::Texture> gemTextures;

class Game {
public:
    Game();
    void run();
    void handleTimeEvent(const TimeEvent& event);
    void setBoardStateDirty(bool isDirty);

    // Getters for game components
    Player& getPlayer();
    Monster& getMonster();
    Board& getBoard();
    GemFactory& getGemFactory();
    TimeManager& getTimeManager();
    void startTargeting(const TargetingData& targetingData);
    void startTransformAnimation(const std::vector<sf::Vector2i>& gemsToTransform);
private:
    PlayMode m_playMode = PlayMode::Normal;
    const Spell* m_pendingSpell = nullptr;
    TargetingRequest m_targetingRequest;
    std::vector<sf::Vector2i> m_targetingSelections;

    void resolveTargeting();
    void cancelTargeting();
    bool m_boardStateDirty = false;
    void processEvents();
    void update(sf::Time deltaTime);
    void render(const sf::Font& font, sf::Clock& pulseClock);
    void loadTextures();
    void handleInput(sf::Event event);
    void resolveMatches(const std::vector<sf::Vector2i>& matches);
    void handleMatches(bool isPlayerMove);
    void setupJudgementTrial(const JudgementTrial& trial);
    void startTowerClimb();
    void setupTreasureRound();
    bool processTreasureMerges();
    void moveToRoom(int destinationRoomId);
    void unloadBoard();

    sf::RenderWindow m_window;
    DataManager dataManager; // Must be initialized before managers that use it.
    UIManager m_uiManager;
    TimeManager m_timeManager;
    GemFactory m_gemFactory;
    Board m_board;
    Player m_player;
    Monster m_monster;
    MatchDetector m_matchDetector;
    MatchProcessor m_matchProcessor;
    EffectProcessor m_effectProcessor;

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
    std::vector<GemSubType> m_combatGemPool; // Stores the dynamic gem pool for the current combat
    sf::Clock m_trialClock;
    sf::Clock m_pulseClock;
    // Game Stats
    int m_currentTurn;
    int m_currentScore;
    bool m_playerActionPerformedThisTurn = false;

    // Gem selection
    sf::Vector2i m_selectedGem = sf::Vector2i(-1, -1);

    // Animation state
    bool m_isAnimating = false;
    bool m_isAnimatingSwap = false;
    bool m_isSwappingBack = false; // Flag for the reverse animation
    bool m_isAnimatingDestruction = false;
    bool m_isAnimatingRefill = false;
    bool m_isAnimatingRowRotation = false;
    bool m_isAnimatingColumnRotation = false;
    bool m_isAnimatingTransform = false;
    int m_rotatingRow;
    int m_rotatingColumn;
    int m_rotationDirection;
    sf::Clock m_animationClock;
    std::pair<sf::Vector2i, sf::Vector2i> m_animatingGems;
    std::set<sf::Vector2i, Vector2iCompare> m_destroyingGems;
    std::vector<Board::FallInfo> m_fallInfo;
    std::vector<sf::Vector2i> m_transformingGems;

    // Combat UI state
    bool showPlayerDamageEffect = false;
    sf::Clock playerDamageClock;
    std::map<std::string, sf::Texture> m_effectIconTextures;

    // Board positioning
    sf::Vector2f m_boardOrigin;

    // Quick Swap state
    sf::Vector2i m_quickSwapFirstSelectedGem = {-1, -1};
};