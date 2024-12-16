#include "AttackCommand.h"


void AttackCommand::execute(Game& game){
        game.execute_player_turn(x,y);
}