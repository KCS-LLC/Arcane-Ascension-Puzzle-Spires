#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include "Player.h"
#include "Monster.h"
#include "Constants.h"

// A structure to represent a command sent from the UI to the Game
enum class UIActionType { CastSpell };
struct UIAction {
    UIActionType type;
    int spellIndex; 
};

class UIManager {
public:
    UIManager(const sf::Font& font);

    // Takes an event and returns an action if a UI element was interacted with
    std::optional<UIAction> handleEvent(const sf::Event& event);

    void setup(const Player& player, const sf::Vector2u& windowSize, const sf::Vector2f& boardOrigin);
    void update(const Player& player, const Monster& monster);
    void render(sf::RenderWindow& window, GameState currentState);

    const std::vector<sf::RectangleShape>& getSpellButtons() const;

private:
    sf::Font font;

    // Titles
    sf::Text playerPanelTitle;
    sf::Text monsterPanelTitle;
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
};

#endif // UIMANAGER_H
