#include "LoadCommand.h"
void LoadCommand::execute(Game& game){
    game.load_game(filename);
}