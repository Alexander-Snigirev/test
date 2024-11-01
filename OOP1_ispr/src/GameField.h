#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>
#include "Ship.h"
#include "ShipManager.h"

enum class CellStatus
{
    Unknown,
    Empty,
    Ship,
    Miss,
};

class GameField
{
public:
    static const size_t DEFAULT_WIDTH = 10;
    static const size_t DEFAULT_HEIGHT = 10;

    GameField(size_t width = DEFAULT_WIDTH, size_t height = DEFAULT_HEIGHT);
    GameField(const GameField& other);
    GameField& operator=(const GameField& other);

    bool place_ship(Ship* ship, int x, int y, Orientation orientation);
    CellStatus get_cell_status(int x, int y) const;
    void attack(int x, int y, ShipManager& shipManager);
    char get_field_point(int x, int y) const;
    GameField(GameField&& other);
    GameField& operator=(GameField&& other);

private:
    int width;
    int height;
    std::vector<std::vector<CellStatus>> field;
    std::vector<std::vector<Ship*>> ships_links;
    std::vector<std::vector<int>> segmentIndexGrid;

    bool is_correct_position(int x, int y) const;
    bool can_place_ship(Ship* ship, int x, int y, Orientation orientation) const;
    bool touch_other_ship(int x, int y) const;
    void copy_from(const GameField& other);
};

#endif
