# Ability Implementation Status

This document tracks the current implementation status of each Quartz-Rank Ability, including a score (out of 100) reflecting its completeness (logic, UI, animations), and a brief statement on what is needed to reach full implementation.

---

## Diviner

*   **Insight**
    *   **Score:** 100/100
    *   **Reasoning:** The effect correctly finds and highlights valid moves sequentially on subsequent casts. The new `Board::findAllValidMoves` method and `Game` class state management ensure all possible moves are found and revealed to the player.
    *   **To Reach 100:** (Already at 100)

*   **Mana Surge**
    *   **Score:** 100/100
    *   **Reasoning:** The `mana_gain` stat modifier is fully implemented. The `Player` class correctly tracks the multiplier, and `ManaGem::onMatch` uses it to double mana gain. The effect icon is also correctly displayed in the UI.
    *   **To Reach 100:** (Already at 100)

---

## Elementalist

*   **Create Burning Tile**
    *   **Score:** 100/100
    *   **Reasoning:** The ability now correctly uses the centralized `TimeManager` to deal damage over time. All visual feedback, including a scaled icon and a horizontal duration bar, is implemented. The tile is also correctly destroyed upon expiration.
    *   **To Reach 100:** (Already at 100)

*   **Gust of Wind**
    *   **Score:** 100/100
    *   **Reasoning:** The ability is fully implemented. It correctly uses the unified targeting system to get two clicks from the player, determines the direction, and performs the row/column rotation. UI prompts, animations, and the final board state change are all functional.
    *   **To Reach 100:** (Already at 100)

---

## Enchanter

*   **Quick Swap**
    *   **Score:** 100/100
    *   **Reasoning:** The ability correctly grants a free swap. All UI feedback, including selection, highlighting, cancellation, and instruction text, is implemented and functional.
    *   **To Reach 100:** (Already at 100)

*   **Insight**
    *   **Score:** 100/100
    *   **Reasoning:** (Same as Diviner's Insight) The effect correctly finds and highlights valid moves sequentially on subsequent casts. The new `Board::findAllValidMoves` method and `Game` class state management ensure all possible moves are found and revealed to the player.
    *   **To Reach 100:** (Already at 100)

---

## Guardian

*   **Empower Self**
    *   **Score:** 100/100
    *   **Reasoning:** The spell correctly applies the `vigor` stat modifier. The `Player` class stores this stat, and the damage calculation in `AttackGem.cpp` correctly uses `player.getVigor()` to increase Skull/Fist damage. The effect correctly appears in the UI with a duration.
    *   **To Reach 100:** (Already at 100)

*   **Minor Heal**
    *   **Score:** 100/100
    *   **Reasoning:** The spell correctly calls the `player.heal()` method. The unified animation system now provides a green "HP Sweep" animation, which serves as sufficient visual feedback.
    *   **To Reach 100:** (Already at 100)

---

## Mender

*   **Minor Heal**
    *   **Score:** 100/100
    *   **Reasoning:** (Same as Guardian's Minor Heal) The spell correctly calls the `player.heal()` method. The unified animation system now provides a green "HP Sweep" animation, which serves as sufficient visual feedback.
    *   **To Reach 100:** (Already at 100)

*   **Chaotic Shift**
    *   **Score:** 100/100
    *   **Reasoning:** The spell correctly identifies and removes 3 random gems from the board, triggering the destruction and refill animations, which results in a cascade.
    *   **To Reach 100:** (Already at 100)

---

## Shadow

*   **Shadow Strike**
    *   **Score:** 100/100
    *   **Reasoning:** The spell correctly calls the `monster.takeDamage()` method. The unified animation system now provides a purple "HP Sweep" animation for damage, which serves as sufficient visual feedback.
    *   **To Reach 100:** (Already at 100)

*   **Empower Weapon**
    *   **Score:** 100/100
    *   **Reasoning:** The spell correctly applies the `skull_damage` stat modifier. The damage calculation in `AttackGem.cpp` correctly uses this multiplier when calculating Skull damage. The effect appears in the UI with a duration.
    *   **To Reach 100:** (Already at 100)

---

## Shaper

*   **Create Weapon**
    *   **Score:** 90/100
    *   **Reasoning:** The core logic is functional. The `TRANSFORM_RANDOM_GEMS` effect correctly identifies random non-attack gems and converts them into Skull gems. The only missing piece is a visual effect for the transformation; the gems currently change instantly.
    *   **To Reach 100:** Add a new animation state (e.g., `m_isAnimatingTransform`) to the state machine in `Game.cpp`/`Game.h`. The effect should trigger this animation, and the actual gem transformation should occur in the `update` loop after the animation timer completes.

*   **Sharpen Weapon**
    *   **Score:** 100/100
    *   **Reasoning:** The `MODIFY_ATTACK_GEM` effect is now fully implemented. The logic correctly selects a random Skull gem and increases its level. The `AttackGem::onMatch` method correctly incorporates this level into its damage calculation, and the level is visually represented on the board.
    *   **To Reach 100:** (Already at 100)

---

## Summoner

*   **Gust of Wind**
    *   **Score:** 100/100
    *   **Reasoning:** (Same as Elementalist's Gust of Wind) The ability is fully implemented. It correctly uses the unified targeting system to get two clicks from the player, determines the direction, and performs the row/column rotation. UI prompts, animations, and the final board state change are all functional.
    *   **To Reach 100:** (Already at 100)

*   **Quick Swap**
    *   **Score:** 100/100
    *   **Reasoning:** (Same as Enchanter's Quick Swap) The ability correctly grants a free swap. All UI feedback, including selection, highlighting, cancellation, and instruction text, is implemented and functional.
    *   **To Reach 100:** (Already at 100)
