# UI & UX Flow

This document outlines the conceptual blueprint for the game's User Interface (UI) and User Experience (UX), focusing on the core combat screen for the vertical slice.

---

## 1. Guiding Principles

*   **Clarity Above All:** The player must understand the game state at a glance. Critical information (HP, Mana) must be clear and unambiguous.
*   **Thematic Consistency:** UI elements should feel appropriate for a magical, fantasy setting.
*   **Future-Proof Layout:** The design should accommodate future features like status effects, abilities, and detailed character portraits without requiring a complete overhaul.

---

## 2. Combat Screen Layout (Vertical Slice)

The combat screen is designed with a 16:9 aspect ratio (e.g., 1280x720) to provide a modern, spacious feel.

### 2.1. Overall Structure
*   **Top Third (Open Space):** This area is intentionally left open. It serves as the spawn point for new gems falling onto the board and will later be used to display spell effects, damage numbers, and other visual feedback without cluttering the gameplay area.
*   **Center-Bottom (Game Board):** The 8x8 puzzle board is the focal point. It is horizontally centered and positioned in the lower portion of the screen, leaving ample space on the sides for character information. It is enclosed in a simple, clean frame to visually separate it from the UI panels.
*   **Left Panel (Player Info):** A dedicated vertical panel on the left side of the screen displays all information relevant to the player.
*   **Right Panel (Monster Info):** A mirrored vertical panel on the right side of the screen displays all information relevant to the current monster.

### 2.2. Left Panel Details (Player)
*   **Layout:** A vertical arrangement of key information.
*   **Content:**
    *   **Player Portrait:** (Placeholder) A designated area for the player's character art.
    *   **HP Bar:** A prominent health bar displaying current HP.
    *   **Mana Pools:** A clear, numerical display of the player's current mana for each gem type (Fire, Water, Earth, Light).
    *   **Status Effects:** (Placeholder) An area below the mana pools where icons for buffs and debuffs will be displayed.
    *   **Abilities:** (Placeholder) An area at the bottom of the panel for spell and ability icons.

### 2.3. Right Panel Details (Monster)
*   **Layout:** A vertical arrangement mirroring the player's panel.
*   **Content:**
    *   **Monster Portrait:** (Placeholder) A designated area for the monster's art.
    *   **HP Bar:** A prominent health bar displaying the monster's current HP.
    *   **Intent Icon:** (Placeholder) An icon indicating the monster's next action (e.g., attack, defend, cast spell).
    *   **Status Effects:** (Placeholder) An area below the HP bar for icons showing buffs and debuffs affecting the monster.

### 2.4. User Flow during Gameplay
1.  **Input:** The player interacts exclusively with the centered game board, using either click-to-select or drag-and-drop to swap gems.
2.  **Feedback:**
    *   When a match is made, the consequences are immediately reflected in the UI panels (e.g., the monster's HP bar decreases, the player's mana values increase).
    *   Gems are destroyed on the board, and new gems are animated falling from the open space at the top of the screen to fill the gaps.
    *   If no moves are available, the board briefly blanks out and then reappears with a new, valid layout.
3.  **Game State:** The player can assess their resources (HP, mana) on the left and the enemy's status on the right to make strategic decisions about their next move on the board.