#ifndef FIELD_RENDERER_H
#define FIELD_RENDERER_H

#include <iostream>
#include "../GameField.h"

class FieldRenderer {
public:
    void draw(const GameField& field, bool hideShips) const;
};

#endif
