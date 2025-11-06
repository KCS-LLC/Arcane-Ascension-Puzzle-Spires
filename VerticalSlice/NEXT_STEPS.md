# Next Steps

## Phase 2: Asset Integration [COMPLETED]
- **[DONE]** Create final, lore-based sprites for all 15 gem types.
- **[DONE]** Integrate all sprites into the game's rendering logic.
- **[DONE]** Refactor rendering logic to be resolution-independent, ensuring future assets of any size will display correctly.

## Phase 3: Player Experience & Visual Polish
- **Objective:** Enhance the user's experience with better feedback, smoother interactions, and more engaging visual elements.
- **Key Tasks:**
    - **Stat "Fog of War":** Player stats (HP, Mana) and Monster stats (HP) should be represented as gauges initially, with the exact numerical values hidden. Unlocking the ability to see the numbers will be a form of progression (e.g., a skill unlock or item).
    - **UI Animations:** Add subtle animations and feedback to UI elements, such as button presses and mana gains.
    - **Visual Effects:** Implement particle effects or other animations for events like gem matches, taking damage, and casting spells.

## Developer Notes
- The rendering system is now robust. Any new sprites added to the `assets` folder and mapped in `Game::loadTextures` will be automatically scaled to fit the `TILE_SIZE`. No further rendering changes should be needed for simple asset swaps.
- The immediate next step is to implement the "Fog of War" feature on the HP and Mana displays. This will involve modifying the `UIManager` to draw gauges and conditionally draw the text values.