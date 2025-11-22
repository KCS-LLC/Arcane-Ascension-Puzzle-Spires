# Tower Climb Refactor Checklist

This checklist tracks the detailed steps for re-implementing the transition from the Judgement phase to the Tower Climb exploration mode.

## Phase 1: Restore the `Exploration` State
- [ ] **Constants.h:** Add `Exploration` to the `GameState` enum.
- [ ] **UIManager.h:** Add member variables for exploration UI elements (room title, description).
- [ ] **UIManager.cpp:** Add `case GameState::Exploration:` to the `render` function's switch statement to draw the new UI.
- [ ] **UIManager.cpp:** Add logic to the `update` function to set the text for the exploration UI based on the current room.

## Phase 2: Implement the Tower Climb Transition
- [ ] **Game.h:** Declare the `void startTowerClimb();` private member function.
- [ ] **Game.cpp:** Implement the `startTowerClimb()` function to set the game mode, state, and load the initial floor/room data.
- [ ] **Game.cpp:** In `handleInput`, replace the manual state change after attunement assignment with a call to `startTowerClimb()`.

## Phase 3: Re-implement Room Transitions
- [ ] **Game.h & Game.cpp:** Add a simplified `moveToRoom(int destinationRoomId)` function.
- [ ] **UIManager.cpp:** In `handleEvent`, add logic to detect clicks on placeholder doors and return a `ChangeRoom` action.
- [ ] **Game.cpp:** In `handleInput`, add a check for the `ChangeRoom` UIAction and call `moveToRoom()` accordingly.