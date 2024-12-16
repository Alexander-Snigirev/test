#include "SaveCommand.h"
SaveCommand::SaveCommand(const std::string &path){
    filename = path;
}

void SaveCommand::execute(Game& game){
    game.save_game(filename);
}

