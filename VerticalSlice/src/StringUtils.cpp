#include "PCH.h"
#include "StringUtils.h"
#include "Structs.h"
#include "Judgement.h" // Include Judgement.h for JudgementTrialType
#include <algorithm>
#include <sstream>

std::string gemSubtypeToString(GemSubType type) {
    // This function will need to be updated with all gem types
    switch (type) {
        case GemSubType::Fire: return "fire";
        case GemSubType::Water: return "water";
        // ... other gem types
        default: return "unknown";
    }
}

GemSubType stringToGemSubType(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    if (lowerStr == "fire") { return GemSubType::Fire; }
    if (lowerStr == "water") { return GemSubType::Water; }
    if (lowerStr == "earth") { return GemSubType::Earth; }
    if (lowerStr == "air") { return GemSubType::Air; }
    if (lowerStr == "light") { return GemSubType::Light; }
    if (lowerStr == "life") { return GemSubType::Life; }
    if (lowerStr == "enhancement") { return GemSubType::Enhancement; }
    if (lowerStr == "transference") { return GemSubType::Transference; }
    if (lowerStr == "death") { return GemSubType::Death; }
    if (lowerStr == "umbral") { return GemSubType::Umbral; }
    if (lowerStr == "mental") { return GemSubType::Mental; }
    if (lowerStr == "perception") { return GemSubType::Perception; }
    if (lowerStr == "coin") { return GemSubType::Coin; }
    if (lowerStr == "skull" || lowerStr == "attack") { return GemSubType::Skull; }
    if (lowerStr == "raw") { return GemSubType::Raw; }
    if (lowerStr == "grey") { return GemSubType::Grey; }
    return GemSubType::Empty; // Default case
}

std::string roomTypeToString(RoomType type) {
    switch (type) {
        case RoomType::Entrance: return "Entrance";
        case RoomType::Combat: return "Combat";
        case RoomType::Treasure: return "Treasure";
        case RoomType::Boss: return "Boss";
        case RoomType::Sanctuary: return "Sanctuary";
        case RoomType::Special: return "Special";
        case RoomType::Puzzle: return "Puzzle";
        case RoomType::Trap: return "Trap";
        case RoomType::AgilityChallenge: return "Agility Challenge";
        case RoomType::EnduranceChallenge: return "Endurance Challenge";
        case RoomType::MagicChallenge: return "Magic Challenge";
        default: return "Unknown";
    }
}

std::string doorColorToString(DoorColor color) {
    switch (color) {
        case DoorColor::Red: return "Red";
        case DoorColor::Orange: return "Orange";
        case DoorColor::Yellow: return "Yellow";
        case DoorColor::Green: return "Green";
        case DoorColor::Blue: return "Blue";
        case DoorColor::Indigo: return "Indigo";
        case DoorColor::Violet: return "Violet";
        case DoorColor::Copper: return "Copper";
        case DoorColor::Silver: return "Silver";
        case DoorColor::Gold: return "Gold";
        case DoorColor::White: return "White";
        default: return "Unknown";
    }
}

sf::Color doorColorToSfColor(DoorColor color) {
    switch (color) {
        case DoorColor::Red: return sf::Color::Red;
        case DoorColor::Orange: return {255, 165, 0};
        case DoorColor::Yellow: return sf::Color::Yellow;
        case DoorColor::Green: return sf::Color::Green;
        case DoorColor::Blue: return sf::Color::Blue;
        case DoorColor::Indigo: return {75, 0, 130};
        case DoorColor::Violet: return {238, 130, 238};
        case DoorColor::Copper: return {184, 115, 51};
        case DoorColor::Silver: return {192, 192, 192};
        case DoorColor::Gold: return {255, 215, 0};
        case DoorColor::White: return sf::Color::White;
        default: return sf::Color::Black;
    }
}

sf::Color getSfColorForRoomType(RoomType type) {
    switch (type) {
        case RoomType::Combat:
            return {139, 0, 0}; // Dark Red
        case RoomType::AgilityChallenge:
            return {255, 140, 0}; // Dark Orange
        case RoomType::Trap:
            return {255, 215, 0}; // Gold (Yellow)
        case RoomType::EnduranceChallenge:
            return {0, 100, 0}; // Dark Green
        case RoomType::Puzzle:
            return {0, 0, 205}; // Medium Blue
        case RoomType::MagicChallenge:
            return {75, 0, 130}; // Indigo
        case RoomType::Special:
            return {148, 0, 211}; // Dark Violet
        case RoomType::Treasure:
            return {184, 115, 51}; // Copper
        case RoomType::Sanctuary:
        case RoomType::Boss:
            return {245, 245, 245}; // White Smoke
        case RoomType::Entrance:
            return {128, 128, 128}; // Grey
        default:
            return sf::Color::Black;
    }
}

sf::Color getSfColorForGemType(GemSubType type) {
    switch (type) {
        case GemSubType::Fire: return {255, 50, 50};
        case GemSubType::Water: return {50, 50, 255};
        case GemSubType::Earth: return {139, 69, 19};
        case GemSubType::Air: return {173, 216, 230};
        case GemSubType::Light: return {255, 255, 150};
        case GemSubType::Umbral: return {100, 100, 100};
        case GemSubType::Life: return {50, 205, 50};
        case GemSubType::Death: return {138, 43, 226};
        case GemSubType::Mental: return {218, 112, 214};
        case GemSubType::Perception: return {0, 191, 255};
        case GemSubType::Enhancement: return {255, 165, 0};
        case GemSubType::Transference: return {64, 224, 208};
        default: return {128, 128, 128};
    }
}

sf::Color gemLevelToColor(int level) {
    switch (level) {
        case 1: return sf::Color::Transparent; // Level 1: Transparent/Black
        case 2: return sf::Color::White;     // Level 2: White/Clear
        case 3: return sf::Color::Red;
        case 4: return {255, 165, 0}; // Orange
        case 5: return sf::Color::Yellow;
        case 6: return sf::Color::Green;
        case 7: return sf::Color::Blue;
        case 8: return {75, 0, 130};  // Indigo

        default: return {148, 0, 211}; // Default to Violet for levels beyond 9
    }
}

std::string wordWrap(const std::string& text, unsigned int lineLength) {
    std::stringstream ss(text);
    std::string line;
    std::string result;
    std::string word;

    while (ss >> word) {
        if (line.length() + word.length() + 1 > lineLength) {
            result += line + '\n';
            line = "";
        }
        if (!line.empty()) {
            line += " ";
        }
        line += word;
    }
    result += line;
    return result;
}

DoorColor stringToDoorColor(const std::string& str) {
    if (str == "Red") { return DoorColor::Red; }
    if (str == "Orange") { return DoorColor::Orange; }
    if (str == "Yellow") { return DoorColor::Yellow; }
    if (str == "Green") { return DoorColor::Green; }
    if (str == "Blue") { return DoorColor::Blue; }
    if (str == "Indigo") { return DoorColor::Indigo; }
    if (str == "Violet") { return DoorColor::Violet; }
    if (str == "Copper") { return DoorColor::Copper; }
    if (str == "Silver") { return DoorColor::Silver; }
    if (str == "Gold") { return DoorColor::Gold; }
    if (str == "White") { return DoorColor::White; }
    return DoorColor::Unknown;
}

RoomType stringToRoomType(const std::string& str) {
    if (str == "Entrance") { return RoomType::Entrance; }
    if (str == "Combat") { return RoomType::Combat; }
    if (str == "Treasure") { return RoomType::Treasure; }
    if (str == "Boss") { return RoomType::Boss; }
    if (str == "Sanctuary") { return RoomType::Sanctuary; }
    if (str == "Special") { return RoomType::Special; }
    if (str == "Puzzle") { return RoomType::Puzzle; }
    if (str == "Trap") { return RoomType::Trap; }
    if (str == "AgilityChallenge") { return RoomType::AgilityChallenge; }
    if (str == "EnduranceChallenge") { return RoomType::EnduranceChallenge; }
    if (str == "MagicChallenge") { return RoomType::MagicChallenge; }
    return RoomType::Unknown;
}

std::string trialTypeToString(JudgementTrialType type) {
    switch (type) {
        case JudgementTrialType::Tactical: return "Tactical";
        case JudgementTrialType::ManaAffinity: return "Mana Affinity";
        case JudgementTrialType::Power: return "Power";
        case JudgementTrialType::Haste: return "Haste";
        case JudgementTrialType::Control: return "Control";
        default: return "Unknown";
    }
}