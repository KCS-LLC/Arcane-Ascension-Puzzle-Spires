#include "PCH.h"
#include "UIManager.h"
#include "Player.h"
#include "Monster.h"
#include "DataManager.h"
#include "Constants.h"
#include "StringUtils.h"
#include "TimeManager.h"

UIManager::UIManager(const sf::Font& font)
    : font(font),
      trialTypeText(font, "", 24),
      trialObjectiveText(font, "", 18),
      turnLimitText(font, "", 20),
      scoreGoalText(font, "", 20),
      currentTrialScoreText(font, "", 20),
      manaAffinityPromptText(font, "", 22),
      manaAffinityChoiceText(font, "", 20),
      judgementSummaryTitle(font, "", 30),
      judgementResultsText(font, "", 18),
      attunementTitleText(font, "", 30),
      attunementNameText(font, "", 28),
      attunementDescriptionText(font, "", 18),
      m_roomNameText(font, "", 24),
      m_roomDescriptionText(font, "", 18),
      playerPanelTitle(font, "Player", 20),
      monsterPanelTitle(font, "Monster", 20),
      monsterNameText(font, "", 22),
      monsterHpText(font, "", 18),
      manaTitle(font, "Mana", 18),
      gameOverText(font, "Game Over", 48),
      explorationTitle(font, "Exploration", 30),
      treasureTitle(font, "Treasure Room", 24),
      specialTitle(font, "Special Event", 24),
      puzzleTitle(font, "Puzzle Room", 24),
      trapTitle(font, "Trap Room", 24),
      sanctuaryTitle(font, "Sanctuary", 24),
      agilityTitle(font, "Agility Challenge", 24),
      enduranceTitle(font, "Endurance Challenge", 24),
      magicTitle(font, "Magic Challenge", 24),
      m_attunementSelectionTitle(font, "", 24),
      m_quickSwapInstructionText(font, "", 18),
      m_targetingPromptText(font, "", 18),
      m_dateText(font, "", 16),
      m_timeText(font, "", 16)
{
    // Positions and colors can be set here
    trialTypeText.setFillColor(sf::Color::White);
    trialObjectiveText.setFillColor(sf::Color::White);
    turnLimitText.setFillColor(sf::Color::White);
    scoreGoalText.setFillColor(sf::Color::White);
    currentTrialScoreText.setFillColor(sf::Color::White);
    
    trialTypeText.setPosition(sf::Vector2f{ 10, 10 });
    trialObjectiveText.setPosition(sf::Vector2f{ 10, 40 });
    turnLimitText.setPosition(sf::Vector2f{ 10, 70 });
    scoreGoalText.setPosition(sf::Vector2f{ 10, 100 });
    currentTrialScoreText.setPosition(sf::Vector2f{ 650, 10 });

    m_roomNameText.setFillColor(sf::Color::White);
    m_roomNameText.setPosition(sf::Vector2f{ 10, 10 });

    m_roomDescriptionText.setFillColor(sf::Color::White);
    m_roomDescriptionText.setPosition(sf::Vector2f{ 10, 40 });

    judgementSummaryTitle.setFillColor(sf::Color::White);
    judgementResultsText.setFillColor(sf::Color::White);
    judgementSummaryTitle.setPosition(sf::Vector2f{ 450, 200 });
    judgementResultsText.setPosition(sf::Vector2f{ 450, 250 });

    m_attunementSelectionTitle.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = m_attunementSelectionTitle.getLocalBounds();
    m_attunementSelectionTitle.setOrigin(sf::Vector2f(titleBounds.position.x + titleBounds.size.x / 2.0f, titleBounds.position.y + titleBounds.size.y / 2.0f));
    m_attunementSelectionTitle.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 150));

    m_quickSwapInstructionText.setFillColor(sf::Color::Yellow);
    m_targetingPromptText.setFillColor(sf::Color::Yellow);

    m_dateText.setFillColor(sf::Color::White);
    m_timeText.setFillColor(sf::Color::White);

    // Center the date/time text at the top
    sf::FloatRect dateRect = m_dateText.getLocalBounds();
    m_dateText.setOrigin(sf::Vector2f(dateRect.position.x + dateRect.size.x / 2.0f, dateRect.position.y + dateRect.size.y / 2.0f));
    m_dateText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 20));

    sf::FloatRect timeRect = m_timeText.getLocalBounds();
    m_timeText.setOrigin(sf::Vector2f(timeRect.position.x + timeRect.size.x / 2.0f, timeRect.position.y + timeRect.size.y / 2.0f));
    m_timeText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 40));
}
      
bool UIManager::handleEvent(const sf::Event& event, GameMode gameMode, GameState currentState, const Room* currentRoom, const std::vector<Attunement>& attunements, UIAction& outAction) {
    if (currentState == GameState::Exploration) {
        if (auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mb->button == sf::Mouse::Button::Left) {
                for (size_t i = 0; i < doorButtons.size(); ++i) {
                    if (doorButtons[i].getGlobalBounds().contains(sf::Vector2f(mb->position))) {
                        outAction.type = UIActionType::ChangeRoom;
                        outAction.destinationRoomId = m_currentConnections[i].destinationRoomId;
                        return true;
                    }
                }
            }
        }
    }
    if (currentState == GameState::Judgement_TreasureRound) {
        if (auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mb->button == sf::Mouse::Button::Left) {
                if (leftPanel.getGlobalBounds().contains(sf::Vector2f(mb->position)) ||
                    rightPanel.getGlobalBounds().contains(sf::Vector2f(mb->position))) {
                    // If the click is on a panel, consume it
                    return true;
                }
            }
        }
    }
    if (currentState == GameState::Judgement_AttunementSelection) {
        if (auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mb->button == sf::Mouse::Button::Left) {
                for (size_t i = 0; i < m_attunementButtons.size(); ++i) {
                    if (m_attunementButtons[i].getGlobalBounds().contains(sf::Vector2f(mb->position))) {
                        outAction.type = UIActionType::SelectAttunement;
                        // This relies on the buttons being created in the same order as the attunements are provided
                        outAction.attunementId = attunements[i].id;
                        return true;
                    }
                }
            }
        }
    }
    if (currentState == GameState::Playing || currentState == GameState::Trial) {
        if (auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mb->button == sf::Mouse::Button::Left) {
                if (leftPanel.getGlobalBounds().contains(sf::Vector2f(mb->position)) ||
                    rightPanel.getGlobalBounds().contains(sf::Vector2f(mb->position))) {
                    // If the click is on a panel, we consume it immediately to prevent fall-through to board logic
                    for (size_t i = 0; i < spellButtons.size(); ++i) {
                        if (spellButtons[i].getGlobalBounds().contains(sf::Vector2f(mb->position))) {
                            outAction.type = UIActionType::CastSpell;
                            outAction.spellIndex = i;
                            return true;
                        }
                    }
                    return true; // Click on panel, but not on a spell button, still consume it
                }
            }
        }
    }

    // Placeholder logic for other UI event handling
    return false;
}
      
void UIManager::setup(const Player& player, const sf::Vector2u& windowSize, const sf::Vector2f& boardOrigin, const std::vector<Attunement>& attunements) {
    const int boardPixelWidth = BOARD_WIDTH * TILE_SIZE;
    const int boardPixelHeight = BOARD_HEIGHT * TILE_SIZE;

    boardFrame.setSize(sf::Vector2f{(float)boardPixelWidth + 4, (float)boardPixelHeight + 4});
    boardFrame.setPosition(sf::Vector2f{boardOrigin.x - 2, boardOrigin.y - 2});
    boardFrame.setFillColor(sf::Color::Transparent);
    boardFrame.setOutlineColor(sf::Color(100, 100, 100));
    boardFrame.setOutlineThickness(2);
    // --- Combat UI Panels ---
    leftPanel.setSize(sf::Vector2f{250, (float)windowSize.y});
    leftPanel.setPosition(sf::Vector2f{0, 0});
    leftPanel.setFillColor(sf::Color(50, 50, 50, 200));

    rightPanel.setSize(sf::Vector2f{250, (float)windowSize.y});
    rightPanel.setPosition(sf::Vector2f{(float)windowSize.x - 250, 0});
    rightPanel.setFillColor(sf::Color(50, 50, 50, 200));

    playerPanelTitle.setPosition(sf::Vector2f{20, 20});
    monsterPanelTitle.setPosition(sf::Vector2f{windowSize.x - rightPanel.getSize().x + 20, 20});
    monsterNameText.setPosition(sf::Vector2f{windowSize.x - rightPanel.getSize().x + 20, 50});
    monsterHpText.setPosition(sf::Vector2f{windowSize.x - rightPanel.getSize().x + 20, 70});

    // --- HP Bars ---
    playerHpBarBack.setSize(sf::Vector2f{210, 20});
    playerHpBarBack.setPosition(sf::Vector2f{20, 60});
    playerHpBarBack.setFillColor(sf::Color(100, 0, 0));
    playerHpBarFront = playerHpBarBack; // Copy size and position
    playerHpBarFront.setFillColor(sf::Color(220, 0, 0));

    monsterHpBarBack.setSize(sf::Vector2f{210, 20});
    monsterHpBarBack.setPosition(sf::Vector2f{windowSize.x - rightPanel.getSize().x + 20, 90});
    monsterHpBarBack.setFillColor(sf::Color(100, 0, 0));
    monsterHpBarFront = monsterHpBarBack; // Copy size and position
    monsterHpBarFront.setFillColor(sf::Color(220, 0, 0));

    // --- Mana Bars Title ---
    manaTitle.setPosition(sf::Vector2f{20, 120});

    monsterSpeedGaugeBackground.setSize(sf::Vector2f{210, 10});
    monsterSpeedGaugeBackground.setPosition(sf::Vector2f{windowSize.x - rightPanel.getSize().x + 20, 120});
    monsterSpeedGaugeBackground.setFillColor(sf::Color(40, 40, 40));
    monsterSpeedGaugeForeground = monsterSpeedGaugeBackground;
    monsterSpeedGaugeForeground.setFillColor(sf::Color(200, 200, 0));

    // --- Game Over Text ---
    gameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.f, textRect.position.y + textRect.size.y / 2.f));
    gameOverText.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f));
}
void UIManager::setupTrial(const JudgementTrial& trial) {
    std::string trialTypeStr;
    switch (trial.type) {
        case JudgementTrialType::Power: trialTypeStr = "Trial of Power"; break;
        case JudgementTrialType::Haste: trialTypeStr = "Trial of Haste"; break;
        case JudgementTrialType::Control: trialTypeStr = "Trial of Control"; break;
        default: trialTypeStr = "Unknown Trial"; break;
    }
    trialTypeText.setString(trialTypeStr);
    trialObjectiveText.setString(wordWrap(trial.objective, 40));
}

void UIManager::setupTreasureRound() {
    trialTypeText.setString("Treasure Round");
    trialObjectiveText.setString("Maximize your score by merging treasure gems!");
    scoreGoalText.setString(""); // No score goal, just maximize
}

void UIManager::update(const Player& player, const Monster& monster, const TimeManager& timeManager, GameMode gameMode, GameState currentState, PlayMode playMode, const TargetingRequest& targetingRequest, const Room* currentRoom, const Floor& currentFloor, const std::set<int>& visitedRoomIds, const DataManager& dataManager, const JudgementTrial& currentTrial, int currentScore, int currentTrialTurn, const std::optional<PrimaryGemType>& manaAffinityChoice, const TrialPerformance& performance, const std::vector<ActiveEffect>& activeEffects) {
    m_dateText.setString(timeManager.getDateString());
    m_timeText.setString(timeManager.getTimeString());

    m_activeEffectsToRender = activeEffects;
    if (currentState == GameState::Trial) {
        turnLimitText.setString("Turns Left: " + std::to_string(currentTrial.turnLimit - currentTrialTurn));
        scoreGoalText.setString("Score Goal: " + std::to_string(currentTrial.scoreGoal));
        currentTrialScoreText.setString("Score: " + std::to_string(currentScore));
    } else if (currentState == GameState::Judgement_TreasureRound) {
        turnLimitText.setString("Turns Left: " + std::to_string(20 - currentTrialTurn));
        scoreGoalText.setString(""); // No score goal in treasure round
        currentTrialScoreText.setString("Score: " + std::to_string(currentScore));
    }

    if (currentState == GameState::Playing || currentState == GameState::Trial) {
        // Update HP Bars
        float playerHpPercent = static_cast<float>(player.getHp()) / player.getMaxHp();
        playerHpBarFront.setSize(sf::Vector2f{playerHpBarBack.getSize().x * playerHpPercent, playerHpBarBack.getSize().y});

        float monsterHpPercent = static_cast<float>(monster.getCurrentHp()) / monster.getMaxHp();
        monsterHpBarFront.setSize(sf::Vector2f{monsterHpBarBack.getSize().x * monsterHpPercent, monsterHpBarBack.getSize().y});
        
        monsterNameText.setString(monster.getName());
        monsterHpText.setString(std::to_string(monster.getCurrentHp()) + "/" + std::to_string(monster.getMaxHp()));

        // Update Monster Speed Gauge
        float speedPercent = static_cast<float>(monster.getActionCounter()) / monster.getSpeed();
        monsterSpeedGaugeForeground.setSize(sf::Vector2f{monsterSpeedGaugeBackground.getSize().x * speedPercent, monsterSpeedGaugeBackground.getSize().y});

        // Update Mana Bars
        manaBarBacks.clear();
        manaBarFronts.clear();
        manaBarTexts.clear();

        const auto& manaTypes = player.getManaTypes();
        // Diagnostic logging (only in combat/trial modes)
        // if (currentState == GameState::Playing || currentState == GameState::Trial) {
        //     std::cout << "Updating UI for mana types: ";
        //     for(const auto& type : manaTypes) {
        //         std::cout << static_cast<int>(type) << " ";
        //     }
        //     std::cout << std::endl;
        // }

        float yOffset = 100.f; // Start mana bars lower
        for (const auto& type : manaTypes) {
            int currentMana = player.getMana(type);
            int maxMana = player.getMaxMana();

            sf::RectangleShape back({150, 15});
            back.setPosition(sf::Vector2f(20, yOffset));
            back.setFillColor(sf::Color(50, 50, 50));
            manaBarBacks[type] = back;

            float manaPercent = (maxMana > 0) ? static_cast<float>(currentMana) / maxMana : 0.f;
            sf::RectangleShape front({150 * manaPercent, 15});
            front.setPosition(sf::Vector2f(20, yOffset));
            front.setFillColor(getSfColorForGemType(type));
            manaBarFronts[type] = front;

            sf::Text text(font, std::to_string(currentMana) + "/" + std::to_string(maxMana), 12);
            text.setPosition(sf::Vector2f(175, yOffset));
            manaBarTexts.emplace(type, text);

            yOffset += 25.f;
        }

                    // --- Refactored Spell Buttons at Bottom ---
                spellButtons.clear();
                spellButtonTexts.clear();
                const auto& spells = player.getSpells();
                float ySpellOffset = WINDOW_HEIGHT - 50.f; // Start from the bottom and build up
                for (const auto& spell : spells) {
                    sf::RectangleShape button({210, 40});
                    button.setPosition(sf::Vector2f(20, ySpellOffset));
                    if (player.getMana(spell->costType) >= spell->manaCost) {
                        button.setFillColor(sf::Color(100, 100, 180)); // Ready color
                    } else {
                        button.setFillColor(sf::Color(50, 50, 80));  // Not enough mana color
                    }
                    // Add outline for the active targeting spell
                    if (playMode == PlayMode::Targeting && spell->id == m_activeTargetingSpellId) {
                        button.setOutlineThickness(3);
                        button.setOutlineColor(sf::Color::Yellow);
                    } else {
                        button.setOutlineThickness(1);
                        button.setOutlineColor(sf::Color(100, 100, 100));
                    }
                    spellButtons.push_back(button);
            // Spell Name (left-aligned)
            sf::Text nameText(font, spell->name, 16);
            nameText.setPosition(sf::Vector2f(30, ySpellOffset + 10));
            spellButtonTexts.push_back(nameText);

            // Spell Cost (right-aligned)
            std::string costStr = "M:" + std::to_string(spell->manaCost) + " S:" + std::to_string(spell->speedCost);
            sf::Text costText(font, costStr, 14);
            sf::FloatRect textBounds = costText.getLocalBounds();
            costText.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x, 0));
            costText.setPosition(sf::Vector2f(20 + 200, ySpellOffset + 12));
            spellButtonTexts.push_back(costText);

            ySpellOffset -= 50.f;
        }

    }

    if (playMode == PlayMode::Targeting) {
        const Spell* spell = dataManager.getSpellById(targetingRequest.abilityId);
        if (spell != nullptr && spell->targeting.has_value()) {
            m_targetingPromptText.setString(wordWrap(spell->targeting->prompt, 50));
        }
    }

    if (currentState == GameState::Exploration) {
        if (currentRoom != nullptr) {
            m_roomNameText.setString(currentRoom->name);
            m_roomDescriptionText.setString("Explore the room. Which way will you go?");

            // --- Dynamic Door Button Generation ---
            doorButtons.clear();
            doorButtonTexts.clear();
            m_currentConnections = currentRoom->connections;

            const float buttonWidth = 220.f;
            const float buttonHeight = 50.f;
            const float buttonSpacing = 20.f;
            const int numButtons = m_currentConnections.size();
            const float totalHeight = (numButtons * buttonHeight) + ((numButtons - 1) * buttonSpacing);
            float startY = (WINDOW_HEIGHT - totalHeight) / 2.f;

            for (size_t i = 0; i < m_currentConnections.size(); ++i) {
                const auto& connection = m_currentConnections[i];
                const Room* destinationRoom = nullptr;
                for(const auto& room : currentFloor.rooms) {
                    if (room.id == connection.destinationRoomId) {
                        destinationRoom = &room;
                        break;
                    }
                }

                if (destinationRoom != nullptr) {
                    sf::RectangleShape button({buttonWidth, buttonHeight});
                    button.setPosition(sf::Vector2f((WINDOW_WIDTH - buttonWidth) / 2.f, startY + i * (buttonHeight + buttonSpacing)));
                    button.setFillColor(getSfColorForRoomType(destinationRoom->type));
                    button.setOutlineColor(sf::Color(200, 200, 200));
                    button.setOutlineThickness(1.f);
                    doorButtons.push_back(button);

                    std::string buttonTextStr;
                    if (visitedRoomIds.count(destinationRoom->id) != 0u) {
                        buttonTextStr = destinationRoom->name;
                    } else {
                        buttonTextStr = roomTypeToString(destinationRoom->type) + " Door";
                    }

                    sf::Text buttonText(font, buttonTextStr, 20);
                    buttonText.setFillColor(sf::Color::White);
                    sf::FloatRect textBounds = buttonText.getLocalBounds();
                    buttonText.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f));
                    buttonText.setPosition(button.getPosition() + sf::Vector2f(button.getSize().x / 2.f, button.getSize().y / 2.f));
                    doorButtonTexts.push_back(buttonText);
                }
            }
        }
    }

    if (currentState == GameState::Judgement_AttunementSelection) {
        m_attunementSelectionTitle.setString("Choose Your Attunement");
        
        m_attunementButtons.clear();
        m_attunementButtonTexts.clear();

        const auto& allAttunements = dataManager.getAttunements();
        const int buttonsPerRow = 4;
        const float buttonWidth = 220.f;
        const float buttonHeight = 50.f;
        const float horizontalSpacing = 20.f;
        const float verticalSpacing = 20.f;
        
        const float totalWidth = (buttonsPerRow * buttonWidth) + ((buttonsPerRow - 1) * horizontalSpacing);
        const float startX = (WINDOW_WIDTH - totalWidth) / 2.f;
        const float startY = 200.f; // Position below the title

        for (size_t i = 0; i < allAttunements.size(); ++i) {
            const auto& attunement = allAttunements[i];
            
            int row = i / buttonsPerRow;
            int col = i % buttonsPerRow;

            float posX = startX + col * (buttonWidth + horizontalSpacing);
            float posY = startY + row * (buttonHeight + verticalSpacing);

            sf::RectangleShape button({buttonWidth, buttonHeight});
            button.setPosition(sf::Vector2f(posX, posY));
            button.setFillColor(sf::Color(80, 80, 120));
            m_attunementButtons.push_back(button);

            sf::Text text(font, attunement.name, 20);
            sf::FloatRect textBounds = text.getLocalBounds();
            text.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f));
            text.setPosition(button.getPosition() + sf::Vector2f(button.getSize().x / 2.f, button.getSize().y / 2.f));
            m_attunementButtonTexts.push_back(text);
        }
    }
}

void UIManager::render(sf::RenderWindow& window, const sf::Vector2f& boardOrigin, GameMode gameMode, GameState currentState, PlayMode playMode, const TargetingRequest& targetingRequest, bool showPlayerDamageEffect, const JudgementTrial& currentTrial, int currentScore, int currentTrialTurn, const std::optional<PrimaryGemType>& manaAffinityChoice, const TrialPerformance& performance, const std::map<GemSubType, sf::Texture>& gemTextures, const std::map<std::string, sf::Texture>& effectIconTextures) {
    window.draw(m_dateText);
    window.draw(m_timeText);

    switch (currentState) {
        case GameState::Intro:
            // Draw specific Intro UI elements here if any
            break;
        case GameState::Trial:
        case GameState::Judgement_TreasureRound:
            window.draw(trialTypeText);
            window.draw(trialObjectiveText);
            window.draw(turnLimitText);
            window.draw(scoreGoalText);
            window.draw(currentTrialScoreText);
            break;
        case GameState::Summary:
            judgementSummaryTitle.setString("Trial Complete");
            judgementResultsText.setString("Final Score: " + std::to_string(currentScore));
            window.draw(judgementSummaryTitle);
            window.draw(judgementResultsText);
            break;
        case GameState::Judgement_AttunementSelection:
            window.draw(m_attunementSelectionTitle);
            for (const auto& button : m_attunementButtons) {
                window.draw(button);
            }
            for (const auto& text : m_attunementButtonTexts) {
                window.draw(text);
            }
            break;
        case GameState::AttunementReveal:
            attunementTitleText.setString("Judgement Complete");
            judgementResultsText.setString(
                "Power Score: " + std::to_string(performance.powerScore) + "\n" + 
                "Haste Score: " + std::to_string(performance.hasteScore) + "\n" + 
                "Control Score: " + std::to_string(performance.controlScore)
            );
            window.draw(attunementTitleText);
            window.draw(judgementResultsText);
            break;
        case GameState::Exploration:
            window.draw(m_roomNameText);
            window.draw(m_roomDescriptionText);
            for (size_t i = 0; i < doorButtons.size(); ++i) {
                window.draw(doorButtons[i]);
                window.draw(doorButtonTexts[i]);
            }
            break;
        case GameState::Playing:
            window.draw(leftPanel);
            window.draw(rightPanel);
            window.draw(playerPanelTitle);
            window.draw(monsterPanelTitle);
            window.draw(monsterNameText);
            window.draw(playerHpBarBack);
            window.draw(playerHpBarFront);
            window.draw(monsterHpBarBack);
            window.draw(monsterHpBarFront);
            window.draw(monsterHpText);
            window.draw(monsterSpeedGaugeBackground);
            window.draw(monsterSpeedGaugeForeground);
            window.draw(manaTitle);
            for (const auto& pair : manaBarBacks) {
                window.draw(pair.second);
            }
            for (const auto& pair : manaBarFronts) {
                window.draw(pair.second);
            }
            for (const auto& pair : manaBarTexts) {
                window.draw(pair.second);
            }
            for (const auto& button : spellButtons) {
                window.draw(button);
            }
            for (const auto& text : spellButtonTexts) {
                window.draw(text);
            }

            if (m_activeEffectsToRender.size() > 0 && m_activeEffectsToRender[0].effectId == "quick_swap_transference") {
                window.draw(m_quickSwapInstructionText);
            }

            if (playMode == PlayMode::Targeting) {
                sf::FloatRect textRect = m_targetingPromptText.getLocalBounds();
                m_targetingPromptText.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f));
                m_targetingPromptText.setPosition(sf::Vector2f(boardOrigin.x + (BOARD_WIDTH * TILE_SIZE) / 2.0f, boardOrigin.y - 70.0f));
                window.draw(m_targetingPromptText);
            }

            // Render Active Effects
            {
                float xOffset = 20.f; // Start at the left of the panel
                const float yOffset = 180.f; // A fixed Y position below the mana bars
                for (const auto& effect : m_activeEffectsToRender) {
                    auto it = effectIconTextures.find(effect.effectId);
                    if (it != effectIconTextures.end()) {
                        sf::Sprite icon(it->second);
                        
                        const sf::Texture& texture = it->second;
                        float desiredHeight = TILE_SIZE / 3.0f;
                        float scale = desiredHeight / texture.getSize().y;
                        icon.setScale(sf::Vector2f(scale, scale));
                        icon.setPosition(sf::Vector2f(xOffset, yOffset));
                        
                        float iconWidth = texture.getSize().x * scale;
                        
                        window.draw(icon);

                        // --- Duration Bar ---
                        sf::RectangleShape durationBarBack(sf::Vector2f(40, 8));
                        durationBarBack.setPosition(sf::Vector2f(xOffset + iconWidth + 5, yOffset + (desiredHeight / 2) - 4));
                        durationBarBack.setFillColor(sf::Color(50, 50, 50));
                        window.draw(durationBarBack);

                        float durationPercent = effect.duration / effect.maxDuration;
                        sf::RectangleShape durationBarFront(sf::Vector2f(40 * durationPercent, 8));
                        durationBarFront.setPosition(sf::Vector2f(xOffset + iconWidth + 5, yOffset + (desiredHeight / 2) - 4));
                        durationBarFront.setFillColor(sf::Color(200, 200, 0));
                        window.draw(durationBarFront);

                        // --- Duration Text ---
                        sf::Text durationText(font, std::to_string((int)ceil(effect.duration)), 10);
                        durationText.setPosition(sf::Vector2f(xOffset + iconWidth + 50, yOffset + (desiredHeight / 2) - 8));
                        window.draw(durationText);

                        xOffset += (iconWidth + 80); // Increment X position for the next effect
                    }
                }
            }
            break;
        case GameState::GameOver:
            window.draw(gameOverText);
            break;
        default:
            break;
    }
}

const std::vector<sf::RectangleShape>& UIManager::getSpellButtons() const {
    return spellButtons;
}

void UIManager::renderAnimations(sf::RenderWindow& window, const std::vector<Animation>& animations) {
    for (const auto& animation : animations) {
        if (animation.type == AnimationType::HpSweep) {
            const sf::RectangleShape* baseBar = nullptr;
            if (animation.target == AnimationTarget::PlayerHpBar) {
                baseBar = &playerHpBarBack;
            } else if (animation.target == AnimationTarget::MonsterHpBar) {
                baseBar = &monsterHpBarBack;
            }

            if (baseBar == nullptr) { continue; }

            float progress = animation.clock.getElapsedTime().asSeconds() / animation.lifetime.asSeconds();
            std::uint8_t alpha = static_cast<std::uint8_t>(255.f * (1.f - progress));
            
            sf::RectangleShape sweep;
            sweep.setSize({0, baseBar->getSize().y});
            sweep.setFillColor({animation.color.r, animation.color.g, animation.color.b, alpha});
            sweep.setPosition(baseBar->getPosition());

            float startPercent = animation.startValue / animation.maxValue;
            float endPercent = animation.endValue / animation.maxValue;

            if (endPercent > startPercent) { // Healing
                float sweepX = baseBar->getPosition().x + startPercent * baseBar->getSize().x;
                float sweepWidth = (endPercent - startPercent) * baseBar->getSize().x;
                sweep.setPosition({sweepX, baseBar->getPosition().y});
                sweep.setSize({sweepWidth, baseBar->getSize().y});
            } else { // Damage
                float sweepX = baseBar->getPosition().x + endPercent * baseBar->getSize().x;
                float sweepWidth = (startPercent - endPercent) * baseBar->getSize().x;
                sweep.setPosition({sweepX, baseBar->getPosition().y});
                sweep.setSize({sweepWidth, baseBar->getSize().y});
            }

            window.draw(sweep);
        }
    }
}