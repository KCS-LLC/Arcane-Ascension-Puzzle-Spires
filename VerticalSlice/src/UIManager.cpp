#include "UIManager.h"
#include <string>

UIManager::UIManager(const sf::Font& fontRef)
    : font(fontRef),
      playerPanelTitle(font),
      monsterPanelTitle(font),
      manaTitle(font),
      gameOverText(font)
{}

void UIManager::setup(const Player& player, const sf::Vector2u& windowSize, const sf::Vector2f& boardOrigin) {

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

    // --- Titles ---
    playerPanelTitle.setFont(font);
    playerPanelTitle.setString("Player");
    playerPanelTitle.setCharacterSize(28);
    playerPanelTitle.setFillColor(sf::Color::White);
    playerPanelTitle.setPosition({leftPanel.getPosition().x + 20, leftPanel.getPosition().y + 20});

    monsterPanelTitle.setFont(font);
    monsterPanelTitle.setString("Monster");
    monsterPanelTitle.setCharacterSize(28);
    monsterPanelTitle.setFillColor(sf::Color::White);
    monsterPanelTitle.setPosition({rightPanel.getPosition().x + 20, rightPanel.getPosition().y + 20});
    
    // --- Player HP Gauge ---
    playerHpBarBack.setSize({panelWidth - 40, 20});
    playerHpBarBack.setPosition({leftPanel.getPosition().x + 20, leftPanel.getPosition().y + 70});
    playerHpBarBack.setFillColor(sf::Color(50, 0, 0));
    playerHpBarFront.setSize({panelWidth - 40, 20});
    playerHpBarFront.setPosition({leftPanel.getPosition().x + 20, leftPanel.getPosition().y + 70});
    playerHpBarFront.setFillColor(sf::Color(200, 0, 0));

    // --- Monster HP Gauge ---
    monsterHpBarBack.setSize({panelWidth - 40, 20});
    monsterHpBarBack.setPosition({rightPanel.getPosition().x + 20, rightPanel.getPosition().y + 70});
    monsterHpBarBack.setFillColor(sf::Color(50, 0, 0));
    monsterHpBarFront.setSize({panelWidth - 40, 20});
    monsterHpBarFront.setPosition({rightPanel.getPosition().x + 20, rightPanel.getPosition().y + 70});
    monsterHpBarFront.setFillColor(sf::Color(200, 0, 0));

    // --- Monster Speed Gauge ---
    monsterSpeedGaugeBackground.setSize({panelWidth - 40, 10});
    monsterSpeedGaugeBackground.setPosition({rightPanel.getPosition().x + 20, rightPanel.getPosition().y + 100});
    monsterSpeedGaugeBackground.setFillColor(sf::Color(50, 50, 50));
    monsterSpeedGaugeForeground.setSize({0, 10});
    monsterSpeedGaugeForeground.setPosition({rightPanel.getPosition().x + 20, rightPanel.getPosition().y + 100});
    monsterSpeedGaugeForeground.setFillColor(sf::Color::Cyan);

    // --- Mana Gauges ---
    manaTitle.setFont(font);
    manaTitle.setString("Mana");
    manaTitle.setCharacterSize(24);
    manaTitle.setFillColor(sf::Color::White);
    manaTitle.setPosition({leftPanel.getPosition().x + 20, leftPanel.getPosition().y + 120});

    float manaBarY = leftPanel.getPosition().y + 160;
    const std::vector<GemType> manaTypes = { GemType::Fire, GemType::Water, GemType::Earth, GemType::Light, GemType::Air, GemType::Umbral }; // Add all types to be displayed
    for (const auto& type : manaTypes) {
        sf::RectangleShape back;
        back.setSize({panelWidth - 40, 15});
        back.setPosition({leftPanel.getPosition().x + 20, manaBarY});
        back.setFillColor(sf::Color(40, 40, 40));
        manaBarBacks[type] = back;

        sf::RectangleShape front;
        front.setSize({0, 15});
        front.setPosition({leftPanel.getPosition().x + 20, manaBarY});
        // Set color based on mana type
        switch(type) {
            case GemType::Fire: front.setFillColor(sf::Color::Red); break;
            case GemType::Water: front.setFillColor(sf::Color::Blue); break;
            case GemType::Earth: front.setFillColor(sf::Color::Green); break;
            case GemType::Light: front.setFillColor(sf::Color::Yellow); break;
            case GemType::Air: front.setFillColor(sf::Color(173, 216, 230)); break; // Light blue
            case GemType::Umbral: front.setFillColor(sf::Color(128, 0, 128)); break; // Purple
            default: break;
        }
        manaBarFronts[type] = front;
        manaBarY += 25;
    }

    // --- Spell Buttons ---
    const auto& spells = player.getSpells();
    float buttonY = manaBarY + 20; // Position buttons below mana bars
    for (const auto& spell : spells) {
        sf::RectangleShape button;
        button.setSize({panelWidth - 40, 40});
        button.setPosition({leftPanel.getPosition().x + 20, buttonY});
        button.setFillColor(sf::Color(80, 80, 80));
        spellButtons.push_back(button);

        sf::RectangleShape fill;
        fill.setSize({panelWidth - 40, 0}); // Start with 0 height
        fill.setPosition({leftPanel.getPosition().x + 20, buttonY + 40}); // Position at the bottom
        fill.setFillColor(sf::Color(0, 100, 200, 150)); // A semi-transparent blue
        spellButtonFills.push_back(fill);

        sf::Text text(font);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::White);
        text.setString(spell.name); // Remove cost from text
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
        text.setPosition({button.getPosition().x + button.getSize().x / 2.0f, button.getPosition().y + button.getSize().y / 2.0f});
        spellButtonTexts.push_back(text);

        buttonY += 50;
    }

    // --- Game Over Text ---
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
}

void UIManager::update(const Player& player, const Monster& monster) {
    // Update HP gauges
    float playerHpPercent = (float)player.getCurrentHp() / player.getMaxHp();
    playerHpBarFront.setSize({playerHpBarBack.getSize().x * playerHpPercent, playerHpBarBack.getSize().y});

    float monsterHpPercent = (float)monster.getCurrentHp() / monster.getMaxHp();
    monsterHpBarFront.setSize({monsterHpBarBack.getSize().x * monsterHpPercent, monsterHpBarBack.getSize().y});

    // Update Mana gauges
    for (auto const& [type, back] : manaBarBacks) {
        float manaPercent = (float)player.getMana(type) / player.getMaxMana();
        manaBarFronts[type].setSize({back.getSize().x * manaPercent, back.getSize().y});
    }

    // Update spell button colors and fills
    const auto& spells = player.getSpells();
    for(size_t i = 0; i < spells.size(); ++i) {
        float manaProgress = 0.f;
        if (spells[i].costAmount > 0) {
            manaProgress = std::min(1.f, (float)player.getMana(spells[i].costType) / spells[i].costAmount);
        }

        float fillHeight = 40 * manaProgress;
        spellButtonFills[i].setSize({spellButtonFills[i].getSize().x, fillHeight});
        spellButtonFills[i].setPosition({spellButtons[i].getPosition().x, spellButtons[i].getPosition().y + 40 - fillHeight});

        if (manaProgress >= 1.f) {
            spellButtons[i].setFillColor(sf::Color(40, 100, 40)); // A noticeable green background
            spellButtonFills[i].setFillColor(sf::Color(100, 250, 100, 200)); // Bright green fill
        } else {
            spellButtons[i].setFillColor(sf::Color(40, 40, 40)); // Not ready color
            spellButtonFills[i].setFillColor(sf::Color(0, 100, 200, 150)); // Progress blue fill
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

    // Draw Titles
    window.draw(playerPanelTitle);
    window.draw(monsterPanelTitle);

    // Draw Gauges
    window.draw(playerHpBarBack);
    window.draw(playerHpBarFront);
    window.draw(monsterHpBarBack);
    window.draw(monsterHpBarFront);
    window.draw(monsterSpeedGaugeBackground);
    window.draw(monsterSpeedGaugeForeground);

    // Draw Mana UI
    window.draw(manaTitle);
    for (auto const& [type, bar] : manaBarBacks) window.draw(bar);
    for (auto const& [type, bar] : manaBarFronts) window.draw(bar);

    // Draw Spell Buttons
    for(const auto& button : spellButtons) window.draw(button);
    for(const auto& fill : spellButtonFills) window.draw(fill); // Draw the fill
    for(const auto& text : spellButtonTexts) window.draw(text);

    if (currentState == GameState::GameOver) {
        window.draw(gameOverText);
    }
}

    

    const std::vector<sf::RectangleShape>& UIManager::getSpellButtons() const {

    

        return spellButtons;

    

    }

    

    

    

    std::optional<UIAction> UIManager::handleEvent(const sf::Event& event) {

    

        if (auto* mbp = event.getIf<sf::Event::MouseButtonPressed>()) {

    

            if (mbp->button == sf::Mouse::Button::Left) {

    

                for (int i = 0; i < spellButtons.size(); ++i) {

    

                    if (spellButtons[i].getGlobalBounds().contains(sf::Vector2f(mbp->position))) {

    

                        // A spell button was clicked, return the corresponding action.

    

                        return UIAction{UIActionType::CastSpell, i};

    

                    }

    

                }

    

            }

    

        }

    

        // No UI element was interacted with, return no action.

    

        return std::nullopt;

    

    }

    

    

    

    
