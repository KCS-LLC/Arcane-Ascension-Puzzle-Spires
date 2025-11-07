#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <set>
#include "Player.h"
#include "Monster.h"
#include "Constants.h"

struct Room; // Forward declaration
class DataManager; // Forward declaration

// A structure to represent a command sent from the UI to the Game
enum class UIActionType { CastSpell, ChangeRoom };
struct UIAction {
    UIActionType type;
    int spellIndex = -1; // Used for CastSpell
    int destinationRoomId = -1; // Used for ChangeRoom
};

class UIManager {
public:
    UIManager(const sf::Font& font);

    // Returns true if the event was handled (consumed) by the UI, false otherwise.
    // If a specific action is needed (like a button click), it's returned via the outAction parameter.
    bool handleEvent(const sf::Event& event, GameState currentState, const Room* currentRoom, UIAction& outAction);

    void setup(const Player& player, const sf::Vector2u& windowSize, const sf::Vector2f& boardOrigin);
    void update(const Player& player, const Monster& monster, GameState currentState, const Room* currentRoom, const std::set<int>& visitedRoomIds, const DataManager& dataManager);
    void render(sf::RenderWindow& window, GameState currentState, bool showPlayerDamageEffect);

    const std::vector<sf::RectangleShape>& getSpellButtons() const;

private:
    sf::Font font;

    // Titles
    sf::Text playerPanelTitle;
    sf::Text monsterPanelTitle;
    sf::Text monsterNameText;
    sf::Text manaTitle;

    // Gauges
    sf::RectangleShape playerHpBarBack;
    sf::RectangleShape playerHpBarFront;
    sf::RectangleShape monsterHpBarBack;
    sf::RectangleShape monsterHpBarFront;
    std::map<GemType, sf::RectangleShape> manaBarBacks;
    std::map<GemType, sf::RectangleShape> manaBarFronts;

    sf::Text gameOverText;
    std::vector<sf::RectangleShape> spellButtons;
    std::vector<sf::RectangleShape> spellButtonFills; // To show mana progress
    std::vector<sf::Text> spellButtonTexts;
    sf::RectangleShape leftPanel;
    sf::RectangleShape rightPanel;
    sf::RectangleShape boardFrame;
    sf::RectangleShape monsterSpeedGaugeBackground;
    sf::RectangleShape monsterSpeedGaugeForeground;
    sf::RectangleShape playerDamageOverlay;

    // Exploration Screen
    sf::Text explorationTitle;
    sf::Text treasureTitle;
    sf::Text specialTitle;
    sf::Text puzzleTitle;
    sf::Text trapTitle;
    sf::Text sanctuaryTitle;

    // Room Exits
    std::vector<sf::RectangleShape> doorButtons;
    std::vector<sf::Text> doorButtonTexts;
};

#endif // UIMANAGER_H
