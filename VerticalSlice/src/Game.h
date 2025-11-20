#ifndef GAME_H
#define GAME_H

#include "Gem.h" // Use the full Gem definition
#include "Board.h"
#include "Player.h"
#include "Monster.h"
#include "DataManager.h"
#include "SpireData.h"
#include "Judgement.h"

// Forward declarations
class UIManager;

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
    void loadTextures();
    void handleSwap(sf::Vector2i tile1, sf::Vector2i tile2);
    void applyMatchConsequences(const std::vector<Gem>& matchResults);
    void moveToRoom(int destinationRoomId);
    void startNewCombat();
    void initializeJudgement();
    void startNextJudgementTrial();

    sf::RenderWindow window;
    Board board;
    Player player;
    Monster monster;
    DataManager dataManager;
    std::unique_ptr<UIManager> uiManager;
    std::map<GemSubType, sf::Texture> gemTextures;

    Floor currentFloor;
    const Room* currentRoom;
    std::vector<int> roomHistory;
    std::set<int> visitedRoomIds;
    std::set<int> clearedRoomIds;

    // Game State
    GameState currentState;
    sf::Clock animationClock;

    // Judgement State
    JudgementResults m_judgementResults;
    std::vector<JudgementTrial> m_pendingJudgementTrials;
    JudgementTrial m_currentJudgementTrial;
    int m_currentTrialTurn = 0;
    int m_currentScore = 0;
    std::vector<Gem> m_matchedGemsInTurn;

    bool isAnimatingSwap;
    bool isAnimatingDestruction;
    bool isAnimatingRefill;
    bool isReshuffling;
    bool isRevertingInvalidSwap;
    std::pair<sf::Vector2i, sf::Vector2i> animatingGems;
    std::set<std::pair<int, int>> destroyingGems;
    std::vector<Board::FallInfo> fallInfo;

    std::optional<sf::Vector2i> selectedTile;
    std::optional<sf::Vector2i> dragStartTile;

    sf::Font font;
    sf::Vector2f boardOrigin;

    sf::Clock playerDamageClock;
    bool showPlayerDamageEffect = false;
};

#endif // GAME_H
