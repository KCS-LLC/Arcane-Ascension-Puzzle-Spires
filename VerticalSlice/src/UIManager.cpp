#include "PCH.h"
#include "UIManager.h"
#include "Player.h"
#include "Monster.h"
#include "DataManager.h"
#include "Constants.h"
#include "StringUtils.h"

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
      magicTitle(font, "Magic Challenge", 24)
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
}
    
    bool UIManager::handleEvent(const sf::Event& event, GameMode gameMode, GameState currentState, UIAction& outAction) {
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

void UIManager::update(const Player& player, const Monster& monster, GameMode gameMode, GameState currentState, const Room* currentRoom, const Floor& currentFloor, const std::set<int>& visitedRoomIds, const DataManager& dataManager, const JudgementTrial& currentTrial, int currentScore, int currentTrialTurn, const std::optional<PrimaryGemType>& manaAffinityChoice, const TrialPerformance& performance) {
    if (currentState == GameState::Trial) {
        turnLimitText.setString("Turns Left: " + std::to_string(currentTrial.turnLimit - currentTrialTurn));
        scoreGoalText.setString("Score Goal: " + std::to_string(currentTrial.scoreGoal));
        currentTrialScoreText.setString("Score: " + std::to_string(currentScore));
    }

    if (currentState == GameState::Exploration) {
        if (currentRoom) {
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

                if (destinationRoom) {
                    sf::RectangleShape button({buttonWidth, buttonHeight});
                    button.setPosition(sf::Vector2f{(WINDOW_WIDTH - buttonWidth) / 2.f, startY + i * (buttonHeight + buttonSpacing)});
                    button.setFillColor(getSfColorForRoomType(destinationRoom->type));
                    button.setOutlineColor(sf::Color(200, 200, 200));
                    button.setOutlineThickness(1.f);
                    doorButtons.push_back(button);

                    std::string buttonTextStr;
                    if (visitedRoomIds.count(destinationRoom->id)) {
                        buttonTextStr = destinationRoom->name;
                    } else {
                        buttonTextStr = roomTypeToString(destinationRoom->type) + " Door";
                    }

                    sf::Text buttonText(font, buttonTextStr, 20);
                    buttonText.setFillColor(sf::Color::White);
                    sf::FloatRect textBounds = buttonText.getLocalBounds();
                    buttonText.setOrigin(sf::Vector2f{textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f});
                    buttonText.setPosition(button.getPosition() + sf::Vector2f{button.getSize().x / 2.f, button.getSize().y / 2.f});
                    doorButtonTexts.push_back(buttonText);
                }
            }
        }
    }
}

void UIManager::render(sf::RenderWindow& window, GameMode gameMode, GameState currentState, bool showPlayerDamageEffect, const JudgementTrial& currentTrial, int currentScore, int currentTrialTurn, const std::optional<PrimaryGemType>& manaAffinityChoice, const TrialPerformance& performance) {


    switch (currentState) {
        case GameState::Intro:
            // Draw specific Intro UI elements here if any
            break;
        case GameState::Trial:
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
            // Render exploration or combat UI elements
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