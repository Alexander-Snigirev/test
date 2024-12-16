#ifndef ATTACKCOMMAND_H
#define ATTACKCOMMAND_H

#include"../Game.h"
#include "Command.h"

class AttackCommand : public Command {
private:
    int x, y;

public:
    AttackCommand(int xCoord, int yCoord) : x(xCoord), y(yCoord) {}

    void execute(Game& game) override;
};

#endif