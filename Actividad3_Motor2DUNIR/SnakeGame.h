#pragma once
#include <UNIR-2D.h>
#include <vector>

using namespace std;
using namespace unir2d;

class BoardActor : public ActorBase {
public:
    BoardActor(int rows, int cols);
    void inicia() override;
    void termina() override;
private:
    void createWallBlock(int row, int col);
    int maxRows;
    int maxCols;
    Color wallColor;
    vector<Rectangulo* > blocks;
};

class Snake : public ActorBase {
public:
    Snake();
    void inicia() override;
    void termina() override;
    void actualiza(double timeSec) override;
    bool advance(Coord applePosition);
    void resetLength(); // NUEVO: Método para reiniciar el tamaño al chocar con la bola roja
    bool collidesWithSelf() const;
    bool collidesWithWalls(int maxRows, int maxCols) const;
    const vector<Coord>& getBody() const;
    Coord getHead() const;
private:
    void updateDrawings();
    vector<Coord> body;
    Coord direction;
    Coord nextDirection;
    vector<Rectangulo*> rectangles;
    Color headColor;
    Color bodyColor;
};

class Apple : public ActorBase {
public:
    Apple();
    void inicia() override;
    void termina() override;
    void reposition(int maxRows, int maxCols, const vector<Coord>& snakeBody);
    Coord getPosition() const;
private:
    Rectangulo* rectangle;
    Coord gridPosition;
    Color appleColor;
};

// NUEVA CLASE: RedBall (La trampa que reduce el tamaño)
class RedBall : public ActorBase {
public:
    RedBall();
    void inicia() override;
    void termina() override;
    void reposition(int maxRows, int maxCols, const vector<Coord>& snakeBody);
    Coord getPosition() const;
private:
    Rectangulo* rectangle;
    Coord gridPosition;
    Color ballColor;
};

class UI : public ActorBase {
public:
    UI();
    void inicia() override;
    void termina() override;
    void setState(int newState, int currentScore = 0);
private:
    void updateUI();
    Texto* titleText;
    Texto* instructionsText;
    Texto* scoreText;
    int state;
    int score;
};

class SnakeGame : public JuegoBase {
public:
    SnakeGame();
protected:
    const wstring tituloVentana() const override;
    void regionVentana(Vector& position, Vector& size) const override;
    void inicia() override;
    void termina() override;
    void preactualiza(double timeSec) override;
    void posactualiza(double timeSec) override;
private:
    void setState(int newState);
    void startGame(double timeSec);
    void hideGameElements();
    void showGameElements();
    int maxRows;
    int maxCols;
    int score;
    int state;
    double lastMoveTime;
    double moveInterval;
    BoardActor* board;
    Snake* snake;
    Apple* apple;
    RedBall* redBall; // NUEVO: Puntero para la bola roja
    UI* ui;
};