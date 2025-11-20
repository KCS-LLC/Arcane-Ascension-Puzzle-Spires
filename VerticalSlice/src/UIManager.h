#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <map>
#include "GemSubType.h"
#include "Judgement.h"
#include "PrimaryGemType.h"
#include "GemSubType.h"

// Forward declarations
class Player;
class Monster;
class DataManager;
struct Room;
struct Attunement;
enum class GameState;

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
    void update(const Player& player, const Monster& monster, GameState currentState, const Room* currentRoom, const std::set<int>& visitedRoomIds, const DataManager& dataManager, const JudgementTrial& currentTrial, int currentScore, int currentTrialTurn, const std::optional<PrimaryGemType>& manaAffinityChoice);
    void render(sf::RenderWindow& window, GameState currentState, bool showPlayerDamageEffect, const JudgementTrial& currentTrial, int currentScore, int currentTrialTurn, const std::optional<PrimaryGemType>& manaAffinityChoice, const JudgementResults& results);

    const std::vector<sf::RectangleShape>& getSpellButtons() const;

private:
    sf::Font font;

    // Judgement Screen
    sf::Text judgementTitle;

    // Judgement Trial UI
    sf::Text trialTypeText;
    sf::Text turnLimitText;
    sf::Text scoreGoalText;
    sf::Text currentTrialScoreText;
    sf::Text manaAffinityPromptText;
    sf::Text manaAffinityChoiceText;
    sf::RectangleShape manaAffinityHighlight;
    sf::Text judgementSummaryTitle;
    sf::Text judgementResultsText;
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
    std::map<GemSubType, sf::RectangleShape> manaBarBacks;
    std::map<GemSubType, sf::RectangleShape> manaBarFronts;

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
