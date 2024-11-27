#include "bombardment.h"
void Bombardment::apply(GameField& field, ShipManager& shipManager, int x, int y)
{
    srand(time(0));
    int fx,fy;
    while(1){
        fx = rand()%10;
        fy = rand()%10;
        if(field.get_field_point(fx,fy)=='D' || field.get_field_point(fx,fy)=='S'){
            field.attack(fx,fy, shipManager,DAMAGE);
            break;
        }
    }
}