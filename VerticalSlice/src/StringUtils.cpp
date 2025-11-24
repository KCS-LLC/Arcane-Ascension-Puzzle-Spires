#include "PCH.h"
#include "StringUtils.h"
#include "Structs.h"
#include "Judgement.h" // Include Judgement.h for JudgementTrialType
#include <algorithm>
#include <sstream>

std::string gemSubTypeToString(GemSubType type) {
    // This function will need to be updated with all gem types
    switch (type) {
        case GemSubType::Fire: return "fire";
        case GemSubType::Water: return "water";
        // ... other gem types
        default: return "unknown";
    }
}

GemSubType stringToGemSubType(const std::string& s) {
    std::string lower_s = s;
    std::transform(lower_s.begin(), lower_s.end(), lower_s.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    if (lower_s == "fire") return GemSubType::Fire;
    if (lower_s == "water") return GemSubType::Water;
    if (lower_s == "earth") return GemSubType::Earth;
    if (lower_s == "air") return GemSubType::Air;
    if (lower_s == "light") return GemSubType::Light;
    if (lower_s == "life") return GemSubType::Life;
    if (lower_s == "enhancement") return GemSubType::Enhancement;
    if (lower_s == "transference") return GemSubType::Transference;
    if (lower_s == "death") return GemSubType::Death;
    if (lower_s == "umbral") return GemSubType::Umbral;
    if (lower_s == "mental") return GemSubType::Mental;
    if (lower_s == "perception") return GemSubType::Perception;
    if (lower_s == "coin") return GemSubType::Coin;
    if (lower_s == "skull") return GemSubType::Skull;
    if (lower_s == "raw") return GemSubType::Raw;
    if (lower_s == "grey") return GemSubType::Grey;
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
        case DoorColor::Orange: return sf::Color(255, 165, 0);
        case DoorColor::Yellow: return sf::Color::Yellow;
        case DoorColor::Green: return sf::Color::Green;
        case DoorColor::Blue: return sf::Color::Blue;
        case DoorColor::Indigo: return sf::Color(75, 0, 130);
        case DoorColor::Violet: return sf::Color(238, 130, 238);
        case DoorColor::Copper: return sf::Color(184, 115, 51);
        case DoorColor::Silver: return sf::Color(192, 192, 192);
        case DoorColor::Gold: return sf::Color(255, 215, 0);
        case DoorColor::White: return sf::Color::White;
        default: return sf::Color::Black;
    }
}

sf::Color getSfColorForRoomType(RoomType type) {
    switch (type) {
        case RoomType::Combat:
            return sf::Color(139, 0, 0); // Dark Red
        case RoomType::AgilityChallenge:
            return sf::Color(255, 140, 0); // Dark Orange
        case RoomType::Trap:
            return sf::Color(255, 215, 0); // Gold (Yellow)
        case RoomType::EnduranceChallenge:
            return sf::Color(0, 100, 0); // Dark Green
        case RoomType::Puzzle:
            return sf::Color(0, 0, 205); // Medium Blue
        case RoomType::MagicChallenge:
            return sf::Color(75, 0, 130); // Indigo
        case RoomType::Special:
            return sf::Color(148, 0, 211); // Dark Violet
        case RoomType::Treasure:
            return sf::Color(184, 115, 51); // Copper
        case RoomType::Sanctuary:
        case RoomType::Boss:
            return sf::Color(245, 245, 245); // White Smoke
        case RoomType::Entrance:
            return sf::Color(128, 128, 128); // Grey
        default:
            return sf::Color::Black;
    }
}

sf::Color getSfColorForGemType(GemSubType type) {
    switch (type) {
        case GemSubType::Fire: return sf::Color(255, 50, 50);
        case GemSubType::Water: return sf::Color(50, 50, 255);
        case GemSubType::Earth: return sf::Color(139, 69, 19);
        case GemSubType::Air: return sf::Color(173, 216, 230);
        case GemSubType::Light: return sf::Color(255, 255, 150);
        case GemSubType::Umbral: return sf::Color(100, 100, 100);
        case GemSubType::Life: return sf::Color(50, 205, 50);
        case GemSubType::Death: return sf::Color(138, 43, 226);
        case GemSubType::Mental: return sf::Color(218, 112, 214);
        case GemSubType::Perception: return sf::Color(0, 191, 255);
        case GemSubType::Enhancement: return sf::Color(255, 165, 0);
        case GemSubType::Transference: return sf::Color(64, 224, 208);
        default: return sf::Color(128, 128, 128);
    }
}

EffectType stringToEffectType(const std::string& s) {
    if (s == "Damage") return EffectType::Damage;
    if (s == "Heal") return EffectType::Heal;
    if (s == "Stun") return EffectType::Stun;
    if (s == "Shield") return EffectType::Shield;
    return EffectType::Damage; // Default
}

TargetType stringToTargetType(const std::string& s) {
    if (s == "Player") return TargetType::Player;
    if (s == "Monster") return TargetType::Monster;
    return TargetType::Monster; // Default
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

DoorColor stringToDoorColor(const std::string& s) {
    if (s == "Red")    return DoorColor::Red;
    if (s == "Orange") return DoorColor::Orange;
    if (s == "Yellow") return DoorColor::Yellow;
    if (s == "Green")  return DoorColor::Green;
    if (s == "Blue")   return DoorColor::Blue;
    if (s == "Indigo") return DoorColor::Indigo;
    if (s == "Violet") return DoorColor::Violet;
    if (s == "Copper") return DoorColor::Copper;
    if (s == "Silver") return DoorColor::Silver;
    if (s == "Gold")   return DoorColor::Gold;
    if (s == "White")  return DoorColor::White;
    return DoorColor::Unknown;
}

RoomType stringToRoomType(const std::string& s) {
    if (s == "Entrance")  return RoomType::Entrance;
    if (s == "Combat")    return RoomType::Combat;
    if (s == "Treasure")  return RoomType::Treasure;
    if (s == "Boss")      return RoomType::Boss;
    if (s == "Sanctuary") return RoomType::Sanctuary;
    if (s == "Special")   return RoomType::Special;
    if (s == "Puzzle")    return RoomType::Puzzle;
    if (s == "Trap")      return RoomType::Trap;
    if (s == "AgilityChallenge") return RoomType::AgilityChallenge;
    if (s == "EnduranceChallenge") return RoomType::EnduranceChallenge;
    if (s == "MagicChallenge") return RoomType::MagicChallenge;
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