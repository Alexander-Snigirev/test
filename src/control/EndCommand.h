#ifndef ENDCOMMAND_H
#define ENDCOMMAND_H

#include"../Game.h"
#include "Command.h"

class EndCommand: public Command{
    public:
    EndCommand(){};
    void execute(Game& game) override;
};


#endif