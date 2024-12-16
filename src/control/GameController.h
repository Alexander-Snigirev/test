#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include "Command.h"
#include "InputHandler.h"
#include "../Game.h"
#include <functional>
#include <iostream>

template <typename GameType, typename InputHandler>
class GameController {
private:
    GameType& game;  // Ссылка на объект игры
    InputHandler inputHandler;

public:
    GameController(GameType& gameInstance, const InputHandler& handler);

    void processInput();
};

#endif
