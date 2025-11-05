
#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <string>

// For the prototype, we'll define gem types here.
// In a full game, this would be in its own file.
enum class GemType {
    Skull,
    Fire,
    Water,
    // Add other mana types as needed
};

class Player {
public:
    // Constructor
    Player(int maxHp) : maxHp(maxHp), currentHp(maxHp) {}

    // Public Methods
    void takeDamage(int damage) {
        currentHp -= damage;
        if (currentHp < 0) {
            currentHp = 0;
        }
    }

    void addMana(GemType type, int amount) {
        if (type != GemType::Skull) {
            manaPools[type] += amount;
        }
    }

    // Getters
    int getCurrentHp() const { return currentHp; }
    int getMaxHp() const { return maxHp; }
    int getMana(GemType type) const {
        if (manaPools.count(type)) {
            return manaPools.at(type);
        }
        return 0;
    }

private:
    int maxHp;
    int currentHp;
    std::map<GemType, int> manaPools;
};

#endif // PLAYER_H
