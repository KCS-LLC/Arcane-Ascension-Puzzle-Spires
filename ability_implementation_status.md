# Ability Implementation Status

This document tracks the current implementation status of each Quartz-Rank Ability, including a score (out of 100) reflecting its completeness (logic, UI, animations), and a brief statement on what is needed to reach full implementation.

---

## Diviner

*   **Insight**
    *   **Score:** 70/100
    *   **Reasoning:** The core logic to find and highlight a single move is implemented. However, your new description specifies that multiple castings should highlight *additional* moves or notify the player if all are revealed. This extended logic is missing.
    *   **To Reach 100:** Implement tracking for already-revealed moves to allow the spell to find and highlight new ones on subsequent casts. Add a UI notification when no more unique moves can be found.

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
    *   **Score:** 0/100
    *   **Reasoning:** The effect type `ROTATE_ROW_COLUMN` exists, but the implementation in `EffectProcessor.cpp` is a placeholder (`board.rotateRow(0, 1);`). It does not allow the player to choose a row or column, and there is no UI for this interaction.
    *   **To Reach 100:** Implement a new game state and UI to prompt the player to select a row or column. Implement the corresponding board logic to rotate the chosen row/column.

---

## Enchanter

*   **Quick Swap**
    *   **Score:** 100/100
    *   **Reasoning:** The ability correctly grants a free swap. All UI feedback, including selection, highlighting, cancellation, and instruction text, is implemented and functional.
    *   **To Reach 100:** (Already at 100)

*   **Insight**
    *   **Score:** 70/100
    *   **Reasoning:** (Same as Diviner's Insight) The core logic to find and highlight a single move is implemented. However, your new description specifies that multiple castings should highlight *additional* moves or notify the player if all are revealed. This logic is missing.
    *   **To Reach 100:** Implement tracking for already-revealed moves to allow the spell to find and highlight new ones on subsequent casts. Add a UI notification when no more unique moves can be found.

---

## Guardian

*   **Empower Self**
    *   **Score:** 100/100
    *   **Reasoning:** The spell correctly applies the `vigor` stat modifier. The `Player` class stores this stat, and the damage calculation in `AttackGem.cpp` correctly uses `player.getVigor()` to increase Skull/Fist damage. The effect correctly appears in the UI with a duration.
    *   **To Reach 100:** (Already at 100)

*   **Minor Heal**
    *   **Score:** 100/100
    *   **Reasoning:** The spell correctly calls the `player.heal()` method with the specified amount. The player's HP is updated, and this is reflected in the UI.
    *   **To Reach 100:** (Already at 100)

---

## Mender

*   **Minor Heal**
    *   **Score:** 100/100
    *   **Reasoning:** (Same as Guardian's Minor Heal) The spell correctly calls the `player.heal()` method with the specified amount. The player's HP is updated, and this is reflected in the UI.
    *   **To Reach 100:** (Already at 100)

*   **Chaotic Shift**
    *   **Score:** 100/100
    *   **Reasoning:** The spell correctly identifies and removes 3 random gems from the board, triggering the destruction and refill animations, which results in a cascade.
    *   **To Reach 100:** (Already at 100)

---

## Shadow

*   **Shadow Strike**
    *   **Score:** 100/100
    *   **Reasoning:** The spell correctly calls the `monster.takeDamage()` method with the specified amount. The monster's HP is updated, and this is reflected in the UI.
    *   **To Reach 100:** (Already at 100)

*   **Empower Weapon**
    *   **Score:** 100/100
    *   **Reasoning:** The spell correctly applies the `skull_damage` stat modifier. The damage calculation in `AttackGem.cpp` correctly uses this multiplier when calculating Skull damage. The effect appears in the UI with a duration.
    *   **To Reach 100:** (Already at 100)

---

## Shaper

*   **Create Weapon**
    *   **Score:** 90/100
    *   **Reasoning:** The core logic is functional. The `TRANSFORM_RANDOM_GEMS` effect correctly identifies random non-attack gems and converts them into Skull gems. The only missing piece is an animation or visual effect to clearly show the transformation happening, rather than the gems just instantly changing.
    *   **To Reach 100:** Implement a brief "transformation" animation (e.g., a flash or particle effect) at the coordinates of the changed gems.

*   **Sharpen Weapon**
    *   **Score:** 0/100
    *   **Reasoning:** The effect type `MODIFY_ATTACK_GEM` exists in `spells.json`, but there is no corresponding logic in `EffectProcessor.cpp` to handle it. The concept of an individual gem having a modified damage value is not yet implemented.
    *   **To Reach 100:** Add a damage modifier member variable to the `AttackGem` class. Implement the `MODIFY_ATTACK_GEM` case in `EffectProcessor.cpp` to select a random Skull gem and increase its damage modifier. Update the `AttackGem::onMatch` method to incorporate this modifier into its damage calculation. Add a visual indicator to the gem on the board.

---

## Summoner

*   **Gust of Wind**
    *   **Score:** 0/100
    *   **Reasoning:** (Same as Elementalist's Gust of Wind) The effect type `ROTATE_ROW_COLUMN` exists, but the implementation in `EffectProcessor.cpp` is a placeholder (`board.rotateRow(0, 1);`). It does not allow the player to choose a row or column, and there is no UI for this interaction.
    *   **To Reach 100:** Implement a new game state and UI to prompt the player to select a row or column. Implement the corresponding board logic to rotate the chosen row/column.

*   **Quick Swap**
    *   **Score:** 100/100
    *   **Reasoning:** (Same as Enchanter's Quick Swap) The ability correctly grants a free swap. All UI feedback, including selection, highlighting, cancellation, and instruction text, is implemented and functional.
    *   **To Reach 100:** (Already at 100)
