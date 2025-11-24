#include "PCH.h"
#include "MatchDetector.h"
#include "Board.h"
#include <set>
#include <algorithm>
#include <iostream>

// --- New Two-Stage Algorithm ---

std::vector<std::vector<sf::Vector2i>> MatchDetector::findAllMatches(const Board& board) const {
    std::vector<std::vector<sf::Vector2i>> simpleRuns;

    // --- Stage 1: Find all simple horizontal and vertical runs ---
    // Horizontal runs
    for (int r = 0; r < board.getHeight(); ++r) {
        for (int c = 0; c < board.getWidth(); ) {
            const BaseGem* gem = board.getGemAt(r, c);
            if (!gem) {
                c++;
                continue;
            }
            GemSubType type = gem->getSubType();
            if (type == GemSubType::Empty) {
                c++;
                continue;
            }

            std::vector<sf::Vector2i> currentRun;
            int runner = c;
            while (runner < board.getWidth()) {
                const BaseGem* nextGem = board.getGemAt(r, runner);
                if (nextGem && nextGem->getSubType() == type) {
                    currentRun.push_back({r, runner});
                    runner++;
                } else {
                    break;
                }
            }

            if (currentRun.size() >= 3) {
                simpleRuns.push_back(currentRun);
            }
            c = runner;
        }
    }

    // Vertical runs
    for (int c = 0; c < board.getWidth(); ++c) {
        for (int r = 0; r < board.getHeight(); ) {
            const BaseGem* gem = board.getGemAt(r, c);
            if (!gem) {
                r++;
                continue;
            }
            GemSubType type = gem->getSubType();
            if (type == GemSubType::Empty) {
                r++;
                continue;
            }

            std::vector<sf::Vector2i> currentRun;
            int runner = r;
            while (runner < board.getHeight()) {
                const BaseGem* nextGem = board.getGemAt(runner, c);
                if (nextGem && nextGem->getSubType() == type) {
                    currentRun.push_back({runner, c});
                    runner++;
                } else {
                    break;
                }
            }
            
            if (currentRun.size() >= 3) {
                simpleRuns.push_back(currentRun);
            }
            r = runner;
        }
    }
    
    if (simpleRuns.empty()) {
        return {};
    }

    // --- Stage 2: Merge overlapping runs into groups ---
    std::vector<std::vector<sf::Vector2i>> mergedGroups;
    if (simpleRuns.empty()) return mergedGroups;

    std::vector<bool> processed(simpleRuns.size(), false);
    for (size_t i = 0; i < simpleRuns.size(); ++i) {
        if (processed[i]) continue;

        // Use a set to automatically handle duplicates from overlapping runs
        std::set<sf::Vector2i, Vector2iCompare> currentGroupSet;
        std::vector<size_t> toProcess;
        
        toProcess.push_back(i);
        processed[i] = true;

        while(!toProcess.empty()) {
            size_t currentIndex = toProcess.back();
            toProcess.pop_back();

            // Add all coords from this run to the set
            for(const auto& coord : simpleRuns[currentIndex]) {
                currentGroupSet.insert(coord);
            }

            // Check for overlaps with all other unprocessed runs
            for (size_t j = 0; j < simpleRuns.size(); ++j) {
                if (processed[j]) continue;

                bool overlaps = false;
                for (const auto& coord1 : simpleRuns[currentIndex]) {
                    for (const auto& coord2 : simpleRuns[j]) {
                        if (coord1 == coord2) {
                            overlaps = true;
                            break;
                        }
                    }
                    if (overlaps) break;
                }

                if (overlaps) {
                    toProcess.push_back(j);
                    processed[j] = true;
                }
            }
        }
        
        // Convert the set back to a vector for the final result
        mergedGroups.emplace_back(currentGroupSet.begin(), currentGroupSet.end());
    }

    return mergedGroups;
}
