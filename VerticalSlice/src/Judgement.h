#pragma once

#include <string>
#include <vector>
#include "GemSubType.h"

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
    int turnLimit;
    int timeLimit = 0;
    int scoreGoal;
};

// Stores the player's performance across all Judgement trials
struct JudgementResults {
    int powerScore = 0;
    int hasteScore = 0;
    int controlScore = 0;
};

// Struct to hold player's performance across the three tactical trials
struct TrialPerformance {
    int powerScore = 0;
    int hasteScore = 0;
    int controlScore = 0;
};