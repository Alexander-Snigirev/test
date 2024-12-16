#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <fstream>
#include <map>
#include <functional>
#include <iostream>
#include <sstream>
#include <memory>
#include "Command.h"
#include "AttackCommand.h"
#include "AbilityCommand.h"
#include "EndCommand.h"
#include "SaveCommand.h"
#include "LoadCommand.h"

class InputHandler {
private:
    std::map<char, std::function<std::unique_ptr<Command>()>> commandMap;
    bool isCorrectCommands(std::vector<int> hashes);
public:
    InputHandler(const std::string& filename);

    void loadCommandsFromFile(const std::string& filename);

    void setDefaultCommands();

    std::unique_ptr<Command> getCommand();
};
#endif