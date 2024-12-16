#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "FieldRenderer.h"
#include "../Game.h"

class GameRenderer {
private:
    FieldRenderer fieldRenderer;

public:
    GameRenderer(const FieldRenderer& renderer);

    void render(GameField& playerField, GameField& enemyField, AbilityManager& abilities);
};

#endif
