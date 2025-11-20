#include "PCH.h"
#include "Game.h"
#include "UIManager.h"
#include <random>
#include <SFML/Graphics/Color.hpp>

const int MATCH_SPEED_COST = 30;
const int SPELL_SPEED_COST = 70;

Game::Game()
    : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Arcane Ascension - Prototype"),
      player(100, {}), 
      monster(0, 0),
      isAnimatingSwap(false),
      isAnimatingDestruction(false),
      isAnimatingRefill(false),
      isReshuffling(false),
      isRevertingInvalidSwap(false)
{
    if (!font.openFromFile("assets/OpenSans-Regular.ttf")) {
        std::cerr << "Error loading font." << std::endl;
    }
    uiManager = std::make_unique<UIManager>(font);

    if (!dataManager.loadAttunements("data/attunements.json") ||
        !dataManager.loadSpells("data/spells.json") || 
        !dataManager.loadMonsterData("data/monster.json") ||
        !dataManager.loadFloor("data/floor1.json")) {
        std::cerr << "Failed to load game data." << std::endl;
        window.close();
        return;
    }

    currentFloor = dataManager.getFloor();
    currentRoom = dataManager.getRoomById(currentFloor.startRoomId);
    visitedRoomIds.insert(currentRoom->id);

    monster = Monster(dataManager.getMonsterHP(), dataManager.getMonsterSpeed());
    monster.name = dataManager.getMonsterName();
    
    const int boardPixelWidth = BOARD_WIDTH * TILE_SIZE;
    const int boardPixelHeight = BOARD_HEIGHT * TILE_SIZE;
    boardOrigin.x = (WINDOW_WIDTH - boardPixelWidth) / 2;
    boardOrigin.y = WINDOW_HEIGHT - boardPixelHeight - 20;

    loadTextures();
    uiManager->setup(player, window.getSize(), boardOrigin, dataManager.getAttunements());
    
    initializeJudgement();
    startNextJudgementTrial();
}

void Game::loadTextures() {
    for (const auto& pair : dataManager.getGemDefinitions()) {
        const GemDefinition& gemDef = pair.second;
        if (!gemDef.texturePath.empty()) {
            if (!gemTextures[gemDef.subType].loadFromFile(gemDef.texturePath)) {
                std::cerr << "Error loading texture for gem: " << gemDef.name << " from " << gemDef.texturePath << std::endl;
            }
        }
    }
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    for (std::optional<sf::Event> event = window.pollEvent(); event; event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        if (currentState == GameState::Animating || currentState == GameState::GameOver) continue;

        if (auto* mbp = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (currentState == GameState::Judgement_AttunementReveal) {
                currentState = GameState::Exploration;
                continue;
            }
            if (currentState == GameState::Treasure || currentState == GameState::Special ||
                currentState == GameState::Puzzle || currentState == GameState::Trap ||
                currentState == GameState::Sanctuary || currentState == GameState::AgilityChallenge ||
                currentState == GameState::EnduranceChallenge || currentState == GameState::MagicChallenge)
            {
                currentState = GameState::Exploration;
                continue;
            }

            if (mbp->button != sf::Mouse::Button::Left) continue;
            
            UIAction uiAction;
            if (uiManager->handleEvent(*event, currentState, currentRoom, dataManager.getAttunements(), uiAction)) {
                if (uiAction.type == UIActionType::CastSpell) {
                    int damage = player.castSpell(uiAction.spellIndex);
                    if (damage > 0) {
                        monster.takeDamage(damage);
                        if (monster.isTurnReady(SPELL_SPEED_COST)) {
                            player.takeDamage(dataManager.getMonsterAttackDamage());
                            showPlayerDamageEffect = true;
                            playerDamageClock.restart();
                        }
                    }
                } else if (uiAction.type == UIActionType::ChangeRoom) {
                    moveToRoom(uiAction.destinationRoomId);
                } else if (uiAction.type == UIActionType::JudgementComplete) {
                    player.finalizeJudgement(m_judgementResults, dataManager);
                    currentState = GameState::Judgement_AttunementReveal;
                    // Clear any pending events to prevent accidental click-through
                    while (window.pollEvent()) {}
                }
                continue;
            }
            
            if (currentState == GameState::Playing || currentState == GameState::Judgement_TacticalTrial || currentState == GameState::Judgement_ManaAffinityTrial) {
                const int c = (mbp->position.x - boardOrigin.x) / TILE_SIZE;
                const int r = (mbp->position.y - boardOrigin.y) / TILE_SIZE;
                if (c < 0 || c >= BOARD_WIDTH || r < 0 || r >= BOARD_HEIGHT) continue;

                if (currentState == GameState::Judgement_ManaAffinityTrial && !m_manaAffinityChoice.has_value()) {
                    Gem clickedGem = board.getGem(r, c);
                    if (clickedGem.primaryType == PrimaryGemType::Mana) {
                        m_manaAffinityChoice = clickedGem.primaryType;
                    }
                } else {
                    dragStartTile = sf::Vector2i(c, r);
                    if (selectedTile.has_value() && selectedTile.value() != dragStartTile.value()) {
                        handleSwap(*selectedTile, *dragStartTile);
                        selectedTile = std::nullopt;
                        dragStartTile = std::nullopt;
                    } else {
                        selectedTile = sf::Vector2i(c, r);
                    }
                }
            }
        }

        if (currentState == GameState::Playing) {
            if (auto* mbr = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mbr->button != sf::Mouse::Button::Left) continue;
                if (dragStartTile.has_value()) {
                    const int c = (mbr->position.x - boardOrigin.x) / TILE_SIZE;
                    const int r = (mbr->position.y - boardOrigin.y) / TILE_SIZE;
                    sf::Vector2i endTile(c, r);
                    if (endTile != *dragStartTile) {
                        handleSwap(*dragStartTile, endTile);
                        selectedTile = std::nullopt;
                    }
                    dragStartTile = std::nullopt;
                }
            }
        }
    }
}

void Game::startNewCombat() {
    monster.reset();
    board.initialize(player);
    currentState = GameState::Playing;
}

void Game::handleSwap(sf::Vector2i tile1, sf::Vector2i tile2) {
    if (board.canSwap(tile1.y, tile1.x, tile2.y, tile2.x)) {
        if (currentState == GameState::Judgement_TacticalTrial || currentState == GameState::Judgement_ManaAffinityTrial) {
            m_currentTrialTurn++;
        }
        m_preAnimationState = currentState;
        currentState = GameState::Animating;
        isAnimatingSwap = true;
        animatingGems = {tile1, tile2};
        animationClock.restart();
    }
}

void Game::applyMatchConsequences(const std::vector<Gem>& matchedGems) {
    int totalDamage = 0;
    m_matchedGemsInTurn.clear();
    for (const auto& gem : matchedGems) {
        m_matchedGemsInTurn.push_back(gem);

        if (m_preAnimationState == GameState::Judgement_TacticalTrial) {
            m_currentScore += 10;
        } else if (m_preAnimationState == GameState::Judgement_ManaAffinityTrial && m_manaAffinityChoice.has_value() && gem.primaryType == m_manaAffinityChoice.value()) {
            m_currentAffinityScore += gem.level;
        }

        switch (gem.primaryType) {
            case PrimaryGemType::Attack:
                totalDamage += gem.level * 10;
                break;
            case PrimaryGemType::Mana:
                player.addMana(gem.subType, gem.level);
                break;
            case PrimaryGemType::Treasure:
                break;
            default:
                break;
        }
    }
    if (totalDamage > 0 && m_preAnimationState == GameState::Playing) {
        monster.takeDamage(totalDamage);
    }
}

void Game::update() {
    const float swapAnimationDuration = 0.2f;
    const float destructionAnimationDuration = 0.3f;
    const float refillAnimationDuration = 0.3f;
    const float reshuffleAnimationDuration = 0.5f;

    if (currentState == GameState::Animating) {
        if (isAnimatingSwap) {
            if (animationClock.getElapsedTime().asSeconds() >= swapAnimationDuration) {
                isAnimatingSwap = false;
                sf::Vector2i t1 = animatingGems.first;
                sf::Vector2i t2 = animatingGems.second;
                board.swapGems(t1.y, t1.x, t2.y, t2.x);

                if (isRevertingInvalidSwap) {
                    isRevertingInvalidSwap = false;
                    currentState = m_preAnimationState;
                    return;
                }

                auto matches = board.findMatches();
                if (!matches.empty()) {
                    std::vector<Gem> matchedGems;
                    board.processMatches(matches, matchedGems);
                    applyMatchConsequences(matchedGems);
                    
                    if (m_preAnimationState == GameState::Playing && monster.isTurnReady(MATCH_SPEED_COST)) {
                        player.takeDamage(dataManager.getMonsterAttackDamage());
                        showPlayerDamageEffect = true;
                        playerDamageClock.restart();
                    }
                    isAnimatingDestruction = true;
                    destroyingGems = matches;
                    animationClock.restart();
                } else {
                    isAnimatingSwap = true;
                    isRevertingInvalidSwap = true;
                    animatingGems = {t2, t1};
                    animationClock.restart();
                }
                if (!isAnimatingSwap && !isAnimatingDestruction) currentState = m_preAnimationState;
            }
        } else if (isAnimatingDestruction) {
            if (animationClock.getElapsedTime().asSeconds() >= destructionAnimationDuration) {
                isAnimatingDestruction = false;
                if (m_preAnimationState == GameState::Judgement_TacticalTrial || m_preAnimationState == GameState::Judgement_ManaAffinityTrial) {
                    const std::vector<GemSubType> trialGems = {
                        GemSubType::Fire, GemSubType::Water, GemSubType::Earth, 
                        GemSubType::Air, GemSubType::Light, GemSubType::Umbral,
                        GemSubType::Skull
                    };
                    fallInfo = board.applyGravityAndRefill(trialGems);
                } else {
                    fallInfo = board.applyGravityAndRefill(player);
                }
                isAnimatingRefill = true;
                animationClock.restart();
            }
        } else if (isAnimatingRefill) {
            if (animationClock.getElapsedTime().asSeconds() >= refillAnimationDuration) {
                isAnimatingRefill = false;
                auto newMatches = board.findMatches();
                if (!newMatches.empty()) {
                    std::vector<Gem> matchedGems;
                    board.processMatches(newMatches, matchedGems);
                    applyMatchConsequences(matchedGems);
                    isAnimatingDestruction = true;
                    destroyingGems = newMatches;
                    animationClock.restart();
                } else if (board.findAllValidSwaps().empty()) {
                    isReshuffling = true;
                    animationClock.restart();
                } else {
                    currentState = m_preAnimationState;
                }
            }
        } else if (isReshuffling) {
            if (animationClock.getElapsedTime().asSeconds() >= reshuffleAnimationDuration) {
                isReshuffling = false;
                board.initialize(player);
                currentState = m_preAnimationState;
            }
        }
    }

    if (currentState == GameState::Playing) {
        if (player.getCurrentHp() <= 0) {
            currentState = GameState::GameOver;
        } else if (monster.getCurrentHp() <= 0) {
            clearedRoomIds.insert(currentRoom->id);
            currentState = GameState::Exploration;
        }
    } else if (currentState == GameState::Judgement_TacticalTrial) {
        if (m_currentJudgementTrial.type == JudgementTrialType::Haste) {
            if (m_trialTimer.getElapsedTime().asSeconds() >= m_currentJudgementTrial.timeLimit) {
                m_judgementResults.trialScores[m_currentJudgementTrial.trialId] = m_currentScore;
                if (m_currentJudgementTrialIndex < m_pendingJudgementTrials.size() - 1) {
                    m_currentJudgementTrialIndex++;
                    startNextJudgementTrial();
                } else {
                    currentState = GameState::Judgement_Summary;
                }
            }
        } else {
            if (m_currentTrialTurn >= m_currentJudgementTrial.turnLimit || m_currentScore >= m_currentJudgementTrial.scoreGoal) {
                m_judgementResults.trialScores[m_currentJudgementTrial.trialId] = m_currentScore;
                if (m_currentJudgementTrialIndex < m_pendingJudgementTrials.size() - 1) {
                    m_currentJudgementTrialIndex++;
                    startNextJudgementTrial();
                } else {
                    currentState = GameState::Judgement_Summary;
                }
            }
        }
    } else if (currentState == GameState::Judgement_ManaAffinityTrial) {
        if (m_manaAffinityChoice.has_value() && (m_currentTrialTurn >= m_currentJudgementTrial.turnLimit || m_currentAffinityScore >= m_currentJudgementTrial.scoreGoal)) {
            m_judgementResults.trialScores[m_currentJudgementTrial.trialId] = m_currentAffinityScore;
            if (m_currentJudgementTrialIndex < m_pendingJudgementTrials.size() - 1) {
                m_currentJudgementTrialIndex++;
                startNextJudgementTrial();
            } else {
                currentState = GameState::Judgement_Summary;
            }
        }
    }

    if (showPlayerDamageEffect && playerDamageClock.getElapsedTime().asMilliseconds() > 200) {
        showPlayerDamageEffect = false;
    }

    uiManager->update(player, monster, currentState, currentRoom, visitedRoomIds, dataManager, m_currentJudgementTrial, m_currentScore, m_currentTrialTurn, m_manaAffinityChoice, m_judgementResults);
}

void Game::render() {
    window.clear(sf::Color(50, 50, 50));
    uiManager->render(window, currentState, m_preAnimationState, showPlayerDamageEffect, m_currentJudgementTrial, m_currentScore, m_currentTrialTurn, m_manaAffinityChoice, m_judgementResults);

    if (currentState == GameState::Playing || currentState == GameState::Animating || currentState == GameState::GameOver || currentState == GameState::Judgement_TacticalTrial || currentState == GameState::Judgement_ManaAffinityTrial) {
        if (!isReshuffling) {
            for (int r = 0; r < BOARD_HEIGHT; ++r) {
                for (int c = 0; c < BOARD_WIDTH; ++c) {
                    bool shouldDraw = true;
                    if (isAnimatingSwap && ((r == animatingGems.first.y && c == animatingGems.first.x) || (r == animatingGems.second.y && c == animatingGems.second.x))) shouldDraw = false;
                    if (isAnimatingDestruction && destroyingGems.count({r, c})) shouldDraw = false;
                    if (isAnimatingRefill) {
                        for (const auto& info : fallInfo) if (info.col == c && info.endRow == r) { shouldDraw = false; break; }
                    }
                    if (shouldDraw) {
                        Gem gem = board.getGem(r, c);
                        if (gem.primaryType != PrimaryGemType::Empty) {
                            if (gemTextures.count(gem.subType)) {
                                const sf::Texture& texture = gemTextures.at(gem.subType);
                                sf::Sprite sprite(texture);
                                float scale = (float)TILE_SIZE / texture.getSize().x;
                                sprite.setScale({scale, scale});
                                sprite.setPosition({(float)boardOrigin.x + c * TILE_SIZE, (float)boardOrigin.y + r * TILE_SIZE});

                                // Special gem pulsing for Control Trial
                                if (m_currentJudgementTrial.type == JudgementTrialType::Control && gem.subType == GemSubType::Transference) {
                                    float wave = sin(m_pulseClock.getElapsedTime().asSeconds() * 5.f);
                                    unsigned char alpha = static_cast<unsigned char>(128 + 127 * wave);
                                    sprite.setColor(sf::Color(255, 255, 255, alpha));
                                }

                                window.draw(sprite);
                            }
                        }
                    }
                }
            }
        }

        if (isAnimatingSwap) {
            float p = std::min(1.f, animationClock.getElapsedTime().asSeconds() / 0.2f);
            sf::Vector2f p1((float)animatingGems.first.x * TILE_SIZE + boardOrigin.x, (float)animatingGems.first.y * TILE_SIZE + boardOrigin.y);
            sf::Vector2f p2((float)animatingGems.second.x * TILE_SIZE + boardOrigin.x, (float)animatingGems.second.y * TILE_SIZE + boardOrigin.y);
            
            Gem g1 = board.getGem(animatingGems.first.y, animatingGems.first.x);
            if (gemTextures.count(g1.subType)) {
                const sf::Texture& tex1 = gemTextures.at(g1.subType);
                sf::Sprite s1(tex1);
                float scale1 = (float)TILE_SIZE / tex1.getSize().x;
                s1.setScale({scale1, scale1});
                s1.setPosition(p1 + (p2 - p1) * p);
                window.draw(s1);
            }

            Gem g2 = board.getGem(animatingGems.second.y, animatingGems.second.x);
            if (gemTextures.count(g2.subType)) {
                const sf::Texture& tex2 = gemTextures.at(g2.subType);
                sf::Sprite s2(tex2);
                float scale2 = (float)TILE_SIZE / tex2.getSize().x;
                s2.setScale({scale2, scale2});
                s2.setPosition(p2 + (p1 - p2) * p);
                window.draw(s2);
            }
        }

        if (isAnimatingDestruction) {
            float animProgress = std::min(1.f, animationClock.getElapsedTime().asSeconds() / 0.3f);
            float scale = 1.f - animProgress;
            for (const auto& pos : destroyingGems) {
                Gem gem = board.getGem(pos.first, pos.second);
                 if (gemTextures.count(gem.subType)) {
                    const sf::Texture& texture = gemTextures.at(gem.subType);
                    sf::Sprite sprite(texture);
                    float base_scale = (float)TILE_SIZE / texture.getSize().x;
                    sprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
                    sprite.setScale({base_scale * scale, base_scale * scale});
                    sprite.setPosition({(float)boardOrigin.x + pos.second * TILE_SIZE + TILE_SIZE / 2.f, (float)boardOrigin.y + pos.first * TILE_SIZE + TILE_SIZE / 2.f});
                    window.draw(sprite);
                }
            }
        }

        if (isAnimatingRefill) {
            float p = std::min(1.f, animationClock.getElapsedTime().asSeconds() / 0.3f);
            for (const auto& info : fallInfo) {
                sf::Vector2f start((float)boardOrigin.x + info.col * TILE_SIZE, (float)boardOrigin.y + info.startRow * TILE_SIZE);
                sf::Vector2f end((float)boardOrigin.x + info.col * TILE_SIZE, (float)boardOrigin.y + info.endRow * TILE_SIZE);
                if (gemTextures.count(info.gem.subType)) {
                    const sf::Texture& texture = gemTextures.at(info.gem.subType);
                    sf::Sprite sprite(texture);
                    float scale = (float)TILE_SIZE / texture.getSize().x;
                    sprite.setScale({scale, scale});
                    sprite.setPosition(start + (end - start) * p);
                    window.draw(sprite);
                }
            }
        }

        if (!isAnimatingSwap && !isAnimatingDestruction && !isAnimatingRefill && selectedTile.has_value()) {
            sf::RectangleShape h({(float)TILE_SIZE, (float)TILE_SIZE});
            h.setPosition({(float)boardOrigin.x + selectedTile->x * TILE_SIZE, (float)boardOrigin.y + selectedTile->y * TILE_SIZE});
            h.setFillColor(sf::Color::Transparent);
            h.setOutlineColor(sf::Color::Yellow);
            h.setOutlineThickness(2.f);
            window.draw(h);
        }
    }
    window.display();
}

void Game::moveToRoom(int destinationRoomId) {
    const Room* destination = nullptr;
    for (const auto& room : currentFloor.rooms) {
        if (room.id == destinationRoomId) {
            destination = &room;
            break;
        }
    }

    if (destination) {
        if (currentRoom) {
            roomHistory.push_back(currentRoom->id);
        }
        visitedRoomIds.insert(destinationRoomId);
        currentRoom = destination;
        
        if (clearedRoomIds.count(destination->id)) {
            currentState = GameState::Exploration;
            return;
        }

        switch (currentRoom->type) {
            case RoomType::Combat:
            case RoomType::Boss:
                monster.reset();
                board.initialize(player);
                currentState = GameState::Playing;
                break;
            case RoomType::Treasure: currentState = GameState::Treasure; break;
            case RoomType::Special: currentState = GameState::Special; break;
            case RoomType::Puzzle: currentState = GameState::Puzzle; break;
            case RoomType::Trap: currentState = GameState::Trap; break;
            case RoomType::Sanctuary: currentState = GameState::Sanctuary; break;
            case RoomType::AgilityChallenge: currentState = GameState::AgilityChallenge; break;
            case RoomType::EnduranceChallenge: currentState = GameState::EnduranceChallenge; break;
            case RoomType::MagicChallenge: currentState = GameState::MagicChallenge; break;
            default:
                currentState = GameState::Exploration;
                break;
        }
    }
}
    
void Game::initializeJudgement() {
    m_currentJudgementTrialIndex = 0;
    m_pendingJudgementTrials = dataManager.getJudgementTrials();
    std::cout << "Loaded " << m_pendingJudgementTrials.size() << " judgement trials." << std::endl;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_pendingJudgementTrials.begin(), m_pendingJudgementTrials.end(), g);
    currentState = GameState::Judgement_TacticalTrial;
}

void Game::startNextJudgementTrial() {
    if (m_currentJudgementTrialIndex >= m_pendingJudgementTrials.size()) {
        currentState = GameState::Judgement_Summary;
        return;
    }

    m_currentJudgementTrial = m_pendingJudgementTrials[m_currentJudgementTrialIndex];
    std::cout << "Starting trial: " << m_currentJudgementTrial.trialId << std::endl;
    m_currentScore = 0;
    m_currentTrialTurn = 0;
    m_manaAffinityChoice = std::nullopt;
    m_trialTimer.restart();

    if (m_currentJudgementTrial.type == JudgementTrialType::ManaAffinity) {
        currentState = GameState::Judgement_ManaAffinityTrial;
    } else {
        currentState = GameState::Judgement_TacticalTrial;
    }

    if (m_currentJudgementTrial.type == JudgementTrialType::Power) {
        board.initializeForPowerTrial();
    } else if (m_currentJudgementTrial.boardLayout.empty()) {
        board.initialize(player);
    } else {
        board.initialize(m_currentJudgementTrial.boardLayout, player);
    }
}
