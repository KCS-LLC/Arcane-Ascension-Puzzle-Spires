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
