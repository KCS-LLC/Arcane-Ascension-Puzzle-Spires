# Next Steps for Vertical Slice Development

This document outlines the prioritized next steps for expanding the prototype from its current state into a more complete demonstration of the game's core mechanics.

---

## COMPLETED - Phase 1: Foundational UX & Visual Feedback

This phase was focused on making the game state understandable and providing clear feedback for player actions.
- **[DONE]** Display Core Game Stats (HP, Mana).
- **[DONE]** Enhance Gem Variety (Multiple colors are implemented).
- **[DONE]** Animate Gem Swaps.
- **[DONE]** Animate Gem Destruction & Refill.

---

## COMPLETED - Phase 2: Complete the Core Combat Loop

This phase was focused on making the puzzle board fully interactive and consequential.
- **[DONE]** Implement Match Consequences (Skulls deal damage, gems grant mana).
- **[DONE]** Implement a Basic Spell System (Player can cast 4 damage spells from UI buttons).
- **[DONE]** Refine Monster Turn (Monster now operates on a speed-based system).

---

## Phase 3: Enhance Player Experience & Visual Polish

This phase focuses on making the game feel better to play.

1.  **Replace Gem Rectangles with Sprites:**
    *   **Goal:** Improve the visual quality of the board.
    *   **Tasks:**
        *   Create or find simple placeholder sprites for each gem type (Skull, Fire, Water, etc.).
        *   Modify the `render()` function in `Game.cpp` to draw these sprites instead of `sf::RectangleShape`.

2.  **Implement "Fog of War" on Stats:**
    *   **Goal:** Align the UI with the design in `UI_and_UX_Flow.md`.
    *   **Tasks:**
        *   Remove the numerical text from the HP and Mana displays.
        *   Rely solely on the visual representation of the bars to show the current values. This is a core design principle that should be implemented early.

---

## Phase 4: Introduce Core RPG Mechanics

This phase begins to integrate the deeper progression systems.

1.  **Implement the Gem Aura System:**
    *   **Goal:** Connect character power to the quality of the puzzle board.
    *   **Tasks:**
        *   Add a `Rank` member (e.g., Quartz, Carnelian) to the `Player` and `Monster` classes.
        *   Update the `Board::refillBoard` logic to use the hierarchical chance system defined in `Puzzle_Board_Mechanics.md` to generate gems with Auras.
        *   Update the match consequence logic to use the Aura multiplier from `Formulas_and_Effects.md`.
        *   Visually represent Auras on the gem sprites (e.g., a colored glow).

2.  **Implement a Single Attunement:**
    *   **Goal:** Begin transitioning from a generic `Player` to a specific `Heir`.
    *   **Tasks:**
        *   Refactor the `Player` class to represent an "Elementalist" Attunement.
        *   Ensure its mana pools and starting spells align with the `attunement.md` document.

---

## Phase 5: Architectural Refactoring

A recent attempt to refactor the codebase was made and subsequently reverted due to a series of cascading errors. This section documents the attempt and provides a clear, revised plan for the next attempt.

### Summary of Failed Attempt (Nov 6, 2025)

The goal was to improve the technical foundation by separating concerns and moving to a data-driven model. The plan involved creating `UIManager` and `DataManager` classes and moving data to JSON files. The attempt failed due to a series of critical, self-inflicted errors, primarily related to incorrect file modifications and a failure to correctly integrate the `nlohmann/json` library. After multiple failed compilations, the codebase was reverted to the last stable commit to ensure a clean state.

### Architectural Refactoring Status

The major architectural refactoring is now largely complete. The codebase is in a much more modular and data-driven state.

*   **[DONE]** **1. Separate Implementations:** `Player` and `Monster` classes now have separate `.h` and `.cpp` files.
*   **[DONE]** **2. Create the `DataManager`:** The `DataManager` class and `nlohmann/json` library have been successfully added and verified.
*   **[DONE]** **3. Integrate the `DataManager`:** The `Game` class now loads all monster and spell data from external JSON files.
*   **[DONE]** **4. Create the `UIManager`:** The `UIManager` class has been created and now handles all UI layout, update, and rendering logic, removing it from the `Game` class.

### Next Architectural Step
*   **Finalize UI Separation:** The input handling for UI elements (specifically spell button clicks) is still currently handled in `Game::processEvents()`. The next step is to move this logic into the `UIManager` to fully complete the separation of concerns. The `UIManager` should process events and return a command or action (e.g., "cast spell 1") for the `Game` class to execute.
