# Judgement Phase Implementation Plan - REVISED

This plan prioritizes safety and stability, using atomic changes, immediate compilation, and frequent commits to prevent major regressions.

---

### New Core Principles

1.  **Atomic Changes:** Only one logical change will be made at a time.
2.  **Compile After Every Change:** After *every single file modification*, `cd VerticalSlice/build; mingw32-make` will be run. A successful compilation is required to proceed.
3.  **Frequent, Stable Commits:** As soon as a small, atomic change is implemented and confirmed to compile, it will be committed.
4.  **Read Before Write:** `read_file` will be used before any `replace` or `write_file` to ensure full context.

---

### Implementation Plan

#### **Phase 1: Foundational Data Structures & State (Goal: Get the basic data types compiling)**

*   **Step 1.1: Create Data-Only Header.**
    *   **Action:** Create a new, self-contained header file: `VerticalSlice/src/Judgement.h`. This file will *only* contain the `JudgementTrialType`, `JudgementTrial`, and `JudgementResults` structs. It will not be included anywhere yet.
    *   **Verification:** Check that the file exists.

*   **Step 1.2: Add Header to Build System.**
    *   **Action:** Add `src/Judgement.h` to the `add_executable` list in `VerticalSlice/CMakeLists.txt`.
    *   **Verification:** Run `cd VerticalSlice/build; mingw32-make`. Project must compile without errors.

*   **Step 1.3: Update Game State Enum.**
    *   **Action:** Modify the `GameState` enum in `VerticalSlice/src/Constants.h` to replace the single `Judgement` state with `Judgement_Intro`, `Judgement_TacticalTrial`, `Judgement_ManaAffinityTrial`, and `Judgement_Summary`.
    *   **Verification:** Run `cd VerticalSlice/build; mingw32-make`. This will likely fail in `Game.cpp` (expected).

*   **Step 1.4: Fix Game State Initialization.**
    *   **Action:** Modify the `Game::Game()` constructor in `VerticalSlice/src/Game.cpp`. Change `currentState` initialization from `GameState::Judgement` to `GameState::Judgement_Intro`.
    *   **Verification:** Run `cd VerticalSlice/build; mingw32-make`. Project must compile successfully.

*   **Step 1.5: Commit Stable Base.**
    *   **Action:** Commit the changes with the message: `feat(judgement): Add foundational data structures and GameState`.
    *   **Verification:** Confirm commit with `git log -n 1`.

#### **Phase 2: Data Loading & Management (Goal: Load trial data from JSON)**

*   **Step 2.1: Create Trial Data Files.**
    *   **Action:** Create the three JSON files (`trial_power.json`, `trial_haste.json`, `trial_control.json`) in `VerticalSlice/data/`.
    *   **Verification:** Check that the files exist.

*   **Step 2.2: Prepare DataManager.**
    *   **Action:** In `DataManager.h`, include `Judgement.h`. Add a new private member: `std::vector<JudgementTrial> m_judgementTrials;` and a public accessor: `const std::vector<JudgementTrial>& getJudgementTrials() const;`.
    *   **Verification:** Run `cd VerticalSlice/build; mingw32-make`.

*   **Step 2.3: Implement Loading Logic.**
    *   **Action:** In `DataManager.cpp`, implement a new private function `loadJudgementTrials()` that reads the three JSON files and populates the `m_judgementTrials` vector.
    *   **Verification:** Run `cd VerticalSlice/build; mingw32-make`.

*   **Step 2.4: Integrate Loading Logic.**
    *   **Action:** Call `loadJudgementTrials()` from the `DataManager::DataManager()` constructor.
    *   **Verification:** Run `cd VerticalSlice/build; mingw32-make`.

*   **Step 2.5: Commit Data Loading.**
    *   **Action:** Commit the changes with the message: `feat(judgement): Implement loading of trial data in DataManager`.
    *   **Verification:** `git log -n 1`.