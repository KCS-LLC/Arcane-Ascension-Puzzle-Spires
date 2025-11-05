# Next Steps for Vertical Slice Development

This document outlines the prioritized next steps for expanding the prototype from its current state into a more complete demonstration of the game's core mechanics.

The current prototype successfully implements:
- A basic SFML window and game loop.
- A functional match-3 board with gem swapping and refilling.
- Simple Player and Monster objects with HP.
- A rudimentary turn system based on player actions.
- A win/loss condition.

The following phases are designed to incrementally add layers of gameplay, bringing the prototype closer to the vision laid out in the design documents.

---

## Phase 1: Foundational UX & Visual Feedback

The highest priority is to make the game state understandable and to provide clear feedback for player actions. This phase addresses the immediate playability issues.

1.  **Display Core Game Stats:**
    *   **Goal:** Provide immediate context for the game state.
    *   **Tasks:**
        *   Render the Player's current and max HP on screen.
        *   Render the Monster's current and max HP on screen.

2.  **Enhance Gem Variety:**
    *   **Goal:** Make the board easier to read and patterns easier to spot.
    *   **Tasks:**
        *   Add more gem types/colors to the `GemType` enum (e.g., Green, Yellow).
        *   Update the board generation logic to include these new colors.

3.  **Animate Gem Swaps:**
    *   **Goal:** Make player moves feel impactful and clear.
    *   **Tasks:**
        *   Implement a simple animation where selected gems visibly slide into each other's places instead of instantly teleporting.

4.  **Animate Gem Destruction & Refill:**
    *   **Goal:** Make the match-and-refill cycle easy to follow.
    *   **Tasks:**
        *   Add a visual effect when gems are matched (e.g., a quick fade-out or shrink).
        *   Animate new gems falling from the top of the board to fill empty spaces.

---

## Phase 2: Complete the Core Combat Loop

With a clearer UI, the next priority is to make the puzzle board fully interactive and consequential.

1.  **Implement Match Consequences:**
    *   **Goal:** Connect the `Board::processMatches` function to the `Player` and `Monster` objects.
    *   **Tasks:**
        *   When **Skull** gems are matched, the `Monster` should take damage based on the formula in `Formulas_and_Effects.md`.
        *   When **Mana** gems (Fire, Water, etc.) are matched, the corresponding mana should be added to the `Player`'s mana pool.
        *   The results of each match (damage dealt, mana gained) should be displayed on screen temporarily.

2.  **Implement a Basic Spell System:**
    *   **Goal:** Allow the player to spend their accumulated mana.
    *   **Tasks:**
        *   Create a simple "Fireball" spell for the player.
        *   Assign a key (e.g., the '1' key) to cast the spell.
        *   When the key is pressed, check if the player has enough Fire mana.
        *   If they do, subtract the mana cost, deal damage to the monster, and display the damage on screen.

3.  **Refine Monster Turn:**
    *   **Goal:** Make the monster's turn more dynamic.
    *   **Tasks:**
        *   Give the monster a base attack damage value.
        *   When the monster's turn is triggered, it should deal its base damage to the player.
        *   This action should also have a "speed cost" that is added to the player's action counter, creating a more interactive turn flow.

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