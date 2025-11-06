# Next Steps

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
