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
    currentTrialScoreText.setFillColor(sf::Color::White);
    
    trialTypeText.setPosition(sf::Vector2f{ 10, 10 });
    trialObjectiveText.setPosition(sf::Vector2f{ 10, 40 });
    currentTrialScoreText.setPosition(sf::Vector2f{ 650, 10 });

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
    if (currentState == GameState::Trial) {
        currentTrialScoreText.setString("Score: " + std::to_string(currentScore));
        turnLimitText.setString("Turns Left: " + std::to_string(currentTrial.turnLimit - currentTrialTurn));
    }

    // ... other update logic ...
}

void UIManager::render(sf::RenderWindow& window, GameState currentState, bool showPlayerDamageEffect, const JudgementTrial& currentTrial, int currentScore, int currentTrialTurn, const std::optional<PrimaryGemType>& manaAffinityChoice, const JudgementResults& results) {
    // Simplified rendering for now, based on current GameState
    switch (currentState) {
        case GameState::Intro:
        case GameState::Trial:
        case GameState::Summary:
        case GameState::AttunementReveal:
            window.draw(trialTypeText);
            window.draw(trialObjectiveText);
            window.draw(turnLimitText);
            window.draw(scoreGoalText);
            window.draw(currentTrialScoreText);
            break;
        case GameState::Playing: // Represents Exploration, Combat, etc.
            // Render exploration or combat UI elements
            break;
        case GameState::GameOver:
            window.draw(gameOverText);
            break;
        default:
            // Render general UI elements or nothing
            break;
    }

    // ... other rendering logic ...
}

const std::vector<sf::RectangleShape>& UIManager::getSpellButtons() const {
    return spellButtons;
}