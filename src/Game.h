#ifndef GAME_H
#define GAME_H

#include "GameField.h"
#include "ShipManager.h"
#include "skills/AbilityManager.h"
#include <string>
#include "in_out/Input.h"

class Game {
private:
    GameField playerField;
    GameField enemyField;
    ShipManager playerShips;
    ShipManager enemyShips;
    AbilityManager playerAbilities;
    bool isPlayerTurn;
    int roundNumber;

    void initialize_round();
    void execute_player_turn();
    void execute_enemy_turn();
    bool check_victory() const;
    bool check_defeat() const;

public:
    Game();
    void start_new_game();
    void start_round();
    void save_game(const std::string& filename) const;
    void load_game(const std::string& filename);
};

#endif
