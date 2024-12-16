#ifndef SAVECOMMAND_H
#define SAVECOMMAND_H

#include"../Game.h"
#include "Command.h"

class SaveCommand: public Command{
    private:
        std::string filename;
    public:
        SaveCommand(const std::string &path);
        void execute(Game& game) override;    
};

#endif

