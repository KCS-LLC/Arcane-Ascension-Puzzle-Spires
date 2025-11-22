# Known Bugs and Issues

This file tracks the known bugs and design discrepancies in the current build.

---

### 1. Power Trial Board Generation is Not Random

*   **Symptom:** The Trial of Power consistently starts with a non-random, unchallenging checkerboard pattern of Fire and Skull gems.
*   **Root Cause:** The current random board generation algorithm has a very low probability of producing a valid starting board (one with no initial matches and at least one valid move) when restricted to only two gem types. The `initializeForPowerTrial()` function attempts to generate a random board up to 10,000 times. When this fails, it falls back to a deterministic checkerboard pattern to prevent the game from freezing or crashing. This fallback is currently happening on every run.
*   **Proposed Solution:** Implement a new board generation algorithm that is guaranteed to produce a valid, solvable, and more random-feeling board without relying on a brute-force `while` loop.

---

### 2. Control Trial Special Gems Are Not Visually Indicated

*   **Symptom:** The objective for the Trial of Control is to "clear all special tiles," but there is no visual cue to distinguish which tiles are special.
*   **Root Cause:** The code intended to create a pulsing visual effect for the special gems (`Transference` gems, subtype 10) is not being executed correctly. The logic in the `Game::render()` function, which checks `m_currentJudgementTrial.type == JudgementTrialType::Control`, is likely failing due to a game state or timing issue, preventing the pulsing alpha effect from being applied.
*   **Proposed Solution:** Add diagnostic logging to the render loop to determine why the visual effect condition is not being met. Once diagnosed, fix the logic to ensure the pulsing effect is applied correctly during the Control Trial.

---

### 3. Attunement System Divergence and "Executioner" Attunement

*   **Symptom:** The game awarded an "Executioner" attunement, which is not present in the design documentation. The attunement assignment does not seem to reflect player performance accurately.
*   **Root Cause:** I incorrectly invented the "Executioner" attunement to solve a logic issue where there were three trials but only two defined attunements (`Elementalist`, `Guardian`). This was a divergence from the established design. The current logic incorrectly maps one trial directly to one attunement, rather than using the performance across all trials as described in `judgement.md`.
*   **Proposed Solution:**
    1.  Remove the `executioner.json` file and all references to it.
    2.  Re-implement the `Player::determineAttunement` function to align with the logic described in `judgement.md`, where performance in the Tactical Aptitude Gauntlet (Power, Haste, Control trials) influences the final attunement. This is a more complex but more faithful implementation of the design.

---

### 4. Blank Combat Board After Judgement

*   **Symptom:** After completing the Judgement and entering a combat room, the board is mostly blank, containing only a few `Attack` (Skull) gems. No mana gems are generated, and the player has no skills.
*   **Root Cause:** The `stringToGemSubType` function, which converts mana type names from JSON files into game data, is **case-sensitive**. The attunement JSON files (`attunements.json`, `executioner.json`) use lowercase names (e.g., `"fire"`, `"skull"`), while the conversion function expects uppercase names (e.g., `"Fire"`, `"Skull"`). This mismatch causes the player's `mana_types` list to be empty. When the board is generated for combat, it has no valid mana types to choose from, resulting in a mostly empty board with a few 10%-chance Skull gems.
*   **Proposed Solution:** Modify the `stringToGemSubType` function in `StringUtils.cpp` to be case-insensitive. This will correctly parse the mana types from the JSON files regardless of capitalization and permanently fix this category of bug.

---

### 5. Post-Judgement & Exploration Bugs (Observed 2025-11-22)

*   **Critical Crash on Load:** The game log shows `JSON error in spells: [json.exception.out_of_range.403] key 'cost' not found`. The game still runs but critical spell data fails to load.
*   **Instant Trial Completion:** All three Judgement trials are completed instantly upon starting the game, with the log showing "Win condition met" three times in a row. This points to a logic error in the win/loss condition check.
*   **Invalid Room IDs:** Clicking the hardcoded "North" and "East" door buttons in the exploration UI results in "Error: Tried to move to a non-existent room ID" for rooms 2 and 3. The hardcoded UI does not match the actual room IDs in `floor1.json`.
*   **UI Rendering Glitch:** During the exploration phase, the gem game board is incorrectly rendered overlaid on top of the room information UI.
*   **Unresponsive Input:** Clicking on the overlaid game board tiles during the exploration phase has no effect; the input seems to be incorrectly handled or disabled in this state.

