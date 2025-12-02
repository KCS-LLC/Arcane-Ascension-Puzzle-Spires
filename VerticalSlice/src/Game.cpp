#include "PCH.h"
#include "Game.h"
#include <iostream>
#include <cstdint>
#include <cmath> // For sin()
#include "GemSubType.h"
#include "BaseGem.h"
#include "Constants.h"
#include "Structs.h"
#include "StringUtils.h"
#include "ItemFactory.h"
#include <numeric>
#include <algorithm>
#include <random>

// Global texture map, defined here
std::map<GemSubType, sf::Texture> gemTextures;

Game::Game()
    : m_window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Judgement", sf::Style::Titlebar | sf::Style::Close),
      dataManager(),
      m_uiManager(dataManager.getFont()),
      m_gemFactory(dataManager),
      m_board(BOARD_WIDTH, BOARD_HEIGHT, m_gemFactory),
      m_player(100, {}), // Pass empty vector of Spell* now
      m_monster(MonsterData{}), // Initialize with default MonsterData, will be overridden in moveToRoom
      m_gameMode(GameMode::JUDGEMENT),
      m_gameState(GameState::Intro),
      m_currentTrialOrderIndex(0),
      m_isAnimating(false),
      m_currentTurn(0),
      m_currentScore(0)
{
    m_window.setFramerateLimit(60);
    m_animationTimings = dataManager.getAnimationTimings();
    loadTextures();

    const int boardPixelWidth = BOARD_WIDTH * TILE_SIZE;
    const int boardPixelHeight = BOARD_HEIGHT * TILE_SIZE;
    m_boardOrigin.x = (WINDOW_WIDTH - boardPixelWidth) / 2.0f;
    m_boardOrigin.y = WINDOW_HEIGHT - boardPixelHeight - 20.0f;

    m_uiManager.setup(m_player, m_window.getSize(), m_boardOrigin, {}); // Passing empty attunements for now

    dataManager.loadAttunements("data/attunements.json");
    const Attunement* defaultAttunement = dataManager.getAttunementById("diviner");
    if (defaultAttunement != nullptr) {
        m_player.setAttunement(*defaultAttunement, dataManager);
    } else {
        std::cerr << "Error: Could not find default attunement 'adept'." << '\n';
    }
    m_judgementTrials = dataManager.getJudgementTrials();
    if (!m_judgementTrials.empty()) {
        m_trialOrder.resize(m_judgementTrials.size());
        std::iota(m_trialOrder.begin(), m_trialOrder.end(), 0); // Fill with 0, 1, 2...
        
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(m_trialOrder.begin(), m_trialOrder.end(), g);
        
        m_currentTrialOrderIndex = 0;
        setupJudgementTrial(m_judgementTrials[m_trialOrder[m_currentTrialOrderIndex]]);
    }
}

void Game::loadTextures() {
    const auto& catalog = dataManager.getGemCatalog();
    for (const auto& pair : catalog) {
        const GemCatalogEntry& entry = pair.second;
        if (!entry.texturePath.empty()) {
            if (!gemTextures[pair.first].loadFromFile(entry.texturePath)) {
                std::cerr << "Failed to load texture for " << entry.name 
                          << " from " << entry.texturePath << '\n';
            }
        }
    }
    if (!m_effectIconTextures["burning_tile_effect"].loadFromFile("assets/effect_burning.png")) {
        std::cerr << "Failed to load burning tile effect icon" << '\n';
    }
    if (!m_effectIconTextures["mana_surge_enhancement"].loadFromFile("assets/gem_enhancement.png")) {
        std::cerr << "Failed to load mana surge icon" << '\n';
    }
}

void Game::setupJudgementTrial(const JudgementTrial& trial) {
    m_currentJudgementTrial = trial;
    m_gameState = GameState::Trial;
    m_currentScore = 0;
    m_currentTurn = 0;
    m_uiManager.setupTrial(trial); 

    if (trial.type == JudgementTrialType::Power) {
        m_board.initializeForPowerTrial();
    } else {
        // Default gems for now, as trial.gems is removed
        m_board.initialize({GemSubType::Fire, GemSubType::Water, GemSubType::Earth, GemSubType::Air});
    }
    m_trialClock.restart();
}

void Game::startTowerClimb() {
    m_gameMode = GameMode::TOWER_CLIMB;
    m_gameState = GameState::Exploration;
    m_currentFloor = dataManager.getFloor();
    m_currentRoom = &m_currentFloor.rooms[0];
    if (m_currentRoom != nullptr) {
        m_visitedRoomIds.insert(m_currentRoom->id);
    }
    // TODO: Potentially load monster data for the first room here if it's a combat room
}

void Game::moveToRoom(int destinationRoomId) {
    // Find the destination room in the floor data
    const Room* destination = nullptr;
    for (const auto& room : m_currentFloor.rooms) {
        if (room.id == destinationRoomId) {
            destination = &room;
            break;
        }
    }

    if (destination != nullptr) {
        m_currentRoom = destination;
        m_visitedRoomIds.insert(destinationRoomId);
        std::cout << "Moving to room " << destinationRoomId << " (" << roomTypeToString(m_currentRoom->type) << ")" << '\n';
        
        // If room is already cleared, just explore it.
        // if (clearedRoomIds.count(destination->id)) {
        //     m_gameState = GameState::Exploration;
        //     return;
        // }

        switch (m_currentRoom->type) {
            case RoomType::Combat:
            case RoomType::Boss:
                {
                    // Determine monster rank based on floor number (for now, 1 for all first floor rooms)
                    int monsterRank = m_currentFloor.floorNumber; 
                    bool isBossRoom = (m_currentRoom->type == RoomType::Boss);
                    const MonsterData* newMonsterData = dataManager.getRandomMonsterByRank(monsterRank, isBossRoom);

                    if (newMonsterData != nullptr) {
                        m_monster = Monster(*newMonsterData); // Construct new monster from loaded data
                    } else {
                        std::cerr << "Error: No monster found for Rank " << monsterRank 
                                  << " (Boss: " << (isBossRoom ? "true" : "false") << "). Using default MonsterData." << '\n';
                        // Fallback to a default constructed monster if no match is found
                        m_monster = Monster(MonsterData{});
                    }
                }

                // Create the dynamic gem pool for combat
                {
                    std::set<GemSubType> gemPoolSet;
                    for (GemSubType type : m_player.getManaTypes()) {
                        gemPoolSet.insert(type);
                    }
                    for (GemSubType type : m_monster.getManaAffinities()) {
                        gemPoolSet.insert(type);
                    }
                    gemPoolSet.insert(GemSubType::Skull); // Always include skulls
                    
                    m_combatGemPool = std::vector<GemSubType>(gemPoolSet.begin(), gemPoolSet.end());
                    m_board.initialize(m_combatGemPool);
                }
                m_gameState = GameState::Playing;
                break;
            case RoomType::Treasure:
                unloadBoard();
                m_gameState = GameState::Treasure;
                break;
            case RoomType::Special:
                unloadBoard();
                m_gameState = GameState::Special;
                break;
            case RoomType::Puzzle:
                unloadBoard();
                m_gameState = GameState::Puzzle;
                break;
            case RoomType::Trap:
                unloadBoard();
                m_gameState = GameState::Trap;
                break;
            case RoomType::Sanctuary: 
                unloadBoard();
                m_gameState = GameState::Sanctuary; 
                break;
            case RoomType::AgilityChallenge: 
                unloadBoard();
                m_gameState = GameState::AgilityChallenge; 
                break;
            case RoomType::EnduranceChallenge: 
                unloadBoard();
                m_gameState = GameState::EnduranceChallenge; 
                break;
            case RoomType::MagicChallenge: 
                unloadBoard();
                m_gameState = GameState::MagicChallenge; 
                break;
            default:
                // For any other room type, just go back to exploration for now
                unloadBoard();
                m_gameState = GameState::Exploration;
                break;
        }
    } else {
        std::cerr << "Error: Tried to move to a non-existent room ID: " << destinationRoomId << '\n';
    }
}

void Game::unloadBoard() {
    m_board.unloadBoard();
}

void Game::startAnimation(const Animation& animation) {
    m_activeAnimations.push_back(animation);
}

void Game::startBoardAnimation(const BoardAnimation& animation) {
    m_activeBoardAnimations.push_back(animation);
    std::cout << "[LOG] Game::startBoardAnimation - Animation added. New vector size: " << m_activeBoardAnimations.size() << '\n';
}

void Game::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime);
        render(dataManager.getFont(), m_pulseClock);
    }
}

void Game::processEvents() {
    for (auto event = m_window.pollEvent(); event; event = m_window.pollEvent()) {
        if (m_isAnimating) { continue; } // Ignore input during animations
        handleInput(*event);
    }
}

Player& Game::getPlayer() { return m_player; }
Monster& Game::getMonster() { return m_monster; }
Board& Game::getBoard() { return m_board; }
GemFactory& Game::getGemFactory() { return m_gemFactory; }
TimeManager& Game::getTimeManager() { return m_timeManager; }

void Game::handleInput(sf::Event event) {
    if (event.is<sf::Event::Closed>()) {
        m_window.close();
    }

    UIAction action;
    bool uiHandled = m_uiManager.handleEvent(event, m_gameMode, m_gameState, m_currentRoom, dataManager.getAttunements(), action);
    if (uiHandled) {
        if (action.type == UIActionType::SelectAttunement) {
            const auto& attunements = dataManager.getAttunements();
            auto it = std::find_if(attunements.begin(), attunements.end(), [&](const Attunement& a) {
                return a.id == action.attunementId;
            });
            if (it != attunements.end()) {
                m_player.setAttunement(*it, dataManager);
                startTowerClimb();
            }
        } else if (action.type == UIActionType::ChangeRoom) {
            moveToRoom(action.destinationRoomId);
        } else if (action.type == UIActionType::CastSpell) {
            const Spell* spell = m_player.getSpells()[action.spellIndex];
            if (spell == nullptr) { return; }

            if (m_playMode == PlayMode::Targeting && m_pendingSpell == spell) {
                cancelTargeting();
                return;
            }

            if (m_player.getMana(spell->costType) < spell->manaCost) {
                return;
            }

            if (spell->targeting.has_value()) {
                m_pendingSpell = spell;
                startTargeting(spell->targeting.value());
            } else {
                m_player.spendMana(spell->costType, spell->manaCost);
                m_timeManager.advanceTime(spell->speedCost, *this);
                m_playerActionPerformedThisTurn = true;

                std::vector<sf::Vector2i> gemsToRemove;
                for (const auto& effect : spell->effects) {
                    auto removed = m_effectProcessor.processEffect(*spell, effect, *this);
                    gemsToRemove.insert(gemsToRemove.end(), removed.begin(), removed.end());
                }

                if (!gemsToRemove.empty()) {
                    m_isAnimating = true;
                    m_animationClock.restart();
                }

                if (m_monster.isTurnReady(spell->speedCost)) {
                    m_player.takeDamage(m_monster.getAttackDamage());
                    showPlayerDamageEffect = true;
                    playerDamageClock.restart();
                }
            }
        } else if (static_cast<int>(action.type) == 99) { // Test button action
            if (m_testSwordEquipped) {
                m_player.unequipItem(EquipmentSlot::Weapon);
                std::cout << "Unequipped Iron Sword. Player Vigor: " << m_player.getVigor() << '\n';
            } else {
                const ItemBase* swordBase = dataManager.getItemBase("iron_sword");
                if (swordBase != nullptr) {
                    auto swordInstance = ItemFactory::createItemInstance(1, swordBase);
                    m_player.equipItem(std::move(swordInstance));
                    std::cout << "Equipped Iron Sword. Player Vigor: " << m_player.getVigor() << '\n';
                }
            }
            m_testSwordEquipped = !m_testSwordEquipped;
        }
        return; // UI handled the event
    }


    if (auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left) {
            // Handle click-through for placeholder screens
            if (m_gameState == GameState::Treasure || m_gameState == GameState::Special ||
                m_gameState == GameState::Puzzle || m_gameState == GameState::Trap ||
                m_gameState == GameState::Sanctuary || m_gameState == GameState::AgilityChallenge ||
                m_gameState == GameState::EnduranceChallenge || m_gameState == GameState::MagicChallenge)
            {
                m_gameState = GameState::Exploration;
                return; // Consume the click and do nothing else
            }

            if (m_gameState == GameState::Summary) {
                // Store performance
                switch (m_currentJudgementTrial.type) {
                    case JudgementTrialType::Power:
                        m_trialPerformance.powerScore = m_currentScore;
                        break;
                    case JudgementTrialType::Haste:
                        m_trialPerformance.hasteScore = m_currentScore;
                        break;
                    case JudgementTrialType::Control:
                        m_trialPerformance.controlScore = m_currentScore;
                        break;
                    default: break;
                }

                m_currentTrialOrderIndex++;
                if (m_currentTrialOrderIndex < m_trialOrder.size()) {
                    setupJudgementTrial(m_judgementTrials[m_trialOrder[m_currentTrialOrderIndex]]);
                } else {
                    // All trials complete, transition to the treasure round
                    m_gameState = GameState::Judgement_TreasureRound;
                    setupTreasureRound();
                }
                return; // Consume the click
            }

            int col = (mb->position.x - static_cast<int>(m_boardOrigin.x)) / TILE_SIZE;
            int row = (mb->position.y - static_cast<int>(m_boardOrigin.y)) / TILE_SIZE;

            if (m_playMode == PlayMode::Targeting) {
                if (m_targetingSelections.empty()) {
                    m_board.getGemAt(row, col)->setActionState(ActionState::Selected);
                }
                m_targetingSelections.emplace_back(col, row);
                if (m_targetingSelections.size() >= m_targetingRequest.numberOfClicks) {
                    resolveTargeting();
                }
                return; // Consume the click
            }

            m_board.clearActionStates();

            
            // --- Normal Swap Logic ---
            if (m_selectedGem.x == -1) {
                // First gem selection
                m_selectedGem = sf::Vector2i(col, row);
                m_board.getGemAt(row, col)->setActionState(ActionState::Selected);
            } else {
                // Second gem selection
                if (m_board.isAdjacent(sf::Vector2i(m_selectedGem.y, m_selectedGem.x), sf::Vector2i(row, col))) {
                    if (m_board.canSwap(m_selectedGem.y, m_selectedGem.x, row, col)) {
                        m_isAnimating = true;
                        sf::Vector2i firstGem = m_selectedGem;
                        sf::Vector2i secondGem = sf::Vector2i(col, row);
                        
                        auto onSwapComplete = [this, firstGem, secondGem]() {
                            m_board.swapGems(firstGem.y, firstGem.x, secondGem.y, secondGem.x);
                            handleMatches(true);
                        };
                        std::cout << "[LOG] Game::handleInput - Attaching onComplete to Swap animation." << '\n';
                        startBoardAnimation({BoardAnimationType::Swap, sf::Clock(), sf::milliseconds(m_animationTimings.swap_duration_ms), firstGem, secondGem, {}, GemSubType::None, {}, {}, 0, 0, onSwapComplete});
                        startBoardAnimation({BoardAnimationType::Swap, sf::Clock(), sf::milliseconds(m_animationTimings.swap_duration_ms), secondGem, firstGem, {}, GemSubType::None});
                        m_animationClock.restart();
                    }
                }
                
                // Always clear selection on second click, whether it was a swap or a cancel
                m_board.getGemAt(m_selectedGem.y, m_selectedGem.x)->setActionState(ActionState::None);
                m_selectedGem = sf::Vector2i(-1, -1);
            }
        }
    }
}



void Game::handleMatches(bool isPlayerMove) {
    std::cout << "[LOG] Game::handleMatches - Entered." << '\n';
    auto matchGroups = m_matchDetector.findAllMatches(m_board);

    if (matchGroups.empty()) {
        return;
    }
    std::cout << "[LOG] Game::handleMatches - Found " << matchGroups.size() << " match groups." << '\n';

    m_playerActionPerformedThisTurn = true;
    m_insightMoves.clear();
    m_insightMovesIndex = 0;

    if (isPlayerMove) {
        m_currentTurn++;
        m_timeManager.advanceTime(BASE_SWAP_SPEED, *this);

        if (m_gameMode == GameMode::TOWER_CLIMB && m_monster.isTurnReady(30)) { // Placeholder speed cost for a match
            m_player.takeDamage(m_monster.getAttackDamage());
            showPlayerDamageEffect = true;
            playerDamageClock.restart();
        }
    }

    std::set<sf::Vector2i, Vector2iCompare> allRemovedGems;
    float totalSkullDamageThisTurn = 0.0f; // New centralized damage accumulator

    for (const auto& match : matchGroups) {
        // Instead of calling resolveMatches, process each gem in the match here
        for (const auto& pos : match) {
            BaseGem* gem = m_board.getGemAt(pos.x, pos.y);
            if (gem != nullptr) {
                totalSkullDamageThisTurn += gem->onMatch(m_board, m_player, m_monster);
            }
        }

        auto resolutionOpt = m_matchProcessor.process(match, m_board, m_gemFactory, dataManager, gemTextures);
        if (resolutionOpt && *resolutionOpt) {
            // Handle transformations first
            for (const auto& transform : (*resolutionOpt)->gemsToTransform) {
                BaseGem* gem = m_board.getGemAt(transform.first.x, transform.first.y);
                if (gem != nullptr) {
                    const GemCatalogEntry* newCatalogEntry = dataManager.getGemCatalogEntry(transform.second);
                    if (newCatalogEntry != nullptr && gemTextures.count(transform.second) != 0u) {
                        gem->transform(newCatalogEntry, gemTextures.at(transform.second));
                    }
                }
            }

            // Handle removals
            for (const auto& pos : (*resolutionOpt)->gemsToRemove) {
                allRemovedGems.insert(pos);
            }
        }
    }

    // Apply accumulated damage once
    if (totalSkullDamageThisTurn > 0.0f) {
        float hpBefore = static_cast<float>(m_monster.getCurrentHp());
        int finalDamage = static_cast<int>(totalSkullDamageThisTurn);
        m_monster.takeDamage(finalDamage);
        
        startAnimation({
            AnimationType::HpSweep,
            AnimationTarget::MonsterHpBar,
            sf::seconds(0.7f),
            sf::Clock(),
            sf::Color(180, 0, 0, 200), // A standard damage-red color
            hpBefore,
            static_cast<float>(m_monster.getCurrentHp()),
            static_cast<float>(m_monster.getMaxHp())
        });

        std::cout << "[COMBAT TURN] Total Damage Applied: " << finalDamage << ". Monster HP after: " << m_monster.getCurrentHp() << '\n';
    }

    if (allRemovedGems.empty()) {
        return;
    }

    m_currentScore += (static_cast<int>(allRemovedGems.size()) * 100);

    std::vector<sf::Vector2i> gemsToDestroy(allRemovedGems.begin(), allRemovedGems.end());
    destroyAndRefillGems(gemsToDestroy);
}

void Game::update(sf::Time deltaTime) {
    const float swapAnimationDuration = 0.2f;
    const float destructionAnimationDuration = 0.3f;
    const float refillAnimationDuration = 0.3f;

    if (!m_isAnimating && m_boardStateDirty) {
        m_boardStateDirty = false;
        handleMatches(false);
    }

    // Update board animations
    if (!m_activeBoardAnimations.empty()) {
        m_isAnimating = true;
        std::vector<std::function<void()>> deferredCallbacks;

        m_activeBoardAnimations.erase(
            std::remove_if(
                m_activeBoardAnimations.begin(),
                m_activeBoardAnimations.end(),
                [this, &deferredCallbacks](BoardAnimation& anim) {
                    if (anim.clock.getElapsedTime() >= anim.duration) {
                        if (anim.onComplete) {
                            deferredCallbacks.push_back(std::move(anim.onComplete));
                        }
                        return true;
                    }
                    return false;
                }
            ),
            m_activeBoardAnimations.end()
        );

        // Execute deferred callbacks AFTER iterating and erasing.
        for (const auto& callback : deferredCallbacks) {
            callback();
        }

    } else {
        m_isAnimating = false;
    }

    if (!m_isAnimating && m_playerActionPerformedThisTurn) {
        m_player.updateEffects(BASE_SWAP_SPEED);
        m_playerActionPerformedThisTurn = false; // Reset for the next turn
    }

    if (showPlayerDamageEffect && playerDamageClock.getElapsedTime().asMilliseconds() > 200) {
        showPlayerDamageEffect = false;
    }

    // --- Game State Transition Checks ---
    // Only check for win/loss or other state transitions if the board is stable.
    if (!m_isAnimating) {
        // Check for Judgement trial win/loss conditions only if not animating AND in a trial
        if (m_gameMode == GameMode::JUDGEMENT && m_gameState == GameState::Trial) {
            if (m_currentScore >= m_currentJudgementTrial.scoreGoal || m_currentTurn >= m_currentJudgementTrial.turnLimit) {
                m_gameState = GameState::Summary; // Win/Loss condition
            }
        }

        // Check for Treasure Round end condition
        if (m_gameState == GameState::Judgement_TreasureRound && m_currentTurn >= 5) { // TODO: Restore this to a higher value
            int finalTreasureValue = 0;
            for (int r = 0; r < BOARD_HEIGHT; ++r) {
                for (int c = 0; c < BOARD_WIDTH; ++c) {
                    BaseGem* gem = m_board.getGemAt(r, c);
                    if (gem != nullptr) {
                        switch (gem->getSubType()) {
                            case GemSubType::Coin: finalTreasureValue += 1; break;
                            case GemSubType::CoinPile: finalTreasureValue += 5; break;
                            case GemSubType::CoinBag: finalTreasureValue += 25; break;
                            case GemSubType::CoinBagBundle: finalTreasureValue += 100; break;
                            case GemSubType::TreasureChest: finalTreasureValue += 500; break;
                            default: break;
                        }
                    }
                }
            }
            std::cout << "Final Treasure Value: " << finalTreasureValue << '\n';
            m_gameState = GameState::Judgement_AttunementSelection;
        }

        // Check for combat win/loss in Tower Climb mode
        if (m_gameMode == GameMode::TOWER_CLIMB && (m_gameState == GameState::Playing || m_gameState == GameState::CombatVictory)) {
            if (m_player.getHp() <= 0) {
                m_gameState = GameState::GameOver;
            } else if (m_monster.getCurrentHp() <= 0) {
                m_gameState = GameState::CombatVictory;
            }
        }

        // Transition from victory screen back to exploration
        if (m_gameState == GameState::CombatVictory && !m_boardStateDirty) {
            m_gameState = GameState::Exploration;
        }
    }

    // Main game logic updates
    switch (m_gameMode) {
        case GameMode::JUDGEMENT:
        {
            m_uiManager.update(m_player, m_monster, m_timeManager, m_gameMode, m_gameState, m_playMode, m_targetingRequest, nullptr, Floor(), {}, dataManager, m_currentJudgementTrial, m_currentScore, m_currentTurn, std::nullopt, m_trialPerformance, m_player.getActiveEffects());
            break;
        }
        case GameMode::TOWER_CLIMB:
            m_uiManager.update(m_player, m_monster, m_timeManager, m_gameMode, m_gameState, m_playMode, m_targetingRequest, m_currentRoom, m_currentFloor, m_visitedRoomIds, dataManager, m_currentJudgementTrial, m_currentScore, m_currentTurn, std::nullopt, m_trialPerformance, m_player.getActiveEffects());
            break;
        case GameMode::GAME_OVER:
            // No specific updates needed for main game logic in GAME_OVER mode
            break;
    }

    // After all other updates, check for new matches that might have been created by spells
    if (!m_isAnimating && m_boardStateDirty) {
        m_boardStateDirty = false;
        handleMatches(false);
    }

    // Update and remove finished cosmetic animations
    m_activeAnimations.erase(
        std::remove_if(
            m_activeAnimations.begin(),
            m_activeAnimations.end(),
            [](const Animation& anim) {
                return anim.clock.getElapsedTime() >= anim.lifetime;
            }
        ),
        m_activeAnimations.end()
    );
}

void Game::setBoardStateDirty(bool isDirty) {
    m_boardStateDirty = isDirty;
}

void Game::render(const sf::Font& font, sf::Clock& highlightClock) {
    m_window.clear(sf::Color(30, 30, 30));

    // Only render the board and animations during combat-related states
    if (m_gameState == GameState::Playing || m_gameState == GameState::Trial || m_isAnimating || m_gameState == GameState::GameOver || m_gameState == GameState::Judgement_TreasureRound) {
        // Do not render the board or animations if the trial is over
        if (m_gameState != GameState::Summary && m_gameState != GameState::AttunementReveal) {
            m_board.render(m_window, m_boardOrigin, font, highlightClock, m_timeManager, m_effectIconTextures, m_activeBoardAnimations);

            for (const auto& anim : m_activeBoardAnimations) {
                float p = std::min(1.f, anim.clock.getElapsedTime().asSeconds() / anim.duration.asSeconds());
                switch (anim.type) {
                    case BoardAnimationType::Swap: {
                        sf::Vector2f p1_local((float)anim.startPos.x * TILE_SIZE, (float)anim.startPos.y * TILE_SIZE);
                        sf::Vector2f p2_local((float)anim.endPos.x * TILE_SIZE, (float)anim.endPos.y * TILE_SIZE);
                        // During a swap, the gem is logically still at its start position.
                        BaseGem* gem = m_board.getGemAt(anim.startPos.y, anim.startPos.x);
                        if (gem != nullptr) {
                            sf::Sprite sprite = gem->getSprite();
                            sprite.setPosition(p1_local + (p2_local - p1_local) * p + m_boardOrigin);
                            m_window.draw(sprite);
                        }
                        break;
                    }
                    case BoardAnimationType::Fall: {
                        sf::Vector2f p1_local((float)anim.startPos.x * TILE_SIZE, (float)anim.startPos.y * TILE_SIZE);
                        sf::Vector2f p2_local((float)anim.endPos.x * TILE_SIZE, (float)anim.endPos.y * TILE_SIZE);
                        // After gravity, the gem is logically at its end position.
                        BaseGem* gem = m_board.getGemAt(anim.endPos.y, anim.endPos.x);
                        if (gem != nullptr) {
                            sf::Sprite sprite = gem->getSprite();
                            sprite.setPosition(p1_local + (p2_local - p1_local) * p + m_boardOrigin);
                            m_window.draw(sprite);
                        }
                        break;
                    }
                    case BoardAnimationType::Destroy: {
                        if (anim.destroyedGemType != GemSubType::None) {
                            float scale = 1.f - p;
                            sf::Sprite sprite(gemTextures.at(anim.destroyedGemType));
                            const sf::Texture* tex = &sprite.getTexture();
                            sf::Vector2u texSize = tex->getSize();
                            sprite.setOrigin({static_cast<float>(texSize.x) / 2.f, static_cast<float>(texSize.y) / 2.f});
                            float baseScaleX = static_cast<float>(TILE_SIZE) / static_cast<float>(texSize.x);
                            float baseScaleY = static_cast<float>(TILE_SIZE) / static_cast<float>(texSize.y);
                            sprite.setScale({baseScaleX * scale, baseScaleY * scale});
                            sprite.setPosition({static_cast<float>(anim.position.y) * TILE_SIZE + TILE_SIZE / 2.f + m_boardOrigin.x, static_cast<float>(anim.position.x) * TILE_SIZE + TILE_SIZE / 2.f + m_boardOrigin.y});
                            m_window.draw(sprite);
                        }
                        break;
                    }
                    case BoardAnimationType::Transform: {
                        float scale = 1.0f + 0.5f * std::sin(p * 3.14159f);
                        BaseGem* gem = m_board.getGemAt(anim.position.x, anim.position.y);
                        if (gem != nullptr) {
                             sf::Sprite sprite = gem->getSprite();
                            const sf::Texture* tex = &sprite.getTexture();
                            sf::Vector2u texSize = tex->getSize();
                            float baseScaleX = static_cast<float>(TILE_SIZE) / static_cast<float>(texSize.x);
                            float baseScaleY = static_cast<float>(TILE_SIZE) / static_cast<float>(texSize.y);
                            sprite.setScale({baseScaleX * scale, baseScaleY * scale});
                            sprite.setPosition({static_cast<float>(anim.position.y) * TILE_SIZE + TILE_SIZE / 2.f + m_boardOrigin.x, static_cast<float>(anim.position.x) * TILE_SIZE + TILE_SIZE / 2.f + m_boardOrigin.y});
                            m_window.draw(sprite);
                        }
                        break;
                    }
                    case BoardAnimationType::RotateRow: {
                        for (int c = 0; c < m_board.getWidth(); ++c) {
                            BaseGem* gem = m_board.getGemAt(anim.index, c);
                            if (gem != nullptr) {
                                sf::Sprite sprite = gem->getSprite();
                                float newX = (static_cast<float>(c) + static_cast<float>(anim.direction) * p);
                                if (newX < 0) { newX += static_cast<float>(m_board.getWidth()); }
                                if (newX >= static_cast<float>(m_board.getWidth())) { newX -= static_cast<float>(m_board.getWidth()); }
                                sprite.setPosition(sf::Vector2f(newX * TILE_SIZE + m_boardOrigin.x, static_cast<float>(anim.index) * TILE_SIZE + m_boardOrigin.y));
                                m_window.draw(sprite);
                            }
                        }
                        break;
                    }
                    case BoardAnimationType::RotateColumn: {
                        for (int r = 0; r < m_board.getHeight(); ++r) {
                            BaseGem* gem = m_board.getGemAt(r, anim.index);
                            if (gem != nullptr) {
                                sf::Sprite sprite = gem->getSprite();
                                float newY = (static_cast<float>(r) + static_cast<float>(anim.direction) * p);
                                if (newY < 0) { newY += static_cast<float>(m_board.getHeight()); }
                                if (newY >= static_cast<float>(m_board.getHeight())) { newY -= static_cast<float>(m_board.getHeight()); }
                                sprite.setPosition(sf::Vector2f(static_cast<float>(anim.index) * TILE_SIZE + m_boardOrigin.x, newY * TILE_SIZE + m_boardOrigin.y));
                                m_window.draw(sprite);
                            }
                        }
                        break;
                    }
                }
            }
        }
    }

    // UI Rendering
    m_uiManager.render(m_window, m_boardOrigin, m_gameMode, m_gameState, m_playMode, m_targetingRequest, showPlayerDamageEffect, m_currentJudgementTrial, m_currentScore, m_currentTurn, std::nullopt, m_trialPerformance, gemTextures, m_effectIconTextures);

    // Render cosmetic animations on top of everything else
    m_uiManager.renderAnimations(m_window, m_activeAnimations);

    m_window.display();
}

void Game::setupTreasureRound() {
    m_gameState = GameState::Judgement_TreasureRound;
    m_currentTurn = 0;
    m_currentScore = 0; // Reset score for the treasure round
    m_uiManager.setupTreasureRound();

    // The treasure round board needs a few extra gem types to prevent
    // the board initializer from getting stuck in an infinite loop.
    m_treasureRoundGems = { GemSubType::Coin, GemSubType::Coin, GemSubType::Coin, GemSubType::Fire, GemSubType::Water };

    m_board.initialize(m_treasureRoundGems);
}

bool Game::processTreasureMerges() {

    // TODO: Implement the treasure merge logic here.

    return false;

}



void Game::handleTimeEvent(const TimeEvent& event) {
    switch (event.type) {
            case TimeEventType::BurningTile_Activation: {
                    BaseGem* gem = m_board.getGemAt(event.coordinates.y, event.coordinates.x);
                    if (gem != nullptr && gem->getStatusEffect() == StatusEffect::Burning) {
                        float damage = (static_cast<float>(m_player.getMaxMana()) * 0.10f) * static_cast<float>(gem->getLevel());
                        m_monster.takeDamage(static_cast<int>(damage));
                        std::cout << "[EVENT] Burning tile dealt " << static_cast<int>(damage) << " damage. Monster HP: " << m_monster.getCurrentHp() << '\n';
                    }
                    break;
                }
        case TimeEventType::BurningTile_Expire: {
            BaseGem* gem = m_board.getGemAt(event.coordinates.x, event.coordinates.y);
            // The tile effect expires, so we damage and destroy whatever is here.
            if (gem != nullptr) {
                float damage = (static_cast<float>(m_player.getMaxMana()) * 0.10f) * static_cast<float>(gem->getLevel());
                m_monster.takeDamage(static_cast<int>(damage));
                std::cout << "[EVENT] Burning tile dealt final " << static_cast<int>(damage) << " damage and expired. Monster HP: " << m_monster.getCurrentHp() << '\n';
                
                // We must clear the status from the current gem before destroying it,
                // in case it's a different gem from the one that was originally burning.
                gem->setStatusEffect(StatusEffect::None);
                destroyAndRefillGems({event.coordinates});
            }
            break;
        }    }
}

void Game::startTargeting(const TargetingData& targetingData) {
    m_playMode = PlayMode::Targeting;
    m_targetingRequest.numberOfClicks = targetingData.numberOfClicks;
    m_targetingRequest.abilityId = m_pendingSpell->id; // Use pending spell for ID in request
    m_targetingSelections.clear();
    m_uiManager.m_activeTargetingSpellId = m_pendingSpell->id; // Set active spell for UI highlight
}

void Game::resolveTargeting() {
    if (m_pendingSpell == nullptr) {
        cancelTargeting();
        return;
    }

    // Validate targeting based on pending spell's targeting data
    if (m_pendingSpell->targeting.has_value()) {
        const auto& targetingData = m_pendingSpell->targeting.value();

        if (targetingData.type == "2_adjacent" && m_targetingSelections.size() == 2) {
            if (!m_board.isAdjacent({m_targetingSelections[0].y, m_targetingSelections[0].x}, {m_targetingSelections[1].y, m_targetingSelections[1].x})) {
                cancelTargeting();
                return;
            }
        } else if (targetingData.type == "2_any" && m_targetingSelections.size() == 2) {
            // No special validation needed for 2_any other than count.
        }
    }

    m_player.spendMana(m_pendingSpell->costType, m_pendingSpell->manaCost);
    m_timeManager.advanceTime(m_pendingSpell->speedCost, *this);
    m_playerActionPerformedThisTurn = true;

    // Process effects now that targeting is successful
    for (const auto& effect : m_pendingSpell->effects) {
        if (effect.type == "ROTATE_ROW_COLUMN") {
            if (m_targetingSelections.size() == 2) {
                sf::Vector2i firstClick = {m_targetingSelections[0].y, m_targetingSelections[0].x};
                sf::Vector2i secondClick = {m_targetingSelections[1].y, m_targetingSelections[1].x};
                int dx = secondClick.y - firstClick.y;
                int dy = secondClick.x - firstClick.x;

                if (abs(dx) > abs(dy)) {
                    int direction = (dx > 0) ? 1 : -1;
                    auto onComplete = [this, firstClick, direction]() {
                        m_board.rotateRow(firstClick.x, direction);
                        handleMatches(false);
                    };
                    startBoardAnimation({BoardAnimationType::RotateRow, sf::Clock(), sf::milliseconds(m_animationTimings.rotate_duration_ms), {}, {}, {}, GemSubType::None, GemSubType::None, GemSubType::None, firstClick.x, direction, onComplete});
                } else {
                    int direction = (dy > 0) ? 1 : -1;
                    auto onComplete = [this, firstClick, direction]() {
                        m_board.rotateColumn(firstClick.y, direction);
                        handleMatches(false);
                    };
                    startBoardAnimation({BoardAnimationType::RotateColumn, sf::Clock(), sf::milliseconds(m_animationTimings.rotate_duration_ms), {}, {}, {}, GemSubType::None, GemSubType::None, GemSubType::None, firstClick.y, direction, onComplete});
                }
            }
        } else if (effect.type == "FREE_SWAP") {
             if (m_targetingSelections.size() == 2) {
                sf::Vector2i firstGem = {m_targetingSelections[0].x, m_targetingSelections[0].y};
                sf::Vector2i secondGem = {m_targetingSelections[1].x, m_targetingSelections[1].y};
                startBoardAnimation({BoardAnimationType::Swap, sf::Clock(), sf::seconds(0.2f), firstGem, secondGem});
                startBoardAnimation({BoardAnimationType::Swap, sf::Clock(), sf::seconds(0.2f), secondGem, firstGem});
            }
        }
    }

    cancelTargeting(); // Clean up state after resolving
}


void Game::cancelTargeting() {
    m_playMode = PlayMode::Normal;
    m_pendingSpell = nullptr;
    m_targetingRequest = {};
    m_targetingSelections.clear();
    m_board.clearActionStates();
    m_uiManager.m_activeTargetingSpellId = ""; // Clear active spell for UI highlight
}

// Insight spell state management
const std::vector<std::pair<sf::Vector2i, sf::Vector2i>>& Game::getInsightMoves() const {
    return m_insightMoves;
}

void Game::setInsightMoves(const std::vector<std::pair<sf::Vector2i, sf::Vector2i>>& moves) {
    m_insightMoves = moves;
}

int Game::getInsightMovesIndex() const {
    return m_insightMovesIndex;
}

void Game::setInsightMovesIndex(int index) {
    m_insightMovesIndex = index;
}

void Game::destroyAndRefillGems(const std::vector<sf::Vector2i>& coords) {
    if (coords.empty()) {
        return;
    }

    // --- Create Destroy Animations ---
    std::map<sf::Vector2i, GemSubType, Vector2iCompare> destroyedGemTypes;
    for (const auto& pos : coords) {
        BaseGem* gem = m_board.getGemAt(pos.x, pos.y);
        if (gem != nullptr) {
            destroyedGemTypes[pos] = gem->getSubType();
        }
    }

    // Now, actually remove the gems from the logical board.
    for (const auto& pos : coords) {
        m_board.removeGem(pos.x, pos.y);
    }

    std::cout << "[LOG] Game::destroyAndRefillGems - Adding Destroy animations..." << '\n';
    for (const auto& pos : coords) {
        GemSubType type = destroyedGemTypes.count(pos) != 0u ? destroyedGemTypes.at(pos) : GemSubType::None;
        startBoardAnimation({BoardAnimationType::Destroy, sf::Clock(), sf::milliseconds(m_animationTimings.destroy_duration_ms), {}, {}, pos, type});
    }

    // Refill logic
    std::cout << "[LOG] Game::destroyAndRefillGems - Adding Fall animations..." << '\n';
    std::vector<Board::FallInfo> fallInfo;
    if (m_gameState == GameState::Judgement_TreasureRound) {
        fallInfo = m_board.applyGravityAndRefill(m_treasureRoundGems);
    } else if (m_gameMode == GameMode::TOWER_CLIMB) {
        fallInfo = m_board.applyGravityAndRefill(m_combatGemPool);
    } else if (m_gameState == GameState::Trial) {
        fallInfo = m_board.applyGravityAndRefill({GemSubType::Fire, GemSubType::Water, GemSubType::Earth, GemSubType::Air});
    }

    for (const auto& info : fallInfo) {
        startBoardAnimation({BoardAnimationType::Fall, sf::Clock(), sf::milliseconds(m_animationTimings.fall_duration_ms), {info.col, info.startRow}, {info.col, info.fallToRow}});
    }

    // After adding all destroy and fall animations, find the last one and attach the recursive callback.
    if (!m_activeBoardAnimations.empty()) {
        m_activeBoardAnimations.back().onComplete = [this]() {
            handleMatches(false); // Check for new matches after refill
        };
    }

    m_animationClock.restart();
}

