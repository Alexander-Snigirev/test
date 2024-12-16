#ifndef ABILITYCOMMAND_H
#define ABILITYCOMMAND_H

#include"../Game.h"
#include "Command.h"

class AbilityCommand : public Command {
public:
    AbilityCommand() {}

    void execute(Game& game) override;
};

#endif