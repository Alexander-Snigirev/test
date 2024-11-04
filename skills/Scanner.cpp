#include "Scanner.h"

void Scanner::apply(GameField& field, ShipManager& shipManager, int x, int y)
{
    if(!field.is_correct_position(x,y)){
        throw std::out_of_range("Cell position out of range.");
    }
    int ny = y < field.DEFAULT_HEIGHT-1;
    int nx = x < field.DEFAULT_WIDTH-1;
    char square_status[4];
    square_status[0] = field.get_field_point(x,y);
    square_status[1] = field.get_field_point(x+nx,y);
    square_status[2] = field.get_field_point(x,y+ny);
    square_status[3] = field.get_field_point(x+nx,y+ny);
    
    for(char st: square_status){
        if(st=='D' || st=='S' ){
            std::cout<<"На указанном участке есть корабль.\n";
            return;
        }
    }
    std::cout<<"На указанном участке нет кораблей.\n";
}

