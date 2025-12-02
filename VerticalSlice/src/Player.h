#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "GemSubType.h"
#include "Structs.h"
#include "Judgement.h"
#include "ItemData.h"
#include <memory>

// Forward declarations
class DataManager;

class Player {
public:
    Player(int initialHp, const std::vector<Spell*>& initialSpells);

    void setAttunement(const Attunement& attunement, const DataManager& dataManager);

    int getHp() const;
    int getMaxHp() const;
    int getMana(GemSubType type) const;
    int getMaxMana() const;
    GemSubType getPrimaryManaType() const;
    const std::vector<Spell*>& getSpells() const;
    const std::vector<ActiveEffect>& getActiveEffects() const;
    const std::vector<GemSubType>& getManaTypes() const;
    const std::string& getAttunementId() const;
    int getVigor() const;
    int getSpeed() const;
    int getWit() const;
    float getManaGainMultiplier() const;

    void equipItem(std::unique_ptr<ItemInstance> item);
    void unequipItem(EquipmentSlot slot);
    bool addItemToInventory(std::unique_ptr<ItemInstance> item);
    void useItem(int inventoryIndex);

    void setManaGainMultiplier(float multiplier);

    float getStatModifier(const std::string& modifier) const;

    void takeDamage(int amount);
    void heal(int amount);
    void addScore(int amount);
    void addMana(GemSubType type, int amount);
    void spendMana(GemSubType type, int amount);
    const Spell* castSpell(int spellIndex);

    void addEffect(const ActiveEffect& effect);
    void updateEffects(float speedCost);
private:
    void setStartingStats(int tactical_score, int mana_affinity_score);
    std::string determineAttunement(const JudgementResults& results, const DataManager& dataManager) const;

    static const int MAX_INVENTORY_SLOTS = 10;

    int m_hp;
    int m_maxHp;
    int m_maxMana;
    int m_vigor = 0;
    float m_manaGainMultiplier = 1.0f;
    std::vector<GemSubType> m_availableManaTypes;
    std::map<GemSubType, int> mana;
    std::vector<Spell*> spells; // Changed to vector of pointers
    std::vector<ActiveEffect> m_activeEffects;
    std::map<EquipmentSlot, std::unique_ptr<ItemInstance>> m_equipment;
    std::vector<std::unique_ptr<ItemInstance>> m_inventory;
    std::string m_attunementId;
    int m_score = 0;
};