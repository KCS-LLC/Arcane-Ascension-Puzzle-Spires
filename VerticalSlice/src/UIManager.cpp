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

    judgementSummaryTitle.setFillColor(sf::Color::White);
    judgementResultsText.setFillColor(sf::Color::White);
    judgementSummaryTitle.setPosition(sf::Vector2f{ 450, 200 });
    judgementResultsText.setPosition(sf::Vector2f{ 450, 250 });

    // ... other initializations ...
}

bool UIManager::handleEvent(const sf::Event& event, GameState currentState, const Room* currentRoom, const std::vector<Attunement>& attunements, UIAction& outAction) {
    // Placeholder logic for UI event handling
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

    // ... other setup ...
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
    turnLimitText.setString("Turns Left: " + std::to_string(trial.turnLimit));
    scoreGoalText.setString("Score Goal: " + std::to_string(trial.scoreGoal));
}

void UIManager::update(const Player& player, const Monster& monster, GameState currentState, const Room* currentRoom, const std::set<int>& visitedRoomIds, const DataManager& dataManager, const JudgementTrial& currentTrial, int currentScore, int currentTrialTurn, const std::optional<PrimaryGemType>& manaAffinityChoice, const JudgementResults& results) {
    // Update Trial UI text
    if (currentState == GameState::Trial || currentState == GameState::AttunementReveal) {
        trialTypeText.setString(trialTypeToString(currentTrial.type));
        trialObjectiveText.setString(currentTrial.objective);
        scoreGoalText.setString("Goal: " + std::to_string(currentTrial.scoreGoal));
        turnLimitText.setString("Turns: " + std::to_string(currentTrialTurn) + "/" + std::to_string(currentTrial.turnLimit));
        currentTrialScoreText.setString("Score: " + std::to_string(currentScore));
    }
}

void UIManager::render(sf::RenderWindow& window, GameState currentState, bool showPlayerDamageEffect, const JudgementTrial& currentTrial, int currentScore, int currentTrialTurn, const std::optional<PrimaryGemType>& manaAffinityChoice, const JudgementResults& results) {



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

            // Add other trial-specific UI elements here

            break;

        case GameState::Summary:
            judgementSummaryTitle.setString("Trial Complete");
            judgementResultsText.setString("Final Score: " + std::to_string(currentScore));
            window.draw(judgementSummaryTitle);
            window.draw(judgementResultsText);
            break;

        case GameState::AttunementReveal:

            // Draw Attunement Reveal UI elements here

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

    // ... other rendering logic ...
}

const std::vector<sf::RectangleShape>& UIManager::getSpellButtons() const {
    return spellButtons;
}