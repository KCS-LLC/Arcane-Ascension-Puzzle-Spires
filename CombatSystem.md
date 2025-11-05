# Combat System

This document outlines the core mechanics of player vs. monster combat in **Arcane Ascension: The Puzzle Spires**.

## 1. The Match-3 Board

Combat takes place on a match-3 board. The player's primary action is matching gems to gather resources and attack the enemy.

*   **Mana Gems:** Matching mana gems (Fire, Water, Earth, etc.) adds mana to the player's corresponding mana pool. A key mechanic is that **only the mana types usable by the player and the current opponent(s) will populate the board during an encounter.**
*   **Skulls:** Matching Skull gems deals direct physical damage to the opponent. The damage is influenced by the heir's Vigor stat and equipped weapon.
*   **Coins/Treasure:** Matching Coin/Treasure gems provides the heir with Marks (in-run currency).

## 2. The Action & Turn System

Combat is turn-based, governed by the **Speed** stat.

*   **Player Actions:** Each action the player takes—matching gems, casting a spell, using an item—has a 'speed cost'.
*   **Monster Actions:** Monsters have a Speed stat. When the player's cumulative speed costs equal or exceed the monster's Speed, the monster takes its turn.
*   **Strategic Advantage:** A high Speed stat allows an heir to perform multiple actions before the enemy gets to act.

## 3. Core Combat Mechanics

*   **Mana Dynamics:** The principle of "Like deflects like, opposites negate" is in full effect. Using opposing mana types can create powerful explosions on the board, while using the same type can deflect incoming spells.
*   **Shroud (Defensive Shield):** Both the player (at Carnelian rank) and higher-ranked monsters possess a Shroud. This is a regenerating shield with its own HP that must be depleted before the target's main HP can be damaged. Shrouds can be infused with mana to grant them special properties.
*   **Attunement & Spells:** The player's Attunement determines their available spells. Casting spells costs mana from the appropriate pool and has a speed cost.
*   **Status Effects:** Both players and monsters can apply buffs (positive effects) and debuffs (negative effects) to each other.

## 4. Monster AI



On their turn, a monster will choose an action from its ability list. A monster's behavior is determined by its Wit stat.



*   **Standard AI (All Monsters):** All monsters can perform basic actions like direct attacks, casting spells, or applying status effects. Their decision-making is straightforward, often prioritizing their most damaging attacks.



*   **Advanced AI (High-Wit Monsters):** Monsters with a Wit stat at or above a certain **"Wit Threshold"** are far more cunning. In addition to their basic actions, they can choose to **make a match on the puzzle board**. This advanced tactic allows them to gather mana for more powerful spells, deny the player valuable gem matches, and create a much more dynamic and challenging opponent.
