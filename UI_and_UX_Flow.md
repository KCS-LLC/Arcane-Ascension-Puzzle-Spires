
# UI & UX Flow

This document provides a conceptual blueprint for the game's User Interface (UI) and the player's User Experience (UX). It outlines the layout of key screens and the intended flow of navigation through the game's core loops, ensuring the game's complex systems are presented in an intuitive and engaging manner.

---

## 1. Guiding Principles

*   **Clarity Above All:** The player should be able to understand the state of the game at a glance. Critical information (HP, Mana, Turn Order) must be clear and unambiguous.
*   **Thematic Consistency:** All UI elements should feel like they belong in the world of Arcane Ascension. Menus, icons, and layouts will draw from the established anime-inspired, magical aesthetic.
*   **Information on Demand:** The game has deep systems. We will avoid clutter by presenting high-level information by default, with more detailed stats and descriptions available via a simple "tap and hold" or a dedicated details button.

---

## 2. Core User Flow

This describes the player's primary journey through the game.

1.  **Main Menu -> Manor Hub:** The player starts in the Manor, the central hub for all meta-game activity.
2.  **Manor Hub -> Heir Selection:** Tapping the "Enter the Spire" button brings up the Heir Selection screen, where the player chooses their next character.
3.  **Heir Selection -> The Judgement:** After choosing an heir, the game transitions to the three-phase Judgement sequence.
4.  **The Judgement -> Manor Hub (Preparation):** Upon completing the Judgement, the player is returned to the Manor to equip items from the Vault and make final preparations.
5.  **Manor Hub -> The Spire:** The player begins their run, entering Floor 1 of the Spire.
6.  **Spire Gameplay:** The player navigates the floor map room by room, engaging in combat or encounters.
7.  **Combat -> Spire Gameplay:** After a battle, the player is returned to the floor map to continue exploring.
8.  **Exit Spire -> Manor Hub:** Upon dying or choosing to exit, the player returns to the Manor Hub. A summary screen details the run's accomplishments, the resources transferred to the House, and any new Generational Knowledge gained. The loop then repeats.

---

## 3. Conceptual Screen Wireframes

### 3.1. The Manor Hub
*   **Layout:** A 2D background image of the Manor/Estate, which can visually evolve with upgrades. Key locations and functions (Vault, Library, Marketplace, Training Grounds, Spire Gate) are represented by easily identifiable buttons or controls arranged for quick access.
*   **Interaction:** The player taps on a button to access its corresponding menu or function directly.
*   **Persistent HUD:** A top bar always displays the **House Treasury (Coins)**, the current **House Piety** (represented by a glowing goddess symbol), and a **Time Meter** indicating when the next financial obligation is due.

### 3.2. The Combat Screen
*   **Layout:** A 2D background image of the Manor/Estate, which can visually evolve with upgrades. Key locations and functions (Vault, Library, Marketplace, Training Grounds, Spire Gate) are represented by easily identifiable buttons or controls arranged for quick access.
*   **Interaction:** The player taps on a button to access its corresponding menu or function directly.
*   **Persistent HUD:** A top bar always displays the **House Treasury (Coins)**, the current **House Piety** (represented by a glowing goddess symbol), and a **Time Meter** indicating when the next financial obligation is due.

### 3.3. The Combat Screen
*   **Center:** The 8x8 match-3 puzzle board.
*   **Left Side:** The **Heir's Portrait**, with their **HP Bar** and **Shroud Bar** below it. Below that, a concise display of the heir's **Mana Pools** (only showing the types they can actually use).
*   **Right Side:** The **Monster's Portrait**, with its **HP Bar** and **Shroud Bar**.
*   **Top:** A **Speed Meter** or **Turn Order Bar**.
*   **Top Right:** An **Enemy Intent** icon. By default, this icon is obscured or shows a generic "?", hiding the monster's specific next action. The ability to decipher this icon and see the monster's intent (e.g., a sword for an attack, a spell icon for a spell) is a form of progression unlocked via Diviner abilities, a high Wit stat, or special items.
*   **Bottom:** A scrollable bar with icons for the heir's available **Spells and Abilities**.

### 3.4. "Fog of War" on Stats (Information as Reward)
A core UX principle is that precise numerical data is a privilege, not a default.
*   **Default View:** All HP, Shroud, and Mana bars are displayed as fractions of a whole, without explicit numbers (e.g., "45/150"). The player can see the bar is about one-third full, but not the exact value.
*   **Unlocking Information:** The ability to see hard numerical values is a form of progression, unlocked via:
    *   **Items:** Equipping an item like a "Mana Watch" will reveal the numerical values on the mana pools. A "Scouter's Lens" might reveal them on the enemy's HP bar.
    *   **Abilities/Traits:** A Diviner might have a passive ability to see all numerical values. A high Wit stat might reveal numbers on any target the player has critically hit.

### 3.5. Heir Selection Screen
*   **Layout:** A side-by-side presentation of three potential heirs.
*   **Information:** Each heir's card displays their **Portrait**, **Name**, and a clear list of their positive and negative **Traits**.

### 3.6. Generational Knowledge Screen
*   **Layout:** A classic branching "tech tree" view.
*   **Interaction:** The player can pan across the tree. Tapping a node brings up a description of the bonus and its Research Point cost. A "tap and hold" action commits the points and unlocks the bonus.

---

## 4. Key Information & Visual Language

*   **Gem Auras:** Gems with Auras will have a distinct, colored glow corresponding to their rank (e.g., a red glow for Carnelian, an orange glow for Sunstone).
*   **Static Effect Tiles:** These tiles will have a clear icon overlay representing their effect (e.g., a flame for Burning, a skull for Poison) and a small number in the corner representing their countdown timer.
*   **Tooltips:** Tapping and holding on almost any element (a spell icon, a status effect, a monster) will bring up a concise tooltip explaining its function and relevant numbers.
