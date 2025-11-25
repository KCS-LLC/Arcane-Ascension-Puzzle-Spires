# Known Bugs and Issues

This file tracks the known bugs and design discrepancies in the current build.

---
## Recently Resolved (as of 2025-11-26)
*   **`Chaotic Shift` Leaves Holes in Board (Bug #V.Mender):** Fixed, ability now correctly triggers gravity and refills the board after removing gems.

---
## High Priority (as of 2025-11-26)
*   **Treasure Chests Do Not Merge (Bug #3.2):** Currently untestable due to the temporary reduction of treasure round moves. To be re-evaluated when the move count is restored.

### Ability & Data Bugs
*   **Summoner Has Incorrect Ability (Bug #V.Summoner):** The Summoner's second ability is currently `Convert Mana`. According to the design, it should be `Quick Swap`, the same as the Enchanter's ability.

---
## Medium Priority (as of 2025-11-26)

### UI/UX & Visual Feedback Bugs
*   **`Insight` Ability Has No Visuals (Bug #V.Diviner, #V.Enchanter):** The `Insight` ability is mechanically functional but provides no visual indicator on the board to highlight a suggested move, making it useless to the player.
*   **`Create Burning Tile` Ability Has No Visuals (Bug #V.Elementalist):** The ability does not provide any visual indication of which tile is "burning."
*   **`Minor Heal` Lacks Feedback (Bug #V.Guardian):** The Guardian's `Minor Heal` does not provide obvious visual feedback (e.g., an animation or floating green number), making it difficult to tell if it worked, especially if it coincides with a monster attack.
*   **`Empower Self`/`Empower Weapon` Need Status Indicators (Bug #V.Guardian, #V.Shadow):** Abilities that grant temporary buffs are untestable and unusable without UI indicators showing that the buff is active and when it expires.
*   **Monster HP Lacks Numerical Display (Bug #V.Shadow):** Testing abilities like `Empower Weapon` requires seeing the exact HP of the monster. A numerical display (e.g., "125/150") is needed.

### Ability-Specific Bugs
*   **`Gust of Wind` Incorrect Functionality (Bug #V.Elementalist, #V.Summoner):** The ability currently only rotates the top row. It needs to be refactored to allow the player to select a row/column and direction.
*   **`Gust of Wind` Lacks Animation (Bug #V.Elementalist, #V.Summoner):** The rotation is instant and jarring. It needs a smooth animation.

---
## Resolved (as of 2025-11-25)

*   **Incorrect Mana Refill (Bug #2.1a):** The game board now correctly refills gems from the dynamic pool.
*   **Incorrect Player Mana Bars (Bug #2.1b):** The UI now displays the correct mana bars for the chosen attunement.
*   **Ambiguous Cost Display (Bug #2.2):** Spell costs are now labeled "M:" and "S:".
*   **Unclear Mana Consumption (Bug #2.3):** Mana bars now have a "current/max" numerical display.

---
## Older / Resolved Bugs
*   **Power Trial Board Generation is Not Random**
*   **Control Trial Special Gems Are Not Visually Indicated**
*   **Attunement System Divergence and "Executioner" Attunement**
*   **Blank Combat Board After Judgement**
*   **Post-Judgement & Exploration Bugs (Observed 2025-11-22)**
