#pragma once

#include <string>
#include <vector>
#include <map>
#include "Gem.h"

// Represents the type of tactical trial
enum class JudgementTrialType {
    Tactical,
    ManaAffinity,
    Power,
    Haste,
    Control
};

// Defines a single trial for the Judgement phase
struct JudgementTrial {
    std::string trialId;
    std::string objective;
    JudgementTrialType type;
    std::vector<std::vector<Gem>> boardLayout;
    int turnLimit = 0;
    int timeLimit = 0;
    int scoreGoal = 0;
};

// Stores the player's performance across all Judgement trials
struct JudgementResults {
    std::map<std::string, int> trialScores;
    int totalTreasureValue = 0;
};