#ifndef LOADCOMMAND_H
#define LOADCOMMAND_H

#include"../Game.h"
#include "Command.h"

class LoadCommand: public Command{
    private:
        std::string filename;
    public:
        LoadCommand(const std::string& path): filename(path){}
        void execute(Game& game) override;    
};

#endif