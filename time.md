# Game Time and Calendar System

This document outlines the core concepts of how time, turns, and actions are handled in the game. The system is designed to be turn-based from the player's perspective, but it uses a granular "time unit" system under the hood that translates directly into an in-game calendar.

---

## Core Concepts

### Time Units
*   The fundamental unit of in-game time. **1 Time Unit = 1 Second of in-game time.**
*   Every action taken by the player has a `speedCost` measured in time units.
*   Making a standard gem swap is the baseline action and has a `BASE_SWAP_SPEED` (e.g., 20 time units, which translates to 20 seconds).
*   Casting spells or using abilities can have different `speedCost` values, making some actions "faster" or "slower" than others.

### Player Actions & Time Advancement
*   The game's timeline is driven exclusively by player actions. The game state and in-game calendar only advance when the player makes a move (swaps a gem, casts a spell, etc.).
*   After each player action, the `speedCost` of that action is processed, advancing the in-game clock and triggering any relevant time-based events.

---

## Calendar of Aethelgard

*   **Year:** `AY` (Ascendant Year)
*   **Months:** 8 months per year. Each month represents half of a season.
    *   **Vernal Season (Spring):**
        *   Month 1: *Primaver* (Early Spring)
        *   Month 2: *Floreal* (Late Spring)
    *   **Solis Season (Summer):**
        *   Month 3: *Aestus* (Early Summer)
        *   Month 4: *Caldus* (Late Summer)
    *   **Autumnus Season (Autumn):**
        *   Month 5: *Fructus* (Early Autumn)
        *   Month 6: *Folium* (Late Autumn)
    *   **Hiemal Season (Winter):**
        *   Month 7: *Gelu* (Early Winter)
        *   Month 8: *Nix* (Late Winter)
*   **Weeks:** 5 weeks per month.
*   **Days of the Week:** 10 days per week.
    *   Day 1: *Aurorday*
    *   Day 2: *Aniday*
    *   Day 3: *Laborday*
    *   Day 4: *Mercaday*
    *   Day 5: *Culmenday*
    *   Day 6: *Vesperday*
    *   Day 7: *Umbraday*
    *   Day 8: *Magiday*
    *   Day 9: *Spiriday*
    *   Day 10: *Stelladay*
*   **Time of Day:** Standard 24-hour clock (`00:00` to `23:59`).

---

## Time Hierarchy

*   **1 Minute** = 60 Time Units (60 seconds)
*   **1 Hour** = 3,600 Time Units (60 minutes)
*   **1 Day** = 86,400 Time Units (24 hours)
*   **1 Week** = 864,000 Time Units (10 days)
*   **1 Month** = 4,320,000 Time Units (5 weeks / 50 days)
*   **1 Year** = 34,560,000 Time Units (8 months / 400 days)