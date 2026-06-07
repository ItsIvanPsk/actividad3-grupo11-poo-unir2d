#pragma once
#include <UNIR-2D.h>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <ctime>

using namespace std;
using namespace unir2d;

// Estructura para guardar una entrada del historial
struct ScoreEntry {
    string playerName;
    int score;
};

// --- BOARD ACTOR ---
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
    vector<Rectangulo*> blocks;
};

// --- SNAKE ---
class Snake : public ActorBase {
public:
    Snake();
    void inicia() override;
    void termina() override;
    void actualiza(double timeSec) override;
    bool advance(Coord applePosition);
    void resetLength();
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

// --- APPLE ---
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

// --- RED BALL (TRAMPA) ---
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

// --- UI ---
class UI : public ActorBase {
public:
    UI();
    void inicia() override;
    void termina() override;
    void setState(int newState, int currentScore = 0);
    void setPlayerName(const string& name);
private:
    void updateUI();
    Texto* titleText;
    Texto* instructionsText;
    Texto* scoreText;
    int state;
    int score;
    string playerName;
};

// --- MAIN GAME ---
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
    void saveScore();
    void exportCSV();
    string generatePlayerName();         // Genera nombre con fecha y hora
    vector<ScoreEntry> loadScores();
    int maxRows;
    int maxCols;
    int score;
    int state;
    // Estados: 0=menu, 1=jugando, 2=game over
    double lastMoveTime;
    double moveInterval;
    BoardActor* board;
    Snake* snake;
    Apple* apple;
    RedBall* redBall;
    UI* ui;
    string currentPlayerName;
};