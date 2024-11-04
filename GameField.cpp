#include "GameField.h"

#include <stdexcept>

GameField::GameField(size_t width, size_t height) : width(width), height(height)
{
    //field.resize(height, std::vector<CellStatus>(width, CellStatus::Empty));
    //ships_links.resize(height, std::vector<Ship*>(width, nullptr));
    //segmentIndexGrid.resize(height, std::vector<int>(width, -1));
    field.resize(height, std::vector<Cell>(width, {nullptr, CellStatus::Empty, -1}));
}

GameField::GameField(const GameField& other)
{
    copy_from(other);
}

GameField& GameField::operator=(const GameField& other)
{
    if (this != &other)
    {
        copy_from(other);
    }
    return *this;
}


void GameField::copy_from(const GameField& other)
{
    width = other.width;
    height = other.height;
    field = other.field;
    //ships_links = other.ships_links;
    //segmentIndexGrid = other.segmentIndexGrid;
}

bool GameField::is_correct_position(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool GameField::can_place_ship(Ship* ship, int x, int y, Orientation orientation) const
{
    int dx = (orientation == Orientation::Horizontal) ? 1 : 0;
    int dy = (orientation == Orientation::Vertical) ? 1 : 0;
    int length = ship->get_length();

    for (int i = 0; i < length; ++i)
    {
        int nx = x + i * dx;
        int ny = y + i * dy;

        if (!is_correct_position(nx, ny) || field[ny][nx].cell != CellStatus::Empty || touch_other_ship(nx, ny))
        {
            return false;
        }
    }
    return true;
}

bool GameField::touch_other_ship(int x, int y) const
{
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            int adj_x = x + dx;
            int adj_y = y + dy;
            if (is_correct_position(adj_x, adj_y) && field[adj_y][adj_x].cell == CellStatus::Ship)
            {
                return true;
            }
        }
    }
    return false;
}

bool GameField::place_ship(Ship* ship, int x, int y, Orientation orientation)
{
    if (!ship)
    {
        throw std::invalid_argument("Ship is null.");
    }
    if (!can_place_ship(ship, x, y, orientation))
    {
        throw ShipPlacementException(x, y);
    }

    int dx = (orientation == Orientation::Horizontal) ? 1 : 0;
    int dy = (orientation == Orientation::Vertical) ? 1 : 0;
    int length = ship->get_length();

    for (int i = 0; i < length; ++i)
    {
        int nx = x + i * dx;
        int ny = y + i * dy;

        field[ny][nx] = {ship, CellStatus::Ship, i};
        //ships_links[ny][nx] = ship;
        //segmentIndexGrid[ny][nx] = i;
    }
    return true;
}

CellStatus GameField::get_cell_status(int x, int y) const
{
    if (!is_correct_position(x, y))
    {
        throw std::out_of_range("Cell position out of range.");
    }
    return field[y][x].cell;
}

void GameField::attack(int x, int y, ShipManager& shipManager)
{
    if (!is_correct_position(x, y))
    {
        throw InvalidCoordinatesException(x, y);
    }

    if (field[y][x].cell == CellStatus::Ship)
    {
        //Ship* ship = ships_links[y][x];
        Ship* ship = field[y][x].ship;
        //int segmentIndex = segmentIndexGrid[y][x];
        int segmentIndex = field[y][x].segment_index_grid;
        ship->decrease_health(segmentIndex, 1);
        shipManager.change_ship_status(ship);
    }
    else if (field[y][x].cell == CellStatus::Empty)
    {
        field[y][x].cell = CellStatus::Miss;
    }
}

char GameField::get_field_point(int x, int y) const
{
    if (!is_correct_position(x, y))
    {
        throw std::out_of_range("Cell position out of range.");
    }

    switch (field[y][x].cell)
    {
        case CellStatus::Unknown:
            return '?';
        case CellStatus::Empty:
            return '.';
        case CellStatus::Miss:
            return 'O';
        case CellStatus::Ship:
        {
            //Ship* ship = ships_links[y][x];
            Ship* ship = field[y][x].ship;
            //int segmentIndex = segmentIndexGrid[y][x];
            int segmentIndex = field[y][x].segment_index_grid;
            SegmentStatus segmentStatus = ship->get_seg_status(segmentIndex);
            switch (segmentStatus)
            {
                case SegmentStatus::Intact:
                    return 'S';
                case SegmentStatus::Damaged:
                    return 'D';
                case SegmentStatus::Destroyed:
                    return 'X';
            }
        }
    }
    return '?';
}
Ship* GameField::get_ship_at(int x, int y) const
{
        if(!is_correct_position(x,y)){
            throw std::out_of_range("Position is out of field bounds.");
        }
        return field[y][x].ship;   
}

int GameField::get_segment_index(int x, int y) const 
{
    if (!is_correct_position(x, y)) {
        throw std::out_of_range("Position is out of field bounds.");
    }
    return field[x][y].segment_index_grid; // Вернет индекс сегмента или -1, если сегмент отсутствует
}

GameField::GameField(GameField&& other)
    : width(other.width), height(other.height), field(std::move(other.field))//, ships_links(std::move(other.ships_links)), segmentIndexGrid(std::move(other.segmentIndexGrid))
{
    other.width = 0;
    other.height = 0;
}

GameField& GameField::operator=(GameField&& other)
{
    if (this != &other)
    {
        width = other.width;
        height = other.height;
        field = std::move(other.field);
        //ships_links = std::move(other.ships_links);
        //segmentIndexGrid = std::move(other.segmentIndexGrid);
        other.width = 0;
        other.height = 0;
    }
    return *this;
}
