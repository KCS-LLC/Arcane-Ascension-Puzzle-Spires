
# Spire Design & Generation

This document details the structure of the Spire, the game's main dungeon, and the procedural generation systems that create its floors.

## 1. Spire Structure

The Spire is a procedurally generated dungeon with multiple floors.

*   **Domains:** Floors are grouped into **Domains** based on their power level, corresponding to the gemstone ranks (e.g., Floors 1-5 are the Quartz Domain). Each Domain features appropriately ranked monsters and loot.
*   **Spire Guardians:** At the end of each Domain (e.g., on Floor 5), the player must face a powerful **Spire Guardian**. Defeating the Guardian is a major milestone, providing significant rewards and allowing access to the next Domain.
*   **Exiting the Spire:** After defeating a Spire Guardian, the player is given the option to safely exit the spire, securing all the loot they have gathered for the House. Rare, expensive consumables (like an Escape Bell) may allow for an early exit from other floors.

## 2. Floor Generation

Each floor is a grid of rooms, procedurally generated to create a unique layout for every run. The algorithm ensures:
*   The **Entrance** is always in a central room.
*   There is always at least one path to the **Spire Guardian** room, which contains the stairs to the next floor.
*   A certain percentage of rooms are **Dead Ends**, which are riskier to explore but often contain the best treasure.

## 3. Room Types

The Spire contains various types of rooms, each with a different purpose and challenge.

| Room Type | Icon | Purpose |
| :--- | :--- | :--- |
| **Entrance** | E | The starting point of the floor. |
| **Spire Guardian**| G | A powerful boss guarding the stairs to the next floor. |
| **Combat Arena** | C | Standard enemy encounters. |
| **Puzzle Chamber**| P | Challenges that test Wit and require clever use of spells. |
| **Trap Gauntlet** | T | Rooms filled with environmental hazards that test Speed and Vigor. |
| **Treasure Vault**| $ | Contains valuable loot, often protected by a trap or a tough enemy. |
| **Sanctuary** | + | A safe room for rest and recovery. Can only be used once. |
| **Merchant** | M | A place to buy and sell items with a mysterious spire merchant. |
| **Lore Repository**| L | Contains scrolls and artifacts that can contribute to **Generational Knowledge**. |
| **Spire Encounter**| ? | A rare room that contains a unique event. This could be a mini-puzzle, a special combat challenge, a narrative choice, or a mysterious merchant. The chance of finding these can be increased through heir traits and House research. |
| **Mana Font** | * | A rare room that fully restores the player's mana and removes mana scarring for a time. |
