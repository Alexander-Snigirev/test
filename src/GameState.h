#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <iostream>
#include "GameField.h"
#include "ShipManager.h"
#include "skills/AbilityManager.h"

class GameState {
public:
    int roundNumber;
    GameField pField;
    GameField eField;
    std::vector <char> pAbilities;
    ShipManager pShips;
    ShipManager eShips;

    GameState() = default;
    GameState(const int round, const GameField& playerField, const GameField& enemyField,
            std::vector<char> playerAbilities);
    friend std::ostream& operator<<(std::ostream& out, const GameState& state);
    friend std::istream& operator>>(std::istream& in, GameState& state);

    void save(std::ostream& out, const ShipManager& playerShips, const ShipManager& enemyShips);
    void save(int round, GameField& playerField, GameField& enemyField,
              ShipManager& playerShips, ShipManager& enemyShips,AbilityManager& playerAbilities,std::ostream& out) const;

    void load(int round, GameField& playerField, GameField& enemyField,
              ShipManager& playerShips, ShipManager& enemyShips,
              AbilityManager& playerAbilities, std::istream& in);
          

};

#endif
