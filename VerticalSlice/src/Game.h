
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <map>
#include <memory>
#include "Board.h"
#include "Player.h"
#include "Monster.h"
#include "DataManager.h"
#include "UIManager.h"
#include "Constants.h"
#include "SpireData.h"

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
    void applyMatchConsequences(const std::map<GemType, int>& matchResults);
    void moveToRoom(int destinationRoomId);

    sf::RenderWindow window;
    Board board;
    Player player;
    Monster monster;
    DataManager dataManager;
    std::unique_ptr<UIManager> uiManager;
    std::map<GemType, sf::Texture> gemTextures;

    Floor currentFloor;
#include <set>

// ... existing class Game ...
    const Room* currentRoom;
    std::vector<int> roomHistory;
    std::set<int> visitedRoomIds;
    std::set<int> clearedRoomIds;

    // Game State
    GameState currentState;
    sf::Clock animationClock;

    bool isAnimatingSwap;
    bool isAnimatingDestruction;
    bool isAnimatingRefill;
    bool isReshuffling;
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
