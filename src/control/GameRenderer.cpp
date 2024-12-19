#include "GameRenderer.h"

GameRenderer::GameRenderer(const FieldRenderer& renderer) : fieldRenderer(renderer) {}

void GameRenderer::render(GameField& playerField, GameField& enemyField, AbilityManager& abilities) {
    std::cout << "Your Field:" << '\n';
    fieldRenderer.draw(playerField,false);

    std::cout << "Enemy Field:" << '\n';
    fieldRenderer.draw(enemyField, true);  // Скрываем корабли противника

    std::cout << "Your abilities: " << abilities.get_count() << std::endl;
}

void GameRenderer::print_msg(const char* msg){
    std::cout<<msg<<std::endl;
}