#include "FieldRenderer.h"
void FieldRenderer::draw(const GameField& field, bool hideShips) const {
        std::cout<<"  0 1 2 3 4 5 6 7 8 9\n";
        for (int y = 0; y < 10; ++y)
        {
            std::cout<<y<<' ';
            if(hideShips){
                for (int x = 0; x < 10; ++x)
                {
                    if(field.get_field_point(x,y)=='S' || field.get_field_point(x,y)=='.')
                        std::cout<< '~'<<' ';
                    else std::cout << field.get_field_point(x, y) << ' ';
                }
            }
            else{
                for (int x = 0; x < 10; ++x)
                { 
                    std::cout << field.get_field_point(x, y) << ' ';
                }
            }
            std::cout << '\n';
        }
    }