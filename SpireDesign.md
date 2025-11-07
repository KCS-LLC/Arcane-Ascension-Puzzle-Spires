
# Spire Design & Generation

This document details the structure of the Spire, the game's main dungeon, and the procedural generation systems that create its floors.

## 1. Spire Structure

The Spire is a procedurally generated dungeon with multiple floors.

*   **Domains:** Floors are grouped into **Domains** based on their power level, corresponding to the gemstone ranks (e.g., Floors 1-5 are the Quartz Domain). Each Domain features appropriately ranked monsters and loot.
*   **Spire Guardians:** At the end of each Domain (e.g., on Floor 5), the player must face a powerful **Spire Guardian**. Defeating the Guardian is a major milestone, providing significant rewards and allowing access to the next Domain.
*   **Exiting the Spire:** After defeating a Spire Guardian, the player is given the option to safely exit the spire, securing all the loot they have gathered for the House. Rare, expensive consumables (like an Escape Bell) may allow for an early exit from other floors.

## 2. Floor Generation - The Branching Graph

Each floor is a **branching graph of rooms** connected by magical teleporters, not a physical grid. This design emphasizes strategic choice and exploration over navigating a maze.

*   **Generation Philosophy:** The algorithm generates a floor by creating a graph of nodes (rooms) and edges (teleporters).
    *   A **Main Path** is always generated, guaranteeing a solvable route from the **Entrance** to the **Spire Guardian** room.
    *   **Minimum Path Length:** The shortest possible path from the Entrance to the Spire Guardian will always consist of at least three intermediate rooms, ensuring a minimum floor length. This rule does not apply to hidden or secret exits.
    *   **Side Branches** are generated off the main path. The type of challenge in a side branch is heavily influenced by the color of the teleporter door chosen to enter it.
    *   **Interconnectivity & Loops:** Rooms can have up to five teleporter exits. Some of these may connect to rooms in different branches, creating loops and shortcuts. This allows for more complex exploration and strategic pathing.
    *   **Backtracking** is allowed and encouraged. Teleporters are two-way, allowing the player to return to previous rooms to explore different branches.
    *   **Secret Exits** can be found in some rooms, offering shortcuts, bonus rewards, or even the ability to skip floors entirely.

## 3. Teleporter Doors & Path Themes

Each exit from a room is a color-coded teleporter door, signaling the theme of the next room or branch. Choosing a path color at the start of a floor will increase the likelihood of encountering more challenges of that type.

| Door Color | Associated Theme & Room Types |
| :--- | :--- |
| **Red** | **Direct Combat:** Primarily Combat Arenas. |
| **Orange** | **Combat & Agility:** A mix of Combat Arenas and Trap Gauntlets. |
| **Yellow** | **Agility & Puzzles:** Primarily Trap Gauntlets, but may have light combat solved by agility. |
| **Green** | **Strength & Endurance:** Physical challenges, rooms with environmental hazards that require Vigor. |
| **Blue/Indigo/Violet** | **Mental & Magical:** Puzzle Chambers, strategy games (e.g., Crowns), riddle-based encounters, magical traps. |
| **Copper** | **Common Rewards:** Small treasure troves, basic merchants, a single common magical item. |
| **Silver** | **Significant Rewards:** Large treasure troves, a choice between several items, a floor warp, or a minor character enhancement. |
| **Gold** | **Major Rewards:** Enormous treasures, unique artifacts, powerful blessings, permanent stat growth, or other rare, game-changing encounters. |
| **White** | **Sanctuary:** Safe rooms, Mana Fonts. Often found just before a Spire Guardian room. |

**A Note on Reward Room Generation:**
The tier of a reward room (Copper, Silver, or Gold) is determined probabilistically. The base chance is heavily skewed towards Copper (e.g., 85% Copper, 14% Silver, 1% Gold). As the player ascends to deeper floors, the chance of encountering Silver and Gold doors gradually increases.

## 4. Room Types

The Spire contains various types of rooms, each with a different purpose and challenge.

| Room Type | Purpose |
| :--- | :--- |
| **Entrance** | The starting point of the floor. Typically offers several different colored doors. |
| **Spire Guardian**| A powerful boss guarding the teleporter to the next floor. |
| **Combat Arena** | Standard enemy encounters that test the player's build and combat skill. |
| **Puzzle Chamber**| Challenges that test Wit and require clever use of spells or items. |
| **Trap Gauntlet** | Rooms filled with environmental hazards that test Speed and Vigor. |
| **Strength Challenge**| Rooms requiring high Vigor or specific items to overcome an obstacle for a reward. |
| **Treasure Vault**| Contains valuable loot, often protected by a trap, puzzle, or a tough enemy. At least one is guaranteed on every 5th floor; other floors may have one or none. |
| **Sanctuary** | A safe room for rest and recovery. Can only be used once per floor. |
| **Merchant** | A place to buy and sell items with a mysterious spire merchant. |
| **Lore Repository**| Contains scrolls and artifacts that can contribute to **Generational Knowledge**. |
| **Spire Encounter**| A rare room that contains a unique event, mini-puzzle, or narrative choice. |
| **Mana Font** | A rare room that fully restores the player's mana and removes mana scarring for a time. |
