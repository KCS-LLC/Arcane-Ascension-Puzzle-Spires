# Next Steps

## Recent Development Log (November 7, 2025)

### Problem Encountered:
During recent development, the project entered a state of repeated compilation failures. This was primarily due to:
- Attempting large-scale, multi-file refactoring without incremental verification.
- Incorrect usage of SFML API features (e.g., `sf::VideoMode` constructor, `sf::Font::loadFromFile` vs. `openFromFile`, `sf::Text` initialization, `window.pollEvent` loop structure) that were incompatible with the project's SFML version or established patterns.
- Unintentionally deleting untracked header files (`PCH.h`, `Structs.h`) during a previous `git reset` attempt, which further complicated debugging.

### Resolution:
To resolve the unstable state, a hard reset was performed to the last known stable commit: `ec2e591` (feat(ui): Implement 'Fog of War' and enhance spell buttons).

### Current Status:
- The codebase has been successfully reverted to a clean, compiling state.
- All previous compilation errors have been resolved by the rollback.

### Revised Plan: Display Monster Name (Incremental Approach)
To avoid past issues, development will now proceed with a strict, incremental, and compile-after-each-step approach. The immediate goal is to display the monster's specific name (e.g., "Goblin Grunt") instead of a generic title.

1.  **Step A:** Add `std::string name;` to `Monster.h`.
2.  **Step B:** Modify `Monster.cpp` to load the name from the `monster.json` file.
3.  **Step C:** Add an `sf::Text monsterNameText;` member to `UIManager.h`.
4.  **Step D:** In `UIManager.cpp`, initialize the `monsterNameText` object (font, size, color, position) in the `setup` method.
5.  **Step E:** In `UIManager.cpp`, update the text string with the monster's name in the `update` method.
6.  **Step F:** In `UIManager.cpp`, draw the `monsterNameText` in the `render` method.

Each step will be followed by a compilation to verify correctness before proceeding.

## Phase 2: Asset Integration [COMPLETED]
- **[DONE]** Create final, lore-based sprites for all 15 gem types.
- **[DONE]** Integrate all sprites into the game's rendering logic.
- **[DONE]** Refactor rendering logic to be resolution-independent, ensuring future assets of any size will display correctly.

## Phase 3: Player Experience & Visual Polish
- **Objective:** Enhance the user's experience with better feedback, smoother interactions, and more engaging visual elements.
- **Key Tasks:**
    - **[DONE]** **Stat "Fog of War":** Player stats (HP, Mana) and Monster stats (HP) are now represented as gauges, hiding the exact numerical values.
    - **[DONE]** **Spell Readiness Indicator:** Spell buttons now have a vertical fill effect to show mana progress and change color distinctly when ready.
    - **[TODO]** **Monster Attack Indicator:** Add a clear visual effect when the monster performs an action (e.g., a screen flash, a simple animation, or a particle effect on the player's panel) to ensure the player is aware they have taken damage.
    - **[TODO]** **UI Animations:** Add subtle animations and feedback to other UI elements, such as mana gains.
    - **[TODO]** **Visual Effects:** Implement particle effects or other animations for events like gem matches and taking damage.

## Developer Notes
- The rendering system is now robust. Any new sprites added to the `assets` folder and mapped in `Game::loadTextures` will be automatically scaled to fit the `TILE_SIZE`.
- The next logical step would be to implement the **Monster Attack Indicator**. A simple, effective approach could be a quick red flash over the player's side panel when they take damage. This would require adding a new state or timer in the `Game` class to control the flash effect in the `render` loop.
