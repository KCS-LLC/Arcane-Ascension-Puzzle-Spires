#include "PCH.h"
#include "EffectProcessor.h"
#include "Player.h"
#include "Monster.h"
#include "Board.h"
#include "StringUtils.h"
#include "GemFactory.h" // Include for GemFactory
#include "Game.h"       // Include for gemTextures global
#include "Constants.h" // For BASE_SWAP_SPEED

std::vector<sf::Vector2i> EffectProcessor::processEffect(const Effect& effect, Player& player, Monster& monster, Board& board, GemFactory& gemFactory, const std::map<GemSubType, sf::Texture>& gemTextures) {
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
    } else if (effect.type == "REMOVE_RANDOM_GEMS") {
        if (effect.params.count("amount")) {
            int amount = effect.params.at("amount").get<int>();
            return board.getRandomGemCoords(amount); // Return coords to be removed
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
        }
            } else if (effect.type == "HIGHLIGHT_MOVE") {
                auto validMove = board.findValidMove();
                if (validMove) {
                    board.getGemAt(validMove->first.y, validMove->first.x)->setActionState(ActionState::ValidMoveHint);
                    board.getGemAt(validMove->second.y, validMove->second.x)->setActionState(ActionState::ValidMoveHint);
                }
            } else if (effect.type == "CREATE_BURNING_TILE") {
                int amount = 1; // Default to 1 burning tile
                if (effect.params.count("amount")) {
                    amount = effect.params.at("amount").get<int>();
                }
                std::vector<sf::Vector2i> coords = board.getRandomGemCoords(amount);
                for (const auto& coord : coords) {
                    BaseGem* gem = board.getGemAt(coord.x, coord.y);
                    if (gem) {
                        gem->setStatusEffect(StatusEffect::Burning);
                        gem->setEffectValue(3.0f * BASE_SWAP_SPEED);
                        gem->setEffectMax(3.0f * BASE_SWAP_SPEED);
                        gem->setPeriodicActivationValue(BASE_SWAP_SPEED);
                    }
                }
            }
            
            // Other effects will be added here
    return {}; // Return empty vector if no gems were removed
}
