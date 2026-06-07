#include <iostream>
#include "SnakeGame.h"

int main() {
	// Create an instance of the game and the motor
    SnakeGame* game = new SnakeGame();
    unir2d::Motor* motor = new unir2d::Motor();

	// Execute the game using the motor
	motor->ejecuta(game);

	// Delete game and motor
    delete game;
    delete motor;
    return 0;
}
