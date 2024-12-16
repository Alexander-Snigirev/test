#include "EndCommand.h"

void EndCommand::execute(Game& game){
    game.setEndFlag();
}