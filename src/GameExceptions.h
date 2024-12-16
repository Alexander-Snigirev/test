#ifndef GAMEEXCEPTIONS_H
#define GAMEEXCEPTIONS_H

#include <exception>
#include <string>

class GameException : public std::exception {
public:
    explicit GameException(const std::string& message) : message_(message) {}

    virtual const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

class InvalidCoordinatesException : public GameException {
public:
    InvalidCoordinatesException(int x, int y)
        : GameException("Invalid coordinates: (" + std::to_string(x) + ", " + std::to_string(y) + "). Please try again.") {}
};

class ShipPlacementException : public GameException {
public:
    ShipPlacementException(int x, int y)
        : GameException("Cannot place ship at coordinates: (" + std::to_string(x) + ", " + std::to_string(y) + "). Please try again.") {}
};

class UsingAbilityException: public GameException {
    public:
        UsingAbilityException() : GameException("You don't have got accessible abilities now."){}
};
class CommandException: public GameException{
    public:
        CommandException(): GameException("You have already done this action in current turn"){}
};

#endif