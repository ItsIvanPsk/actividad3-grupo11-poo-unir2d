#include <iostream>
#include "SnakeGame.h"

int main() {
    SnakeGame* game = new SnakeGame();
    unir2d::Motor* motor = new unir2d::Motor();

	motor->ejecuta(game);

    delete game;
    delete motor;
    return 0;
}
