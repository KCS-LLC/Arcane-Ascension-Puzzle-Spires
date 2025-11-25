#include "PCH.h"
#include "EffectProcessor.h"
#include "Player.h"
#include "Monster.h"
#include "Board.h"
#include "StringUtils.h"
#include "GemFactory.h" // Include for GemFactory
#include "Game.h"       // Include for gemTextures global

bool EffectProcessor::processEffect(const Effect& effect, Player& player, Monster& monster, Board& board, GemFactory& gemFactory, const std::map<GemSubType, sf::Texture>& gemTextures) {
    if (effect.type == "DEAL_DAMAGE") {
        if (effect.params.count("amount")) {
            monster.takeDamage(effect.params.at("amount").get<int>());
        }
    } else if (effect.type == "HEAL_PLAYER") {
        if (effect.params.count("amount")) {
            player.heal(effect.params.at("amount").get<int>());
        }
    } else if (effect.type == "ROTATE_ROW_COLUMN") {
        // Placeholder for UI interaction
        board.rotateRow(0, 1);
        return true; // Board was altered
    } else if (effect.type == "REMOVE_RANDOM_GEMS") {
        if (effect.params.count("amount")) {
            int amount = effect.params.at("amount").get<int>();
            std::vector<sf::Vector2i> coords = board.getRandomGemCoords(amount);
            std::set<sf::Vector2i, Vector2iCompare> gemsToRemove(coords.begin(), coords.end());
            board.removeGems(gemsToRemove);
            return true; // Board was altered
        }
    } else if (effect.type == "TRANSFORM_RANDOM_GEMS") {
        if (effect.params.count("amount") && effect.params.count("to_type")) {
            int amount = effect.params.at("amount").get<int>();
            std::string toTypeStr = effect.params.at("to_type").get<std::string>();
            GemSubType toType = stringToGemSubType(toTypeStr);
            
            std::vector<sf::Vector2i> coords = board.getRandomGemCoords(amount, true);
            for (const auto& coord : coords) {
                if (gemTextures.count(toType)) {
                    board.setGemAt(coord.x, coord.y, gemFactory.createGem(toType, gemTextures.at(toType)));
                }
            }
            return true; // Board was altered
        }
    }
    
    // Other effects will be added here

    return false; // No board alteration by default
}
