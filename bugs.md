# Known Bugs and Issues

This file tracks the known bugs and design discrepancies in the current build.

---
## Recently Resolved (as of 2025-11-26)
*   **`Create Burning Tile` Ability Has No Visuals (Bug #V.Elementalist):** Fixed, the ability now applies a `Burning` status effect with a visual indicator and a timer bar.
*   **`Insight` Ability Has No Visuals (Bug #V.Diviner, #V.Enchanter):** Fixed, the `Insight` ability now provides a clear visual indicator on the two gems that form a valid move.
*   **Monster HP Lacks Numerical Display (Bug #V.Shadow):** Fixed, the monster's HP bar now includes a numerical display (e.g., "125/150").
*   **`Chaotic Shift` Leaves Holes in Board (Bug #V.Mender):** Fixed, ability now correctly triggers gravity and refills the board after removing gems.
*   **Summoner Has Incorrect Ability (Bug #V.Summoner):** Fixed. Corrected `attunements.json` to assign `quick_swap_transference` instead of `convert_mana_transference`.

---
## High Priority (as of 2025-11-26)
*   **Treasure Chests Do Not Merge (Bug #3.2):** Currently untestable due to the temporary reduction of treasure round moves. To be re-evaluated when the move count is restored.

### Ability & Data Bugs


---
## Medium Priority (as of 2025-11-26)

### UI/UX & Visual Feedback Bugs
*   **`Minor Heal` Lacks Feedback (Bug #V.Guardian):** The Guardian's `Minor Heal` does not provide obvious visual feedback (e.g., an animation or floating green number), making it difficult to tell if it worked, especially if it coincides with a monster attack.
*   **`Empower Self`/`Empower Weapon` Need Status Indicators (Bug #V.Guardian, #V.Shadow):** Abilities that grant temporary buffs are untestable and unusable without UI indicators showing that the buff is active and when it expires.

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
