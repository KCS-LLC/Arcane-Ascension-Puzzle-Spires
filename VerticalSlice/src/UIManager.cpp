#include "UIManager.h"
#include "SpireData.h"
#include "DataManager.h"
#include "StringUtils.h"
#include <string>
#include <algorithm>

UIManager::UIManager(const sf::Font& fontRef)
    : font(fontRef),
      judgementTitle(font),
      playerPanelTitle(font),
      monsterPanelTitle(font),
      monsterNameText(font),
      manaTitle(font),
      gameOverText(font),
      explorationTitle(font),
      treasureTitle(font),
      specialTitle(font),
      puzzleTitle(font),
      trapTitle(font),
      sanctuaryTitle(font),
      agilityTitle(font),
      enduranceTitle(font),
      magicTitle(font)
{}

void UIManager::setup(const Player& player, const sf::Vector2u& windowSize, const sf::Vector2f& boardOrigin, const std::vector<Attunement>& attunements) {

    // --- Judgement Screen ---
    judgementTitle.setString("Choose your Attunement");
    judgementTitle.setCharacterSize(36);
    judgementTitle.setFillColor(sf::Color::White);
    sf::FloatRect textRect = judgementTitle.getLocalBounds();
    judgementTitle.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
    judgementTitle.setPosition({(float)windowSize.x / 2.0f, 100.f});

    float buttonY = 200.f;
    for (const auto& attunement : attunements) {
        sf::RectangleShape button;
        button.setSize({300, 50});
        button.setPosition({(float)windowSize.x / 2.0f - 150, buttonY});
        button.setFillColor(sf::Color(80, 80, 80));
        attunementButtons.push_back(button);

        sf::Text text(font);
        text.setString(attunement.name);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        textRect = text.getLocalBounds();
        text.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
        text.setPosition({button.getPosition().x + button.getSize().x / 2.0f, button.getPosition().y + button.getSize().y / 2.0f});
        attunementButtonTexts.push_back(text);

        buttonY += 70;
    }

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

    monsterNameText.setFont(font);
    monsterNameText.setString("TEMP NAME");
    monsterNameText.setCharacterSize(20);
    monsterNameText.setFillColor(sf::Color::White);
    monsterNameText.setPosition({rightPanel.getPosition().x + 20, rightPanel.getPosition().y + 50});
    
    // --- Player HP Gauge ---
    playerHpBarBack.setSize({panelWidth - 40, 20});
    playerHpBarBack.setPosition({leftPanel.getPosition().x + 20, leftPanel.getPosition().y + 70});
    playerHpBarBack.setFillColor(sf::Color(50, 0, 0));
    playerHpBarFront.setSize({panelWidth - 40, 20});
    playerHpBarFront.setPosition({leftPanel.getPosition().x + 20, leftPanel.getPosition().y + 70});
    playerHpBarFront.setFillColor(sf::Color(200, 0, 0));

    // --- Monster HP Gauge ---
    monsterHpBarBack.setSize({panelWidth - 40, 20});
    monsterHpBarBack.setPosition({rightPanel.getPosition().x + 20, rightPanel.getPosition().y + 80});
    monsterHpBarBack.setFillColor(sf::Color(50, 0, 0));
    monsterHpBarFront.setSize({panelWidth - 40, 20});
    monsterHpBarFront.setPosition({rightPanel.getPosition().x + 20, rightPanel.getPosition().y + 80});
    monsterHpBarFront.setFillColor(sf::Color(200, 0, 0));

    // --- Monster Speed Gauge ---
    monsterSpeedGaugeBackground.setSize({panelWidth - 40, 10});
    monsterSpeedGaugeBackground.setPosition({rightPanel.getPosition().x + 20, rightPanel.getPosition().y + 110});
    monsterSpeedGaugeBackground.setFillColor(sf::Color(50, 50, 50));
    monsterSpeedGaugeForeground.setSize({0, 10});
    monsterSpeedGaugeForeground.setPosition({rightPanel.getPosition().x + 20, rightPanel.getPosition().y + 110});
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

    // --- Game Over Text ---
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);

    // --- Player Damage Overlay ---
    playerDamageOverlay.setSize(leftPanel.getSize());
    playerDamageOverlay.setPosition(leftPanel.getPosition());
    playerDamageOverlay.setFillColor(sf::Color(255, 0, 0, 75));

    // --- Exploration Title ---
    explorationTitle.setFont(font);
    explorationTitle.setCharacterSize(36);
    explorationTitle.setFillColor(sf::Color::White);
    
    // --- Placeholder Screen Titles ---
    auto setupPlaceholderTitle = [&](sf::Text& text, const std::string& str) {
        text.setFont(font);
        text.setCharacterSize(36);
        text.setFillColor(sf::Color::White);
        text.setString(str);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
        text.setPosition({(float)windowSize.x / 2.0f, (float)windowSize.y / 2.0f});
    };

    setupPlaceholderTitle(treasureTitle, "Treasure Room\n(Click to continue)");
    setupPlaceholderTitle(specialTitle, "Special Room\n(Click to continue)");
    setupPlaceholderTitle(puzzleTitle, "Puzzle Room\n(Click to continue)");
    setupPlaceholderTitle(trapTitle, "Trap Room\n(Click to continue)");
    setupPlaceholderTitle(sanctuaryTitle, "Sanctuary\n(Click to continue)");
    setupPlaceholderTitle(agilityTitle, "Agility Challenge\n(Click to continue)");
    setupPlaceholderTitle(enduranceTitle, "Endurance Challenge\n(Click to continue)");
    setupPlaceholderTitle(magicTitle, "Magic Challenge\n(Click to continue)");
}

void UIManager::update(const Player& player, const Monster& monster, GameState currentState, const Room* currentRoom, const std::set<int>& visitedRoomIds, const DataManager& dataManager) {
    if (currentState == GameState::Judgement) {
        // No dynamic updates needed on the Judgement screen yet
        return;
    }
    
    // Update monster name
    monsterNameText.setString(monster.name);

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

    // --- Spell Buttons ---
    // Clear old buttons and create new ones based on player's current spells
    spellButtons.clear();
    spellButtonFills.clear();
    spellButtonTexts.clear();
    float buttonY = leftPanel.getPosition().y + 160 + (manaBarBacks.size() * 25) + 20;
    const auto& spells = player.getSpells();
    for (const auto& spell : spells) {
        sf::RectangleShape button;
        button.setSize({leftPanel.getSize().x - 40, 40});
        button.setPosition({leftPanel.getPosition().x + 20, buttonY});
        spellButtons.push_back(button);

        sf::RectangleShape fill;
        fill.setSize({leftPanel.getSize().x - 40, 0}); // Start with 0 height
        fill.setPosition({leftPanel.getPosition().x + 20, buttonY + 40}); // Position at the bottom
        spellButtonFills.push_back(fill);

        sf::Text text(font);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::White);
        text.setString(spell.name);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
        text.setPosition({button.getPosition().x + button.getSize().x / 2.0f, button.getPosition().y + button.getSize().y / 2.0f});
        spellButtonTexts.push_back(text);

        buttonY += 50;
    }


    // Update spell button colors and fills
    for(size_t i = 0; i < spells.size(); ++i) {
        float manaProgress = 0.f;
        if (spells[i].costAmount > 0) {
            manaProgress = std::min(1.f, (float)player.getMana(spells[i].costType) / spells[i].costAmount);
        }

        float fillHeight = 40 * manaProgress;
        spellButtonFills[i].setSize({spellButtonFills[i].getSize().x, fillHeight});
        spellButtonFills[i].setPosition({spellButtons[i].getPosition().x, spellButtons[i].getPosition().y + 40 - fillHeight});

        if (manaProgress >= 1.f) {
            spellButtons[i].setFillColor(sf::Color(40, 100, 40));
            spellButtonFills[i].setFillColor(sf::Color(100, 250, 100, 200));
        } else {
            spellButtons[i].setFillColor(sf::Color(40, 40, 40));
            spellButtonFills[i].setFillColor(sf::Color(0, 100, 200, 150));
        }
    }

    // Update monster speed gauge
    float speedPercent = (float)monster.getActionCounter() / monster.getSpeed();
    monsterSpeedGaugeForeground.setSize({monsterSpeedGaugeBackground.getSize().x * speedPercent, monsterSpeedGaugeBackground.getSize().y});

    // --- Update Door Buttons (only in exploration mode) ---
    doorButtons.clear();
    doorButtonTexts.clear();
    if (currentState == GameState::Exploration && currentRoom) {
        explorationTitle.setString(currentRoom->name);
        sf::FloatRect textRect = explorationTitle.getLocalBounds();
        explorationTitle.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
        explorationTitle.setPosition({(float)WINDOW_WIDTH / 2.0f, 100.f});

        const float buttonWidth = 150.f;
        const float buttonHeight = 50.f;
        const float buttonSpacing = 20.f;
        const float screenPadding = 100.f;
        int numDoors = currentRoom->connections.size();
        
        int buttonsPerLine = (WINDOW_WIDTH - screenPadding * 2) / (buttonWidth + buttonSpacing);
        int numRows = (numDoors + buttonsPerLine - 1) / buttonsPerLine;

        float totalLayoutHeight = (numRows * buttonHeight) + ((numRows - 1) * buttonSpacing);
        float startY = (WINDOW_HEIGHT - totalLayoutHeight) / 2.0f;

        float currentY = startY;
        int doorIndex = 0;

        for (int row = 0; row < numRows; ++row) {
            int doorsInThisRow = std::min(buttonsPerLine, numDoors - doorIndex);
            float totalLineWidth = (doorsInThisRow * buttonWidth) + ((doorsInThisRow - 1) * buttonSpacing);
            float startX = (WINDOW_WIDTH - totalLineWidth) / 2.0f;
            float currentX = startX;

            for (int i = 0; i < doorsInThisRow; ++i) {
                const auto& teleporter = currentRoom->connections[doorIndex];
                
                sf::RectangleShape button;
                button.setSize({buttonWidth, buttonHeight});
                button.setPosition({currentX, currentY});
                button.setFillColor(doorColorToSfColor(teleporter.color));
                doorButtons.push_back(button);

                sf::Text text(font);
                text.setCharacterSize(18);
                text.setFillColor(sf::Color::White);
                text.setOutlineColor(sf::Color::Black);
                text.setOutlineThickness(1.f);

                // **THE FIX**: Check if the destination room has been visited
                if (visitedRoomIds.count(teleporter.destinationRoomId)) {
                    const Room* destRoom = dataManager.getRoomById(teleporter.destinationRoomId);
                    if (destRoom) {
                        text.setString(destRoom->name);
                    } else {
                        text.setString("Error"); // Should not happen
                    }
                } else {
                    text.setString(doorColorToString(teleporter.color) + " Door");
                }
                
                sf::FloatRect textRect = text.getLocalBounds();
                text.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
                text.setPosition({button.getPosition().x + button.getSize().x / 2.0f, button.getPosition().y + button.getSize().y / 2.0f});
                doorButtonTexts.push_back(text);

                currentX += buttonWidth + buttonSpacing;
                doorIndex++;
            }
            currentY += buttonHeight + buttonSpacing;
        }
    }

    // Update Game Over text
    if (player.getCurrentHp() <= 0) {
        gameOverText.setString("You have been defeated!");
        gameOverText.setFillColor(sf::Color::Red);
        sf::FloatRect textRect = gameOverText.getLocalBounds();
        gameOverText.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
        gameOverText.setPosition({(float)WINDOW_WIDTH / 2.0f, 150.f});
    }
}

void UIManager::render(sf::RenderWindow& window, GameState currentState, bool showPlayerDamageEffect) {
    switch (currentState) {
        case GameState::Judgement:
            window.draw(judgementTitle);
            for(const auto& button : attunementButtons) window.draw(button);
            for(const auto& text : attunementButtonTexts) window.draw(text);
            break;
        case GameState::Playing:
        case GameState::Animating:
        case GameState::GameOver:
            window.draw(leftPanel);
            window.draw(rightPanel);
            window.draw(boardFrame);
            window.draw(monsterPanelTitle);
            window.draw(monsterNameText);
            window.draw(monsterHpBarBack);
            window.draw(monsterHpBarFront);
            window.draw(monsterSpeedGaugeBackground);
            window.draw(monsterSpeedGaugeForeground);
            window.draw(playerPanelTitle);
            window.draw(playerHpBarBack);
            window.draw(playerHpBarFront);
            window.draw(manaTitle);
            for (auto const& [type, bar] : manaBarBacks) window.draw(bar);
            for (auto const& [type, bar] : manaBarFronts) window.draw(bar);
            for(const auto& button : spellButtons) window.draw(button);
            for(const auto& fill : spellButtonFills) window.draw(fill);
            for(const auto& text : spellButtonTexts) window.draw(text);
            if (showPlayerDamageEffect) window.draw(playerDamageOverlay);
            if (currentState == GameState::GameOver) window.draw(gameOverText);
            break;

        case GameState::Exploration:
            window.draw(explorationTitle);
            for(const auto& button : doorButtons) window.draw(button);
            for(const auto& text : doorButtonTexts) window.draw(text);
            // Simplified player status at the bottom
            window.draw(playerHpBarBack);
            window.draw(playerHpBarFront);
            break;
        
        case GameState::Treasure: window.draw(treasureTitle); break;
        case GameState::Special: window.draw(specialTitle); break;
        case GameState::Puzzle: window.draw(puzzleTitle); break;
        case GameState::Trap: window.draw(trapTitle); break;
        case GameState::Sanctuary: window.draw(sanctuaryTitle); break;
        case GameState::AgilityChallenge: window.draw(agilityTitle); break;
        case GameState::EnduranceChallenge: window.draw(enduranceTitle); break;
        case GameState::MagicChallenge: window.draw(magicTitle); break;
    }
}

bool UIManager::handleEvent(const sf::Event& event, GameState currentState, const Room* currentRoom, const std::vector<Attunement>& attunements, UIAction& outAction) {
    if (auto* mbp = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mbp->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos(mbp->position);

            if (currentState == GameState::Judgement) {
                for (size_t i = 0; i < attunementButtons.size(); ++i) {
                    if (attunementButtons[i].getGlobalBounds().contains(mousePos)) {
                        outAction = {UIActionType::SelectAttunement, -1, -1, attunements[i].id};
                        return true;
                    }
                }
            }

            if (currentState == GameState::Playing) {
                if (leftPanel.getGlobalBounds().contains(mousePos) || rightPanel.getGlobalBounds().contains(mousePos)) {
                    for (size_t i = 0; i < spellButtons.size(); ++i) {
                        if (spellButtons[i].getGlobalBounds().contains(mousePos)) {
                            outAction = {UIActionType::CastSpell, (int)i, -1};
                            return true;
                        }
                    }
                    return true;
                }
            }
            
            if (currentState == GameState::Exploration && currentRoom) {
                for (size_t i = 0; i < doorButtons.size(); ++i) {
                    if (doorButtons[i].getGlobalBounds().contains(mousePos)) {
                        outAction = {UIActionType::ChangeRoom, -1, currentRoom->connections[i].destinationRoomId};
                        return true;
                    }
                }
            }
        }
    }
    return false;
}