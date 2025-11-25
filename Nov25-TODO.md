# November 25th - Attunement & Ability System Implementation Plan

This document outlines the structured task list to implement the full, data-driven attunement and ability system.

### Phase 1: Data Structure & Foundational Fixes

- **[TODO] Task 1.1: Fix the Attunement Bug.**
  - **Action:** Implement the logic for the `DataManager::getAttunementById` function in `DataManager.cpp`. It needs to search the `m_attunements` vector for an attunement with a matching ID string.

- **[TODO] Task 1.2: Expand the Data Structures.**
  - **Action:** In `Structs.h`, add `int speedCost;` to the `Spell` struct.

- **[TODO] Task 1.3: Update JSON Data Files.**
  - **Action:** Verify and update `attunements.json` to ensure its `mana_types` match the primary, secondary, and tertiary mana defined in `attunement.md`.
  - **Action:** Create a new `spells.json` file. For each of the 16 abilities in `abilities.md`, create a JSON object with a unique `id`, `name`, `manaType`, `manaCost`, and the new `speedCost`.
  - **Action:** Update `attunements.json` to include a `spellIds` array for each attunement, listing the two appropriate spell IDs.

- **[TODO] Task 1.4: Update Data Loading.**
  - **Action:** In `DataManager.cpp`, update the `from_json` function for the `Spell` struct to parse the new `speedCost` field.

### Phase 2: Core Gameplay Integration

- **[TODO] Task 2.1: Integrate `speedCost` as a Time Cost.**
  - **Action:** In `Game.cpp`, modify the game loop. When a spell is cast, add its `speedCost` to a monster's "turn progress" bar. When this bar is full, the monster takes its turn. This replaces the old turn system.
  - **Action:** In `Player.cpp`, modify the `castSpell` method to simply return the `speedCost` of the spell so the `Game` class can use it.

- **[TODO] Task 2.2: Implement a Robust Effect System.**
  - **Action:** Design and implement a new, flexible effect system that can describe actions like `ROTATE_ROW`, `TRANSFORM_GEM`, `APPLY_STAT_MODIFIER`, etc.

- **[TODO] Task 2.3: Implement New Ability Effects.**
  - **Action:** In `Player.cpp`'s `castSpell` method (or a new `EffectProcessor`), add the logic for the new ability types, calling new methods on the `Board` or modifying `Player` stats.
  - **Action:** Create the necessary new methods in `Board.h` and `Board.cpp` to handle these manipulations (e.g., `rotateRow`, `transformGem`).

### Phase 3: UI and Finalization

- **[TODO] Task 3.1: Update UI Display.**
  - **Action:** In `UIManager.cpp`, add a general speed/time tracker to the UI so the player can see the monster's turn progress.
  - **Action:** In `UIManager.cpp`, modify the spell button rendering logic to display both the `manaCost` and the `speedCost` for each ability.

- **[TODO] Task 3.2: Implement Temporary Attunement Selector.**
  - **Action:** After the treasure phase of the Judgement, implement a temporary UI screen that allows the player to manually select an attunement from a list. This will be used for testing until the full Judgement performance logic is built.

- **[TODO] Task 3.3: Full Integration Test.**
  - **Action:** Use the new selector to test each attunement, verifying that the correct abilities are granted and that they function as described.
