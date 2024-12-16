#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

#include "GameField.h"
#include "ShipManager.h"
#include "GameState.h"
#include "skills/AbilityManager.h"
#include <string>
#include "in_out/Input.h"
#include <ctime>
#include "control/GameController.h"
#include "control/GameRenderer.h"
#include "control/InputHandler.h"
#include "control/FieldRenderer.h"


class Game {
private:
    GameField playerField;
    GameField enemyField;
    ShipManager playerShips;
    ShipManager enemyShips;
    AbilityManager playerAbilities;
    bool isPlayerTurn;
    int roundNumber;
    int acounter;
    int ucounter;
    int endflag;
    void initialize_round();
    void execute_enemy_turn();
    bool check_victory() const;
    bool check_defeat() const;
    void loading();

public:
    Game();
    void execute_player_turn(int x, int y);
    void start_new_game();
    void start_round();
    void save_game(const std::string& filename);
    void load_game(const std::string& filename);
    void UseCommand();
    void UseAbility();
    void setEndFlag();
};

#endif
