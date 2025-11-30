#include "PCH.h"
#include "EffectProcessor.h"
#include "Player.h"
#include "Monster.h"
#include "Board.h"
#include "StringUtils.h"
#include "GemFactory.h" // Include for GemFactory
#include "Game.h"       // Include for gemTextures global
#include "Constants.h" // For BASE_SWAP_SPEED
#include "AttackGem.h"

std::vector<sf::Vector2i> EffectProcessor::processEffect(const Spell& spell, const Effect& effect, Game& game) {
    if (effect.type == "DEAL_DAMAGE") {
        if (effect.params.count("amount") != 0) {
            game.getMonster().takeDamage(effect.params.at("amount").get<int>());
        }
    } else if (effect.type == "HEAL_PLAYER") {
        if (effect.params.count("amount") != 0) {
            game.getPlayer().heal(effect.params.at("amount").get<int>());
        }
    } else if (effect.type == "ROTATE_ROW_COLUMN") {
        TargetingRequest request;
        request.numberOfClicks = 2;
        request.type = TargetType::Row;
        request.abilityId = "gust_of_wind";
        game.startTargeting(request);
    } else if (effect.type == "REMOVE_RANDOM_GEMS") {
        if (effect.params.count("amount") != 0) {
            int amount = effect.params.at("amount").get<int>();
            return game.getBoard().getRandomGemCoords(amount); // Return coords to be removed
        }
    } else if (effect.type == "TRANSFORM_RANDOM_GEMS") {
        if (effect.params.count("amount") != 0 && effect.params.count("to_type") != 0) {
            int amount = effect.params.at("amount").get<int>();
            std::string toTypeStr = effect.params.at("to_type").get<std::string>();
            GemSubType toType = stringToGemSubType(toTypeStr);
            
            std::vector<sf::Vector2i> coords = game.getBoard().getRandomGemCoords(amount, true);
            game.startTransformAnimation(coords); // Start the animation
            // The actual transformation will happen in Game::update after the animation
        }
    } else if (effect.type == "MODIFY_ATTACK_GEM") {
        std::vector<sf::Vector2i> attackGemCoords = game.getBoard().getRandomGemCoords(1, false, GemSubType::Skull); // Get 1 random Skull gem
        if (!attackGemCoords.empty()) {
            BaseGem* gem = game.getBoard().getGemAt(attackGemCoords[0].x, attackGemCoords[0].y);
            if (gem != nullptr) { // The gem is guaranteed to be a Skull gem, so no need to check again
                gem->levelUp();
                std::cout << "[EFFECT] Sharpened a Skull gem to level " << gem->getLevel() << '\n';
            }
        }
    } else if (effect.type == "HIGHLIGHT_MOVE") {
                auto validMove = game.getBoard().findValidMove();
                if (validMove.has_value()) {
                    game.getBoard().getGemAt(validMove->first.y, validMove->first.x)->setActionState(ActionState::ValidMoveHint);
                    game.getBoard().getGemAt(validMove->second.y, validMove->second.x)->setActionState(ActionState::ValidMoveHint);
                }
            } else if (effect.type == "APPLY_STAT_MODIFIER") {
        ActiveEffect activeEffect;
        activeEffect.effectId = spell.id;
        if (effect.params.count("stat") != 0) {
            activeEffect.modifier = effect.params.at("stat").get<std::string>();
        } else {
            activeEffect.modifier = "generic_buff"; // Default value for effects without a specific stat
        }

        if (effect.params.count("multiplier") != 0) {
            activeEffect.value = effect.params.at("multiplier").get<float>();
        } else if (effect.params.count("amount") != 0) {
            activeEffect.value = effect.params.at("amount").get<float>();
        }
        activeEffect.duration = effect.params.at("duration").get<float>();
        activeEffect.maxDuration = effect.params.at("duration").get<float>();

        game.getPlayer().addEffect(activeEffect);
    } else if (effect.type == "FREE_SWAP") {
        TargetingRequest request;
        request.numberOfClicks = 2;
        request.type = TargetType::Gem;
        request.abilityId = spell.id; // Use the spell's ID
        game.startTargeting(request);
    }
     else if (effect.type == "CREATE_BURNING_TILE") {
        int amount = 1; // Default to 1 burning tile
        if (effect.params.count("amount") != 0) {
            amount = effect.params.at("amount").get<int>();
        }
        std::vector<sf::Vector2i> coords = game.getBoard().getRandomGemCoords(amount);
        for (const auto& coord : coords) {
            BaseGem* gem = game.getBoard().getGemAt(coord.x, coord.y);
            if (gem != nullptr) {
                gem->setStatusEffect(StatusEffect::Burning);
                
                long long currentTime = game.getTimeManager().getCurrentTime().totalTimeUnits;
                const int tickInterval = 20; // Activates every 20 time units
                
                // Schedule activation events
                for (int i = 1; i <= (BURNING_TILE_DURATION / tickInterval) -1; ++i) {
                     game.getTimeManager().addEvent({currentTime + (i * tickInterval), TimeEventType::BurningTile_Activation, coord});
                }

                // Schedule expiration event
                game.getTimeManager().addEvent({currentTime + BURNING_TILE_DURATION, TimeEventType::BurningTile_Expire, coord});
            }
        }
    }
            
            // Other effects will be added here
    return {}; // Return empty vector if no gems were removed
}
