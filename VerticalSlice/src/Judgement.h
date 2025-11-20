#pragma once

#include <string>
#include <vector>
#include <map>

// Represents the type of tactical trial
enum class JudgementTrialType {
    Power,
    Haste,
    Control
};

// Defines a single trial for the Judgement phase
struct JudgementTrial {
    JudgementTrialType type;
    std::vector<std::vector<Gem>> boardLayout; // 2D vector to store the actual board state
    int turnLimit = 0;
    int scoreGoal = 0;
};

// Stores the player's performance across all Judgement trials
struct JudgementResults {
    std::map<JudgementTrialType, int> trialScores;
    int totalTreasureValue = 0;
};