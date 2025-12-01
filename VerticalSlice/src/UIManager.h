#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <map>
#include "GemSubType.h"
#include "Judgement.h"
#include "PrimaryGemType.h"
#include "GemSubType.h"
#include "Structs.h"
#include "Animation.h"

// Forward declarations
class Player;
class Monster;
class DataManager;
class TimeManager;
struct Room;
struct Attunement;
enum class GameState;

// A structure to represent a command sent from the UI to the Game
enum class UIActionType { CastSpell, ChangeRoom, SelectAttunement, JudgementComplete };
struct UIAction {
    UIActionType type;
    int spellIndex = -1;
    int destinationRoomId = -1;
    std::string attunementId;
};

class UIManager {
public:
    UIManager(const sf::Font& font);

    bool handleEvent(const sf::Event& event, GameMode gameMode, GameState currentState, const Room* currentRoom, const std::vector<Attunement>& attunements, UIAction& outAction);
    void setup(const Player& player, const sf::Vector2u& windowSize, const sf::Vector2f& boardOrigin, const std::vector<Attunement>& attunements);
    void setupTrial(const JudgementTrial& trial);
    void setupTreasureRound();
    void update(const Player& player, const Monster& monster, const TimeManager& timeManager, GameMode gameMode, GameState currentState, PlayMode playMode, const TargetingRequest& targetingRequest, const Room* currentRoom, const Floor& currentFloor, const std::set<int>& visitedRoomIds, const DataManager& dataManager, const JudgementTrial& currentTrial, int currentScore, int currentTrialTurn, const std::optional<PrimaryGemType>& manaAffinityChoice, const TrialPerformance& performance, const std::vector<ActiveEffect>& activeEffects);
    void render(sf::RenderWindow& window, const sf::Vector2f& boardOrigin, GameMode gameMode, GameState currentState, PlayMode playMode, const TargetingRequest& targetingRequest, bool showPlayerDamageEffect, const JudgementTrial& currentTrial, int currentScore, int currentTrialTurn, const std::optional<PrimaryGemType>& manaAffinityChoice, const TrialPerformance& performance, const std::map<GemSubType, sf::Texture>& gemTextures, const std::map<std::string, sf::Texture>& effectIconTextures);
    void renderAnimations(sf::RenderWindow& window, const std::vector<Animation>& animations);

    const std::vector<sf::RectangleShape>& getSpellButtons() const;

    std::string m_activeTargetingSpellId; // Tracks the ID of the spell currently in targeting mode.

private:
    sf::Font font;
    sf::Clock m_pulseClock;
    std::vector<ActiveEffect> m_activeEffectsToRender;

    // Judgement Trial UI
    sf::Text trialTypeText;
    sf::Text trialObjectiveText;
    sf::Text turnLimitText;
    sf::Text scoreGoalText;
    sf::Text currentTrialScoreText;
    sf::Text manaAffinityPromptText;
    sf::Text manaAffinityChoiceText;
    sf::RectangleShape manaAffinityHighlight;
    sf::Text judgementSummaryTitle;
    sf::Text judgementResultsText;
    sf::Text attunementTitleText;
    sf::Text attunementNameText;
    sf::Text attunementDescriptionText;

    // Exploration UI Elements
    sf::Text m_roomNameText;
    sf::Text m_roomDescriptionText;

    // Titles
    sf::Text playerPanelTitle;
    sf::Text monsterPanelTitle;
    sf::Text monsterNameText;
    sf::Text monsterHpText;
    sf::Text manaTitle;

    // Gauges
    sf::RectangleShape playerHpBarBack;
    sf::RectangleShape playerHpBarFront;
    sf::RectangleShape monsterHpBarBack;
    sf::RectangleShape monsterHpBarFront;
    std::map<GemSubType, sf::RectangleShape> manaBarBacks;
    std::map<GemSubType, sf::RectangleShape> manaBarFronts;
    std::map<GemSubType, sf::Text> manaBarTexts;

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

    // Attunement Selection UI
    sf::Text m_attunementSelectionTitle;
    std::vector<sf::RectangleShape> m_attunementButtons;
    std::vector<sf::Text> m_attunementButtonTexts;

    // Room Exits
    std::vector<sf::RectangleShape> doorButtons;
    std::vector<sf::Text> doorButtonTexts;
    std::vector<Teleporter> m_currentConnections;
    sf::Text m_quickSwapInstructionText;
    sf::Text m_targetingPromptText;
    sf::Text m_dateText;
    sf::Text m_timeText;
};

#endif // UIMANAGER_H