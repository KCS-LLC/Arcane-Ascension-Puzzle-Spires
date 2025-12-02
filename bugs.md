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
*   **`Minor Heal` Lacks Feedback (Bug #V.Guardian):** FIXED. The `HEAL_PLAYER` effect now correctly triggers an `HpSweep` animation, providing visual feedback.
*   **`Empower Self`/`Empower Weapon` Need Status Indicators (Bug #V.Guardian, #V.Shadow):** FIXED. The `APPLY_STAT_MODIFIER` effect correctly adds `ActiveEffect` objects to the player, and the UI rendering logic already displays these active effects along with their durations.

### Ability-Specific Bugs
*   **`Gust of Wind` Incorrect Functionality (Bug #V.Elementalist, #V.Summoner):** FIXED. The ability has been refactored to allow player selection of a row/column and direction, and animations have been integrated.
*   **`Gust of Wind` Lacks Animation (Bug #V.Elementalist, #V.Summoner):** FIXED. The ability now includes a smooth animation for row/column rotations.

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
