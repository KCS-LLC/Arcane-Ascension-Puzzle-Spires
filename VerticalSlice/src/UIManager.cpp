#include "UIManager.h"
#include <string>

UIManager::UIManager() 
    : font(),
      playerHpText(font),
      monsterHpText(font),
      manaText(font),
      gameOverText(font)
{}

void UIManager::setup(const sf::Font& fontRef, const Player& player, const sf::Vector2u& windowSize, const sf::Vector2f& boardOrigin) {
    font = fontRef;

    // --- UI Layout Calculations ---
    const int boardPixelWidth = BOARD_WIDTH * TILE_SIZE;
    const float panelWidth = (windowSize.x - boardPixelWidth) / 2.f - 40;

    leftPanel.setSize({panelWidth, (float)windowSize.y - 40});
    leftPanel.setPosition({20, 20});
    leftPanel.setFillColor(sf::Color(30, 30, 30, 200));
    leftPanel.setOutlineColor(sf::Color(80, 80, 80));
    leftPanel.setOutlineThickness(2.f);

    rightPanel.setSize({panelWidth, (float)windowSize.y - 40});
    rightPanel.setPosition({(float)windowSize.x - panelWidth - 20, 20});
    rightPanel.setFillColor(sf::Color(30, 30, 30, 200));
    rightPanel.setOutlineColor(sf::Color(80, 80, 80));
    rightPanel.setOutlineThickness(2.f);

    boardFrame.setSize({(float)(BOARD_WIDTH * TILE_SIZE) + 4, (float)(BOARD_HEIGHT * TILE_SIZE) + 4});
    boardFrame.setPosition({boardOrigin.x - 2, boardOrigin.y - 2});
    boardFrame.setFillColor(sf::Color::Transparent);
    boardFrame.setOutlineColor(sf::Color(80, 80, 80));
    boardFrame.setOutlineThickness(2.f);

    monsterSpeedGaugeBackground.setSize({rightPanel.getSize().x - 40, 20});
    monsterSpeedGaugeBackground.setPosition({rightPanel.getPosition().x + 20, rightPanel.getPosition().y + 80});
    monsterSpeedGaugeBackground.setFillColor(sf::Color(50, 50, 50));
    monsterSpeedGaugeBackground.setOutlineColor(sf::Color(80, 80, 80));
    monsterSpeedGaugeBackground.setOutlineThickness(1.f);

    monsterSpeedGaugeForeground.setSize({0, 20});
    monsterSpeedGaugeForeground.setPosition({rightPanel.getPosition().x + 20, rightPanel.getPosition().y + 80});
    monsterSpeedGaugeForeground.setFillColor(sf::Color::Cyan);

    // --- Text and Button Setup ---
    playerHpText.setFont(font);
    playerHpText.setCharacterSize(24);
    playerHpText.setFillColor(sf::Color::White);
    playerHpText.setPosition({leftPanel.getPosition().x + 20, leftPanel.getPosition().y + 20});

    monsterHpText.setFont(font);
    monsterHpText.setCharacterSize(24);
    monsterHpText.setFillColor(sf::Color::White);
    monsterHpText.setPosition({rightPanel.getPosition().x + 20, rightPanel.getPosition().y + 20});

    manaText.setFont(font);
    manaText.setCharacterSize(22);
    manaText.setFillColor(sf::Color::White);
    manaText.setPosition({leftPanel.getPosition().x + 20, leftPanel.getPosition().y + 80});
    manaText.setLineSpacing(1.2f);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);

    const auto& spells = player.getSpells();
    float buttonY = leftPanel.getPosition().y + 220;
    for (const auto& spell : spells) {
        sf::RectangleShape button;
        button.setSize({leftPanel.getSize().x - 40, 40});
        button.setPosition({leftPanel.getPosition().x + 20, buttonY});
        button.setFillColor(sf::Color(80, 80, 80));
        spellButtons.push_back(button);

        sf::Text text(font);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::White);
        text.setString(spell.name + " (" + std::to_string(spell.costAmount) + ")");
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
        text.setPosition({button.getPosition().x + button.getSize().x / 2.0f, button.getPosition().y + button.getSize().y / 2.0f});
        spellButtonTexts.push_back(text);

        buttonY += 50;
    }
}

void UIManager::update(const Player& player, const Monster& monster) {
    // Update HP and Mana text
    playerHpText.setString("Player HP: " + std::to_string(player.getCurrentHp()));
    monsterHpText.setString("Monster HP: " + std::to_string(monster.getCurrentHp()));
    manaText.setString("Fire: " + std::to_string(player.getMana(GemType::Fire)) + "\nWater: " + std::to_string(player.getMana(GemType::Water)) + "\nEarth: " + std::to_string(player.getMana(GemType::Earth)) + "\nLight: " + std::to_string(player.getMana(GemType::Light)));

    // Update spell button colors based on mana
    const auto& spells = player.getSpells();
    for(int i = 0; i < spells.size(); ++i) {
        if (player.getMana(spells[i].costType) >= spells[i].costAmount) {
            spellButtons[i].setFillColor(sf::Color(80, 80, 80));
        } else {
            spellButtons[i].setFillColor(sf::Color(40, 40, 40));
        }
    }

    // Update monster speed gauge
    float speedPercent = (float)monster.getActionCounter() / monster.getSpeed();
    monsterSpeedGaugeForeground.setSize({monsterSpeedGaugeBackground.getSize().x * speedPercent, monsterSpeedGaugeBackground.getSize().y});

    // Update Game Over text
    if (player.getCurrentHp() <= 0 || monster.getCurrentHp() <= 0) {
        bool playerWon = player.getCurrentHp() > 0;
        gameOverText.setString(playerWon ? "You are victorious!" : "You have been defeated!");
        gameOverText.setFillColor(playerWon ? sf::Color::Green : sf::Color::Red);
        sf::FloatRect textRect = gameOverText.getLocalBounds();
        gameOverText.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
        gameOverText.setPosition({(float)WINDOW_WIDTH / 2.0f, 150.f});
    }
}

void UIManager::render(sf::RenderWindow& window, GameState currentState) {
    window.draw(leftPanel);
    window.draw(rightPanel);
    window.draw(boardFrame);

    window.draw(playerHpText);
    window.draw(monsterHpText);
    window.draw(manaText);
    for(const auto& button : spellButtons) window.draw(button);
    for(const auto& text : spellButtonTexts) window.draw(text);
    window.draw(monsterSpeedGaugeBackground);
    window.draw(monsterSpeedGaugeForeground);

        if (currentState == GameState::GameOver) {

            window.draw(gameOverText);

        }

    }

    

    const std::vector<sf::RectangleShape>& UIManager::getSpellButtons() const {

        return spellButtons;

    }

    

    
