#include "GameController.h"

template <typename GameType, typename InputHandler>
void GameController<GameType, InputHandler>::processInput() {
    auto command = inputHandler.getCommand();
    if (!command) {
        std::cout << "Invalid command!" << std::endl;
        return;
    }
    try{
        command->execute(game);
    }
    catch(const InvalidCoordinatesException& e){
        std::cout<<e.what()<<std::endl;
        processInput();
    }
    catch (const UsingAbilityException&) {
        std::cout << "Warning! No abilities available" << std::endl;
    }
    catch(const CommandException& e){
        std::cout<<e.what()<<std::endl;
    }
    catch(std::runtime_error& e){
        std::cout<<e.what()<<std::endl;
        processInput();
    }
}

template <typename GameType, typename InputHandler>
GameController<GameType, InputHandler>::GameController(GameType& gameInstance, const InputHandler& handler): game(gameInstance), inputHandler(handler) {}

template class GameController<Game, InputHandler>;