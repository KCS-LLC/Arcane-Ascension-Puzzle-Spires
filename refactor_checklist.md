# Refactor Checklist: Object-Oriented Gem Architecture

This document tracks the step-by-step process of refactoring the gem system to a fully normalized, data-driven, and object-oriented architecture.

**Phase 1: Data Foundation (COMPLETE)**
- [x] **Step 1: Create and Populate New JSON Data Files**
  - [x] Create `primary_gem_types.json` with primary gem categories.
  - [x] Create `secondary_gem_types.json` with prefixed IDs.
  - [x] Create `gems.json` (replacing `gem_definitions.json`).

**Phase 2: C++ Architecture Refactor**
- [ ] **Step 2: Implement Core C++ Structs and Enums**
  - [ ] Create `PrimaryGemType.h` with the `PrimaryGemType` enum.
  - [ ] Create `Structs.h` containing:
    - [ ] `SecondaryGemTypeData` struct (for `secondary_gem_types.json`).
    - [ ] `GemCatalogEntry` struct (for `gems.json`).
  - [ ] Implement `from_json` specializations for the new structs.

- [ ] **Step 3: Implement Object-Oriented Gem Classes**
  - [ ] Create `BaseGem.h`, the abstract base class with a virtual `onMatch` method.
  - [ ] Create concrete subclasses: `ManaGem`, `AttackGem`, `TreasureGem`, etc., inheriting from `BaseGem`.
  - [ ] Implement the specific `onMatch` logic for each subclass.

- [ ] **Step 4: Refactor `DataManager` to Load All New Data**
  - [ ] Update `DataManager.h` with member maps and loading functions for all three JSON files.
  - [ ] Implement the new loading functions in `DataManager.cpp`.
  - [ ] Remove all old gem-related loading logic.

- [ ] **Step 5: Implement a Gem Factory**
  - [ ] Create a `GemFactory` class that takes a `GemSubType` ID.
  - [ ] The factory will use the data from `DataManager` to determine the `PrimaryGemType`.
  - [ ] It will then create and return a `std::unique_ptr<BaseGem>` of the correct subclass (e.g., `ManaGem`, `AttackGem`).

- [ ] **Step 6: Refactor the `Board` to Use Gem Objects**
  - [ ] Change the board's internal grid from `Gem` structs to `std::vector<std::unique_ptr<BaseGem>>`.
  - [ ] Update board logic (`initialize`, `findMatches`, etc.) to use the `GemFactory` to create gems.
  - [ ] Rewrite `resolveMatches` to be a simple, polymorphic call: `gem->onMatch(boardContext)`.

**Phase 3: Final Integration and Cleanup**
- [ ] **Step 7: Refactor Attunement Data and Loading**
  - [ ] Edit `attunements.json` to use integer IDs from `gems.json`.
  - [ ] Update `DataManager::loadAttunements` to read these IDs.

- [ ] **Step 8: Final Code Cleanup**
  - [ ] Delete the obsolete `stringToGemSubType` function.
  - [ ] Move `#include "json.hpp"` to `PCH.h` and remove redundant includes.
  - [ ] Delete the now-unused `Gem.h` file (the simple struct).

