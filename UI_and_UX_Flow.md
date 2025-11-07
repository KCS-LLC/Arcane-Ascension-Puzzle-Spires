# UI & UX Flow

This document outlines the conceptual blueprint for the game's User Interface (UI) and User Experience (UX), focusing on the core combat screen for the vertical slice.

---

## 1. Guiding Principles

*   **Clarity Above All:** The player must understand the game state at a glance. Critical information (HP, Mana) must be clear and unambiguous.
*   **Thematic Consistency:** UI elements should feel appropriate for a magical, fantasy setting.
*   **Future-Proof Layout:** The design should accommodate future features like status effects, abilities, and detailed character portraits without requiring a complete overhaul.

---

## 2. Screen Layouts

### 2.1. Overall Structure
The game transitions between two primary screens: **Exploration** and **Combat**.

### 2.2. Combat Screen Layout
*   **Top Third (Open Space):** This area is intentionally left open. It serves as the spawn point for new gems falling onto the board and will later be used to display spell effects, damage numbers, and other visual feedback without cluttering the gameplay area.
*   **Center-Bottom (Game Board):** The 8x8 puzzle board is the focal point. It is horizontally centered and positioned in the lower portion of the screen, leaving ample space on the sides for character information. It is enclosed in a simple, clean frame to visually separate it from the UI panels.
*   **Left Panel (Player Info):** A dedicated vertical panel on the left side of the screen displays all information relevant to the player.
*   **Right Panel (Monster Info):** A mirrored vertical panel on the right side of the screen displays all information relevant to the current monster.

### 2.3. Left Panel Details (Player)
*   **Layout:** A vertical arrangement of key information.
*   **Content:**
    *   **Player Portrait:** (Placeholder) A designated area for the player's character art.
    *   **HP Bar:** A prominent health bar displaying current HP.
    *   **Mana Pools:** A clear, numerical display of the player's current mana for each gem type (Fire, Water, Earth, Light).
    *   **Status Effects:** (Placeholder) An area below the mana pools where icons for buffs and debuffs will be displayed.
    *   **Abilities:** (Placeholder) An area at the bottom of the panel for spell and ability icons.

### 2.4. Right Panel Details (Monster)
*   **Layout:** A vertical arrangement mirroring the player's panel.
*   **Content:**
    *   **Monster Portrait:** (Placeholder) A designated area for the monster's art.
    *   **HP Bar:** A prominent health bar displaying the monster's current HP.
    *   **Intent Icon:** (Placeholder) An icon indicating the monster's next action (e.g., attack, defend, cast spell).
    *   **Status Effects:** (Placeholder) An area below the HP bar for icons showing buffs and debuffs affecting the monster.

### 2.5. Exploration Screen Layout
When not in combat, the player sees the Exploration screen. This view is designed for clear, simple navigation.
*   **Layout:** A centered, minimalist layout. The combat UI panels are hidden.
*   **Content:**
    *   **Room Name:** The unique, procedurally generated name of the current room is displayed prominently at the top of the screen (e.g., "Forgotten Chamber").
    *   **Door Buttons:** The exits to other rooms are displayed as a series of buttons in the center of the screen. The layout wraps dynamically to handle any number of doors.
        *   **Unexplored Rooms:** Doors leading to new rooms are colored according to the destination's type and labeled with the color (e.g., "Red Door"). The color themes are: Red (Combat), Orange (Agility), Yellow (Trap), Green (Endurance), Blue (Puzzle), Indigo (Magic), Violet (Special), White (Sanctuary/Boss), and Metallics (Treasure).
        *   **Explored Rooms:** Doors leading to rooms the player has already visited are labeled with the destination room's name (e.g., "Whispering Hall"). This allows the player to navigate without having to memorize the floor layout.

### 2.6. User Flow
1.  **Exploration:** The player starts in the "Entrance" room. They see the room name and a set of colored doors.
2.  **Decision:** The player clicks a door to move to a new room.
3.  **Encounter:** If the new room is a Combat room, the UI transitions to the Combat Screen layout.
4.  **Combat:** The player fights the monster using the match-3 board.
5.  **Victory:** Upon defeating the monster, the room is marked as "cleared." The UI transitions back to the Exploration Screen.
6.  **Return:** The player can now see the exits from the cleared room. If they return to a previously visited room (like the Entrance), they will see its name on the door. If they re-enter the cleared combat room, no new monster will appear.