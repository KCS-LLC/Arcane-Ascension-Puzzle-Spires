#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.h"
#include "Monster.h"
#include "Constants.h"

class UIManager {
public:
    UIManager();

    void setup(const sf::Font& font, const Player& player, const sf::Vector2u& windowSize, const sf::Vector2f& boardOrigin);
    void update(const Player& player, const Monster& monster);
    void render(sf::RenderWindow& window, GameState currentState);

    const std::vector<sf::RectangleShape>& getSpellButtons() const;

private:
    sf::Font font;
    sf::Text playerHpText;
    sf::Text monsterHpText;
    sf::Text manaText;
    sf::Text gameOverText;
    std::vector<sf::RectangleShape> spellButtons;
    std::vector<sf::Text> spellButtonTexts;
    sf::RectangleShape leftPanel;
    sf::RectangleShape rightPanel;
    sf::RectangleShape boardFrame;
    sf::RectangleShape monsterSpeedGaugeBackground;
    sf::RectangleShape monsterSpeedGaugeForeground;
};

#endif // UIMANAGER_H
