# Refactor Checklist: Object-Oriented Gem Architecture

This document tracks the step-by-step process of refactoring the gem and mana handling system to a fully normalized, data-driven, and object-oriented architecture.

**Phase 1: Data Foundation (COMPLETE)**
- [x] **Step 1: Create and Populate New JSON Data Files**
  - [x] Create `primary_gem_types.json` with primary gem categories.
  - [x] Create `secondary_gem_types.json` with prefixed IDs.
  - [x] Create `gems.json` (replacing `gem_definitions.json`).

**Phase 2: C++ Architecture Refactor (COMPLETE)**
- [x] **Step 2: Implement Core C++ Structs and Enums**
  - [x] Create `PrimaryGemType.h` with the `PrimaryGemType` enum.
  - [x] Consolidate all data structures into `Structs.h`.
  - [x] Remove `SpireData.h`.

- [x] **Step 3: Implement Object-Oriented Gem Classes**
  - [x] Create `BaseGem.h`, the abstract base class with a virtual `onMatch` method.
  - [x] Create concrete subclasses: `ManaGem`, `AttackGem`, `TreasureGem`.
  - [x] Implement the specific `onMatch` logic for each subclass.
  - [x] Update `CMakeLists.txt` to include new .cpp files.

- [x] **Step 4: Refactor `DataManager` to Load All New Data**
  - [x] Add include guards to `DataManager.h`.
  - [x] Update `DataManager.h` with member maps and loading functions for all three JSON files.
  - [x] Implement `from_json` parsers in `DataManager.cpp`.
  - [x] Implement the new loading functions in `DataManager.cpp`.
  - [x] Update `DataManager` constructor to call new loading functions.

- [x] **Step 5: Implement a Gem Factory**
  - [x] Create a `GemFactory` class.
  - [x] Implement `GemFactory` to create `std::unique_ptr<BaseGem>` instances.

- [x] **Step 6: Refactor the `Board` to Use Gem Objects**
  - [x] Change the board's internal grid to `std::vector<std::unique_ptr<BaseGem>>`.
  - [x] Update `Board.h` and `Board.cpp` to use the `GemFactory` to create gems.
  - [x] Implement polymorphic `onMatch` call in `Game::resolveMatches`.

- [x] **Step 7: Refactor Game Loop and UI Integration**
  - [x] Update `Game.h` and `Game.cpp` to use new `Board` and `BaseGem` interfaces.
  - [x] Update `UIManager.h` and `UIManager.cpp` to align with new `GameState` enums and `setupTrial`.
  - [x] Fix `Player.h` and `Player.cpp` for consistent member naming and correct `JudgementResults` usage.

**Phase 3: Final Integration and Cleanup (COMPLETE)**
- [x] **Step 8: Refactor Attunement Data and Loading**
  - [x] Edit `attunements.json` and `spells.json` to use integer IDs.
  - [x] Update C++ parsers to read integer IDs.

- [ ] **Step 9: Final Code Cleanup**
  - [ ] Remove obsolete `stringToGemSubType` (from `StringUtils.h` and `StringUtils.cpp`).
  - [ ] Restore full `UIManager` functionality.
  - [ ] Commit the completed refactor.

The major architectural work is done. The remaining steps are cleanup and restoring the UI.
