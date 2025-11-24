#include "PCH.h"
#include "MatchProcessor.h"
#include "Board.h"
#include "DefaultMatchRule.h"
#include "TreasureMergeRule.h" // Will be used in the next phase

MatchProcessor::MatchProcessor() {
    // Rules are processed in order. The first one that applies, wins.
    m_rules.push_back(std::make_unique<TreasureMergeRule>());
    m_rules.push_back(std::make_unique<DefaultMatchRule>());
}

std::optional<std::unique_ptr<MatchResolution>> MatchProcessor::process(const std::vector<sf::Vector2i>& match, Board& board, GemFactory& gemFactory, const DataManager& dataManager, const std::map<GemSubType, sf::Texture>& gemTextures) const {
    for (const auto& rule : m_rules) {
        if (rule->appliesTo(match, board, dataManager)) {
            return std::move(rule->execute(match, board, gemFactory, dataManager, gemTextures));
        }
    }
    return std::nullopt;
}
