#include <iostream>
#include "SnakeGame.h"

int main() {
	// Create an instance of the game and the motor
    SnakeGame* game = new SnakeGame();
    unir2d::Motor* motor = new unir2d::Motor();

	// Execute the game using the motor
	motor->ejecuta(game); // TODO: Hector This does not work rn, Hector, when u part is done, uncomment this line and test it

	// Delete game and motor
    delete game;
    delete motor;
    return 0;
}
