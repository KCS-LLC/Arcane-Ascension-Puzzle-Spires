
# Technical Data Structures

This document provides a technical blueprint for the core data structures of the game. It is intended to serve as a guide for programmers, translating the game's design into a clear, implementable format. The structure is presented in a JSON-like format for clarity.

---

## 1. Heir Data Structure

Defines a single character for a run.

```json
{
  "id": "heir_unique_id_12345",
  "name": "Procedurally Generated Name",
  "portrait_id": "portrait_asset_ref_01",
  "base_stats": {
    "vigor": 10,
    "speed": 12,
    "wit": 8
  },
  "current_stats": {
    "max_hp": 200,
    "current_hp": 200,
    "max_shroud": 50,
    "current_shroud": 50
  },
  "traits": [
    {
      "id": "trait_strong_constitution",
      "name": "Strong Constitution",
      "description": "+10% Max HP.",
      "effect": { "stat": "max_hp", "multiplier": 1.10 }
    },
    {
      "id": "trait_clumsy",
      "name": "Clumsy",
      "description": "Occasionally drop Coins after taking damage.",
      "effect": { "trigger": "on_damage_taken", "action": "drop_coins", "chance": 0.25 }
    }
  ],
  "attunement": {
    "id": "attunement_elementalist",
    "rank": "Quartz", // Enum: Quartz, Carnelian, Sunstone, etc.
    "location": "Right Hand", // Enum: Left Hand, Right Hand, etc.
    "mana_pools": [
      { "type": "Fire", "max": 60, "current": 60 },
      { "type": "Water", "max": 50, "current": 50 },
      { "type": "Earth", "max": 40, "current": 40 },
      { "type": "Air", "max": 45, "current": 45 }
    ],
    "spells": [ "spell_id_fireball", "spell_id_frost_tile" ]
  },
  "equipment": {
    "weapon": "item_id_short_sword",
    "off_hand": null,
    "head": "item_id_leather_cap",
    "body": "item_id_apprentice_robes",
    "legs": null,
    "feet": null,
    "hands": null,
    "ring_1": "item_id_ring_of_blasting",
    "ring_2": null
  },
  "inventory": [ "item_id_health_potion", "item_id_escape_bell" ]
}
```

---

## 2. Monster Data Structure

Defines a single monster for a combat encounter.

```json
{
  "id": "monster_id_sunstone_sentinel",
  "name": "Sunstone Sentinel",
  "rank": "Sunstone", // Enum: Quartz, Carnelian, Sunstone, etc.
  "stats": {
    "max_hp": 800,
    "current_hp": 800,
    "max_shroud": 400,
    "current_shroud": 400,
    "speed": 60,
    "vigor": 40,
    "wit": 35 // Is above the Wit Threshold
  },
  "mana_affinities": [ "Light", "Enhancement" ],
  "resistances": [
    { "type": "Light", "multiplier": 0.5 } // Takes 50% damage from Light
  ],
  "vulnerabilities": [
    { "type": "Umbral", "multiplier": 1.5 } // Takes 150% damage from Umbral
  ],
  "ability_list": [ "ability_id_reflective_shroud", "ability_id_purifying_beam", "action_id_make_match" ]
}
```

---

## 3. Item Data Structure

Defines a single piece of equipment or a consumable.

```json
{
  "id": "item_id_ring_of_blasting",
  "name": "Ring of Blasting",
  "type": "Equipment", // Enum: Equipment, Consumable, Treasure
  "rank": "Carnelian",
  "slot": "Ring", // Enum: Weapon, Head, Ring, etc.
  "affixes": [
    { "stat": "wit", "value": 5 }
  ],
  "special_property": {
    "id": "prop_unlock_bomb_creation",
    "description": "Unlocks the ability to create Bomb gems from L-shape and T-shape matches."
  }
}
```

---

## 4. Ability & Spell Data Structure

Defines a single spell or ability.

```json
{
  "id": "spell_id_create_burning_tile",
  "name": "Create Burning Tile",
  "attunement": "Elementalist",
  "rank_unlocked": "Quartz",
  "cost": {
    "mana_type": "Fire",
    "mana_cost": 20,
    "speed_cost": 40
  },
  "target": "Board", // Enum: Self, Single Enemy, All Enemies, Board
  "effect": {
    "id": "effect_create_static_tile",
    "tile_id": "tile_burning",
    "duration": 3,
    "description": "Creates a Burning Tile that deals Fire damage to the enemy each turn."
  },
  "scaling": [
    {
      "rank": "Sunstone",
      "new_description": "The Burning Tile now also reduces enemy Fire resistance.",
      "effect_modification": { "id": "effect_add_debuff" }
    }
  ]
}
```
