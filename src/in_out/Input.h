#ifndef INPUT_H
#define INPUT_H

#include "../GameField.h"
#include "../ShipManager.h"
#include "../skills/AbilityManager.h"
#include <iostream>

class Input {
public:
    static void place_ships(GameField& field, ShipManager& shipManager);
    static std::vector<int> get_ship_lengths();    
};

#endif
