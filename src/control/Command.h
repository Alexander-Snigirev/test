#ifndef COMMAND_H
#define COMMAND_H

#include <memory>

class Game;
class Command {
public:
    virtual void execute(Game& game) = 0;
    virtual ~Command() = default;
};

// Другие команды (например, Save, Load) можно добавлять аналогично
#endif
