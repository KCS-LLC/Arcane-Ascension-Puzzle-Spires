#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <set>
#include "Player.h"
#include "Monster.h"
#include "Constants.h"
#include "Structs.h"

struct Room; // Forward declaration
class DataManager; // Forward declaration

// A structure to represent a command sent from the UI to the Game
enum class UIActionType { CastSpell, ChangeRoom, SelectAttunement };
struct UIAction {
    UIActionType type;
    int spellIndex = -1;
    int destinationRoomId = -1;
    std::string attunementId;
};

class UIManager {
public:
    UIManager(const sf::Font& font);

    bool handleEvent(const sf::Event& event, GameState currentState, const Room* currentRoom, const std::vector<Attunement>& attunements, UIAction& outAction);
    void setup(const Player& player, const sf::Vector2u& windowSize, const sf::Vector2f& boardOrigin, const std::vector<Attunement>& attunements);
    void update(const Player& player, const Monster& monster, GameState currentState, const Room* currentRoom, const std::set<int>& visitedRoomIds, const DataManager& dataManager);
    void render(sf::RenderWindow& window, GameState currentState, bool showPlayerDamageEffect);

    const std::vector<sf::RectangleShape>& getSpellButtons() const;

private:
    sf::Font font;

    // Judgement Screen
    sf::Text judgementTitle;
    std::vector<sf::RectangleShape> attunementButtons;
    std::vector<sf::Text> attunementButtonTexts;

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
    sf::Text agilityTitle;
    sf::Text enduranceTitle;
    sf::Text magicTitle;

    // Room Exits
    std::vector<sf::RectangleShape> doorButtons;
    std::vector<sf::Text> doorButtonTexts;
};

#endif // UIMANAGER_H
