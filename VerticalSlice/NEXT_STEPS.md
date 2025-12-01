# Next Steps

## Phase 0: Core Game Introduction
- **Objective:** Implement the initial player experience for character creation and attunement assignment.
- **Key Tasks:**
    - **[TODO]** **Implement the Judgement Phase:** Develop the UI and logic for character creation, including the Visage's assessment, puzzle gauntlet, and trial run.
    - **[TODO]** **Define All Attunement Data and Base Abilities:** Create comprehensive data definitions for each attunement, including their two base abilities (one primary mana, one secondary mana).

### Phase 1: Player Equipment & Advanced Gem Mechanics
- **Objective:** Implement a comprehensive system for player stats, equipment, and advanced gem mechanics to add depth to character progression and combat.
- **Key Tasks:**
    - **[TODO]** **Fully integrate core player stats (Vigor, Speed, Wit)** into combat formulas for damage, turn speed, crit chance, etc.
    - **[TODO]** **Implement an equipment system** with slots (Weapon, Head, Body, etc.) and items that grant stat bonuses and special properties.
    - **[TODO]** **Create the logic for Special Gems** (e.g., Spears, Bombs) that are generated from complex matches (4-in-a-row, L-shapes), contingent on the player having specific equipment or abilities.
    - **[TODO]** **Implement a gem modifier system,** starting with the "Gem Aura" mechanic, which multiplies the resources gained from a match.

### Phase 2: Player Experience & Visual Polish
- **Objective:** Enhance the user's experience with better feedback, smoother interactions, and more engaging visual elements.
- **Key Tasks:**
    - **[IN PROGRESS] Implement Unified Animation System:** Create a central, non-blocking system to handle cosmetic UI and game event animations.
        - **Phase 1: Core Framework:**
            - **[TODO]** Create `Animation.h` with `Animation`, `AnimationType`, and `AnimationTarget` structs.
            - **[TODO]** Integrate an `m_activeAnimations` vector into `Game.h`.
            - **[TODO]** Implement `startAnimation()` and the animation lifecycle (removal after lifetime) in `Game.cpp`.
        - **Phase 2: Event Triggers:**
            - **[TODO]** In `EffectProcessor.cpp`, trigger `HpSweep` animations for `HEAL` and `DEAL_DAMAGE` effects, capturing health before and after.
        - **Phase 3: Rendering:**
            - **[TODO]** Add `renderAnimations()` to `UIManager` to draw animation effects.
            - **[TODO]** Implement the HP sweep visual: a fading overlay on the changed portion of the HP bar.
    - **[TODO]** **UI Animations:** Add subtle animations and feedback to other UI elements, such as mana gains.
    - **[COMPLETED] Refactor to Unified Board Animation System:** Replace the multiple `m_isAnimating...` flags with a single, unified, and scalable animation handler for all blocking board animations (swaps, destruction, falls, etc.).
        - **[COMPLETED] Phase 1: Core Framework:**
            - **[COMPLETED]** Create `BoardAnimation.h` with a `BoardAnimation` struct and enums for `BoardAnimationType`.
            - **[COMPLETED]** Refactor `Game.h` to remove all individual `m_isAnimating...` flags and animation-specific data members.
            - **[COMPLETED]** Add `std::vector<BoardAnimation> m_activeBoardAnimations;` to `Game.h`.
            - **[COMPLETED]** Implement `startBoardAnimation()` and the animation lifecycle management in `Game.cpp`.
        - **[COMPLETED] Phase 2: Refactor Triggers:**
            - **[COMPLETED]** Update `Game::handleInput` to trigger `Swap` animations instead of setting a flag.
            - **[COMPLETED]** Update `Game::handleMatches` to trigger `Destroy` and `Fall` animations.
            - **[COMPLETED]** Update `EffectProcessor.cpp` to trigger `Transform` and `Rotate` animations.
        - **[COMPLETED] Phase 3: Unify Rendering:**
            - **[COMPLETED]** Replace the `if/else if` chain in `Game::render` with a single loop over `m_activeBoardAnimations`.
            - **[COMPLETED]** Implement a `switch` statement within the render loop to handle the drawing logic for each animation type.

### Phase 3: Procedural Spire Generation
- **Objective:** Build the core roguelike loop by creating a multi-room spire for the player to ascend.
- **Key Tasks:**
    - **[TODO]** **Create Floor/Room Structure:** Design and implement the basic C++ classes for `Room` (to hold encounters) and `Floor` (a collection of rooms).
    - **[TODO]** **Implement Linear Floor Progression:** For the initial implementation, create a static, non-random sequence of rooms (e.g., Combat Room -> Combat Room -> Boss Room) to test the mechanics of moving between encounters.
    - **[TODO]** **Integrate into Game Loop:** Modify the main `Game` class to manage the current floor and transition the player to the next room upon winning a combat encounter.
    - **[TODO]** **Refactor/Integrate Floor Generation:** Address the current reliance on an external tool for `floor1.json` generation. Either refactor the external tool for better integration or integrate floor generation logic directly into the game engine for dynamic floor creation.

### Phase 4: Core Gameplay Refinements
- **Objective:** Refine fundamental gameplay mechanics for accuracy and depth.
- **Key Tasks:**
    - **[TODO]** **Implement robust gem generation logic:** Ensure the board only spawns Attack (Skulls) and relevant Mana types (player's and monster's combined unique mana affinities). Treasure (Coins) should not be generated. This involves:
        1. Updating `MonsterData` struct in `Structs.h` with `mana_affinities`.
        2. Updating `DataManager.cpp` to load monster mana affinities.
        3. Adding `manaAffinities` member to the `Monster` class.
        4. Modifying `Board::getRandomGem` to accept `Player` and `Monster` objects.
        5. Implementing new generation logic in `Board::getRandomGem` to create a spawn pool from unique player/monster mana subtypes and Skull gems, ensuring correct filtering and no Coin generation.
