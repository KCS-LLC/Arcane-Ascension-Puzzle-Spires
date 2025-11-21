# Refactor Checklist & Bug Fix Log

This document tracks the step-by-step process of refactoring the gem system and the subsequent bug-fixing phase.

**Phase 1: Data Foundation (COMPLETE)**
- [x] All new JSON data files created and populated.

**Phase 2: C++ Architecture Refactor (COMPLETE)**
- [x] All C++ structs, enums, and classes implemented for the new architecture.
- [x] `DataManager`, `GemFactory`, `Board`, `Game`, and `Player` classes fully refactored.
- [x] Project compiles cleanly with the new object-oriented architecture.
- [x] A commit point (`d8d3ee1`) exists for this stable state.

**Phase 3: Bug Fixing (IN PROGRESS)**

- **Target:** Address visual and logical bugs discovered during the first playable test.
- **Bugs Identified:**
  - **[BUG-1] Blank Spaces / Invisible Gems:** Only some gems are visible on the board (e.g., "sword" tiles), creating a checkerboard of blank spaces.
  - **[BUG-2] Incorrect Positioning:** The game board and all UI elements are rendered at the top-left of the screen instead of being centered or properly positioned.
  - **[BUG-3] Missing UI Elements:** The main HUD (player/monster HP, mana bars, etc.) is not being rendered, even though the `UIManager`'s render function is being called.
  - **[BUG-4] No Cascade Matches:** The game loop does not check for new matches after gems fall, preventing cascading combos.

- **Next Steps:**
  - [ ] **[DIAGNOSE]** Add detailed logging to `Game.cpp`, `Board.cpp`, and `UIManager.cpp` to trace the flow of data at runtime.
  - [ ] **[FIX]** Address the root causes of the identified bugs based on the diagnostic output.
  - [ ] **[VERIFY]** Compile and run the game to confirm fixes.
  - [ ] **[CLEANUP]** Remove diagnostic logging once all bugs are resolved.

**Phase 4: Final Cleanup (PENDING)**

- [ ] **Step 9: Remove Obsolete String Conversion Functions**
  - [ ] Delete `stringToGemSubType` (and related `gemSubTypeToString`) from `StringUtils.h` and `StringUtils.cpp`.

- [ ] **Step 10: Restore Full `UIManager` Functionality**
  - [ ] Implement all detailed rendering logic for HUD elements (HP bars, mana bars, etc.) in `UIManager.cpp`.

- [ ] **Step 11: Final Commit**
  - [ ] Create a final commit with all cleanup and remaining fixes.
