# To-Do: Tile System Refactor (Nov 26)

This document outlines the step-by-step plan to refactor the `BaseGem` class to support the multi-layered visual system described in `tiles.md`.

## Phase 1: Refactor `BaseGem` and Core Structures
- [X] **1.1 Define New Enums/Types:**
    - [X] Create enumerations for `StatusEffect` (None, Burning, Frozen, Poisoned, Drained) and `ActionState` (None, Selected, ValidMoveHint) in `Structs.h`.
    - [X] Gem level will be an `int` (not an enum).
- [X] **1.2 Modify `BaseGem.h`:** Add new member variables to the `BaseGem` base class to represent the layers:
    - [X] `int m_level;` (for gem level/rank)
    - [X] `StatusEffect m_statusEffect;`
    - [X] `ActionState m_actionState;`
    - [X] `int m_counter;`
- [X] **1.3 Add Getters/Setters:** Add public getter and setter methods for these new properties in `BaseGem.h` and implement them in `BaseGem.cpp`.
- [X] **1.4 Update `GemFactory`:** (No changes needed) `GemFactory.cpp` relies on the `BaseGem` constructor for default initialization of new properties (`m_level = 1`, `StatusEffect::None`, `ActionState::None`, `m_counter = 0`).

## Phase 2: Implement Multi-Layer Rendering
- [X] **2.1 Create Color Helper:** Add a `gemLevelToColor` helper function to `StringUtils` to convert a gem's `int m_level` to the corresponding `sf::Color`, handling the special cases for level 1 (transparent/black) and level 2 (white/clear).
- [X] **2.2 Update `Board::render()` Signature:** Pass the game's `font` and a `pulseClock` from `Game::render()` down to `Board::render()` to enable text and animation rendering.
- [X] **2.3 Implement Layered Rendering:** Rewrite the `Board::render()` logic to draw each gem's layers in the correct order: Background, Gem, Effect, Counter, and Frame. Use placeholders for effect/frame sprites for now.
    - [ ] **2.3.1 Add Effect Animation Placeholder:** Add a placeholder for a generic "effect applied" animation in `Board::render`.

## Phase 3: Integrate into Game Logic & Fix Visual Bugs
- [X] **3.1 `Insight` Ability (Diviner/Enchanter):**
    - [X] Modify `EffectProcessor.cpp` so the `HIGHLIGHT_MOVE` effect finds a valid move and sets the `ActionState` of the two gems to `ValidMoveHint`.
    - [X] In `Game.cpp`, call `m_board.clearActionStates()` at the beginning of input-handling blocks to reset highlighted gems.
- [X] **3.2 `Create Burning Tile` Ability (Elementalist):**
    - [X] **3.2.1 Refactor `BaseGem`:** Replace `int m_counter` with `float m_effectValue`, `float m_effectMax`, and `float m_periodicActivationValue`. Add getters/setters.
    - [X] **3.2.2 Define `BASE_SWAP_SPEED`:** Add `constexpr float BASE_SWAP_SPEED = 20.0f;` to `Constants.h`.
    - [X] **3.2.3 Update `EffectProcessor`:** Modify the `CREATE_BURNING_TILE` logic to initialize the new time unit properties on the target gem.
    - [X] **3.2.4 Implement `onTurnEnd()`:** Add a `virtual void onTurnEnd(Board& board, Player& player, Monster& monster)` method to `BaseGem`. Implement the logic for the `Burning` effect to deal damage based on the new formula and decrement its timer.
    - [X] **3.2.5 Update `Game::handleMatches`:** After a successful player move, add a loop to call `onTurnEnd()` for every gem on the board, passing the `speedCost` of the action.
    - [X] **3.2.6 Implement Visuals:** Update `Board::render()` to draw a horizontal bar representing the effect's duration instead of the numerical counter.
- [X] **3.3 UI for Monster HP:**
    - [X] Add a numerical display (`current/max`) to the monster's HP bar in `UIManager`.
- [X] **3.4 Update Documentation:** As each visual bug is fixed, move the corresponding item in `bugs.md` to the "Resolved" section and update the `QA_Checklist.md` with a fresh item to verify the fix.

## Phase 4: Polish Visual Effects & Animations
- [ ] **4.1 Animate Status Effects:**
    - [ ] Implement an animation when a status effect is first applied to a tile (e.g., a fiery burst for `Burning`).
    - [ ] Implement a continuous animation for active effects (e.g., a gentle flicker for the `Burning` overlay).
    - [ ] Implement an animation for when an effect expires or is cleared.
- [ ] **4.2 Animate Damage & Healing:**
    - [ ] Implement a damage animation (e.g., a red flash or screen shake) for both the player and the monster.
    - [ ] Implement a healing animation (e.g., a green glow or floating green numbers) for the player.
