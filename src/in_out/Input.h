#ifndef INPUT_H
#define INPUT_H

#include "../GameField.h"
#include "../ShipManager.h"
#include "../skills/AbilityManager.h"
#include <iostream>

class Input {
public:
    static void place_ships(GameField& field, ShipManager& shipManager);
    static std::pair<int, int> get_attack_coordinates();
    static std::vector<int> get_ship_lengths();
    static void use_ability(AbilityManager& abilityManager, GameField& field, ShipManager& shipManager);
};

#endif
