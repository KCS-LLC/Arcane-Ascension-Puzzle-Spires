# Treasure System: The Art of Creation

This document outlines the design and philosophy behind the "Merge and Upgrade" treasure system.

## Overview

In our game, players will encounter special, resource-focused "Treasure" pieces on the game board. The primary goal is not simply to match and clear these pieces, but to strategically merge them to forge items of greater and greater value. This system transforms the core puzzle mechanic into a dynamic exercise in creation and growth, where the player acts as an alchemist, transmuting base materials into powerful treasures.

## The Core Mechanic: Merge & Upgrade

Every match a player makes is an act of fusion. Instead of being destroyed, the matched pieces are consumed, their essence combining to create a single, more valuable item. This reframes the core action: players are creators, actively forging wealth and resources rather than just collecting points.

### Treasure Tiers

Treasure exists in a clear hierarchy, providing players with a visible path of progression. The tiers are as follows:

1.  **Coin**
2.  **Coin Pile**
3.  **Coin Bag**
4.  **Coin Bag Bundle**
5.  **Treasure Chest**

### How It Works: The Player Experience

1.  **The Standard Merge:** When a player makes a standard match of three Treasure pieces of the same tier, two are removed, and the third transforms, upgrading to the next tier. For example, matching three **Coin** pieces will result in one of them becoming a **Coin Pile**.

2.  **The Bonus Merge:** Creating a match of four Treasure pieces in a single line rewards foresight with an exceptional bonus. Instead of just one, **two** of the matched pieces are upgraded. This is the key to rapidly increasing the value on your board.

3.  **The Grand Fusion:** A linear match of five Treasure pieces triggers a "Grand Fusion." This powerful merge consumes all five pieces to forge three of the next tier treasure next to each other, that should then settle and if still in a line will merge again on the next update. 

4.  **The Point of No Return:** During the final phase of a level or after a key objective is met, new base-level Treasure components will stop appearing. This creates a climactic challenge where players must work with what they have, strategically upgrading their remaining resources before the end.

## The Philosophy: Creation and Growth

This system is designed to reward strategic foresight and provide a powerful sense of accomplishment.

*   **A Tangible Sense of Progression:** The board becomes a direct reflection of the player's skill. Watching a chaotic grid of scattered **Coins** evolve into gleaming **Treasure Chests** provides a constant, visible feedback loop of success. The player is not just clearing a level; they are cultivating a garden of wealth.

*   **Encouraging Strategic Foresight:** The mechanic inherently rewards long-term planning. A simple, immediate match might be tempting, but a savvy player will start seeing the board differently. They will begin asking questions like, "How can I set up a four-piece match for a double upgrade?" or "If I make this move, will it position me for an even better merge later?" This adds a rich, chess-like layer to the puzzle.

*   **Compounding Value:** The core loop is about reinvesting your successes. Every match is an investment that yields a higher-value asset. A well-executed cascade that results in a high-tier upgrade feels like a massive payoff, turning a series of small, smart plays into a huge reward.

## Final Goal

The Treasure forged on the board is not just for score. The total value of all collected Treasure pieces is converted into a special currency used to acquire powerful and unique items, gear, and abilities, directly fueling the player's overall progression in the game.

---

## Mechanic Clarifications & Edge Cases

-   **Match-3 Upgrade Placement:** When a match of three is made, the upgraded piece replaces the tile the player interacted with (either the one they moved or the one they swapped with). If the match was created by a cascade and not a direct player swap, the upgraded piece replaces the **bottom-most, then left-most** tile of the matched set.
-   **Match-4 Upgrade Placement:** The first upgraded piece follows the rule above. The second upgraded piece replaces the next available bottom-most, then left-most tile in the matched set.
-   **Grand Fusion Cascades:** The three upgraded pieces from a match-5 are created and settle on the board. Any new matches they form will be resolved on the next board update/cascade check, not instantaneously. This means a match of 5 **Coins** will resolve, on the next update, into a single Tier 3 **Coin Bag**.
-   **Strict Matching Rules:** Treasure tiles can only be matched with other Treasure tiles of the exact same tier (e.g., **Coin** with **Coin**, **Coin Pile** with **Coin Pile**). A **Coin** cannot be matched with a **Coin Pile**.
-   **Destruction vs. Matching:** If a Treasure tile is destroyed by a spell, ability, or special gem effect (e.g., a Bomb), it is simply removed from the board. No value is awarded. Value is only gained when Treasure tiles are matched and upgraded according to the rules.
