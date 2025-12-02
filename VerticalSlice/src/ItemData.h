#pragma once

#include <string>
#include <vector>
#include <map>

// Enum for item rarity tiers
enum class ItemRarity {
    Quartz,
    Carnelian,
    Sunstone,
    Citrine,
    Emerald,
    Heirloom
};

// Enum for equipment slots, aligned with player.md
enum class EquipmentSlot {
    Weapon,
    OffHand,
    Head,
    Body,
    Legs,
    Feet,
    Hands,
    Ring1,
    Ring2
};

// Enum for core stat types that can be modified
enum class StatType {
    Vigor,
    Speed,
    Wit,
    Health,
    ManaCapacity // Note: Needs to be handled with a specific mana type
};

// Represents a single stat bonus on an item
struct StatModifier {
    StatType stat;
    int value;
    // Optional: for things like "+15 Max Fire Mana"
    std::string subType; 
};

// Represents a passive ability granted by an item
struct PassiveAbility {
    std::string id; // e.g., "TenPercentFireCostReduction"
    float value;
};

// Represents an activatable ability (spell) granted by an item
struct ActivatedAbility {
    std::string spellId; // ID of the spell to be granted
};

// Defines the static, base properties of an item type
struct ItemBase {
    std::string id; // e.g., "iron_sword"
    std::string name;
    std::string description;
    EquipmentSlot slot;
    // Base properties that might not be random
    int baseDamage = 0; 
};

// Represents a specific, unique instance of an item, procedurally generated or loaded from save
class ItemInstance {
public:
    ItemInstance(int instanceId, const ItemBase* base)
        : m_instanceId(instanceId), m_base(base) {}

    int getInstanceId() const { return m_instanceId; }
    const ItemBase* getBase() const { return m_base; }
    const std::vector<StatModifier>& getStatModifiers() const { return m_statModifiers; }
    const std::vector<PassiveAbility>& getPassiveAbilities() const { return m_passiveAbilities; }
    const ActivatedAbility* getActivatedAbility() const { return m_activatedAbility.get(); }

    void addStatModifier(const StatModifier& mod) { m_statModifiers.push_back(mod); }
    void addPassiveAbility(const PassiveAbility& ability) { m_passiveAbilities.push_back(ability); }
    void setActivatedAbility(const ActivatedAbility& ability) { m_activatedAbility = std::make_unique<ActivatedAbility>(ability); }

private:
    int m_instanceId;
    const ItemBase* m_base; // Pointer to the static base data
    ItemRarity m_rarity = ItemRarity::Quartz;

    std::vector<StatModifier> m_statModifiers;
    std::vector<PassiveAbility> m_passiveAbilities;
    std::unique_ptr<ActivatedAbility> m_activatedAbility;
};
