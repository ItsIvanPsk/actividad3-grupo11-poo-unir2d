#include "SnakeGame.h"
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;
using namespace unir2d;

BoardActor::BoardActor (int rows, int cols) {
    maxRows = rows;
    maxCols = cols;
    wallColor = Color { 100, 100, 100 };
}

void BoardActor::inicia () {
    for (int r = 0 ; r < maxRows ; r++) {
        for (int c = 0 ; c < maxCols ; c++) {
            if (r == 0 || r == maxRows - 1 || c == 0 || c == maxCols - 1) {
                createWallBlock (r, c);
            }
        }
    }
}

void BoardActor::termina () {
    extraeDibujos ();
    for (int i = 0 ; i < blocks.size() ; i++) {
        delete blocks [i];
    }
    blocks.clear ();
}

void BoardActor::createWallBlock (int row, int col) {
    Rectangulo * rect = new Rectangulo (18.0f, 18.0f);
    rect->ponColor (wallColor);
    rect->ponPosicion (Vector { col * 20.0f + 1.0f, row * 20.0f + 1.0f });
    blocks.push_back (rect);
    agregaDibujo (rect);
}

Snake::Snake()
{
    headColor = Color{0, 200, 0};
    bodyColor = Color{0, 150, 0};
}

void Snake::inicia()
{
    direction = Coord{0, 1};
    nextDirection = direction;

    body.clear();
    body.push_back(Coord{15, 10});
    body.push_back(Coord{15, 9});
    body.push_back(Coord{15, 8});

    for(int i=0; i<rectangles.size(); i++)
    {
        delete rectangles[i];
    }
    rectangles.clear();
    extraeDibujos();

    for(int i=0; i<body.size(); i++)
    {
        Rectangulo *rect = new Rectangulo(18.0f, 18.0f);
        if (i == 0)
        {
            rect->ponColor(headColor);
        }
        else
        {
            rect->ponColor(bodyColor);
        }
        rectangles.push_back(rect);
        agregaDibujo(rect);
    }

    updateDrawings();
}

void Snake::termina()
{
    extraeDibujos();
    for(int i=0; i<rectangles.size(); i++)
    {
        delete rectangles[i];
    }
    rectangles.clear();
}

void Snake::actualiza(double timeSec)
{
    if (Teclado::pulsando(Tecla::arriba) || Teclado::pulsando(Tecla::W))
    {
        if (direction.fila() == 0)
        {
            nextDirection = Coord{-1, 0};
        }
    }
    if (Teclado::pulsando(Tecla::abajo) || Teclado::pulsando(Tecla::S))
    {
        if (direction.fila() == 0)
        {
            nextDirection = Coord{1, 0};
        }
    }

    if (Teclado::pulsando(Tecla::izquierda) || Teclado::pulsando(Tecla::A))
    {
        if (direction.coln() == 0)
        {
            nextDirection = Coord{0, -1};
        }
    }

    if (Teclado::pulsando(Tecla::derecha) || Teclado::pulsando(Tecla::D))
    {
        if (direction.coln() == 0)
        {
            nextDirection = Coord{0, 1};
        }
    }
}

bool Snake::advance(Coord applePosition)
{
    direction = nextDirection;
    Coord newHead = body[0] + direction;
    body.insert(body.begin(), newHead);
        bool grows = false;
    if (newHead.fila() == applePosition.fila() && newHead.coln() == applePosition.coln())
    {
        grows = true;
    }

    if (!grows)
    {
        body.pop_back();
    }
    else
    {
        Rectangulo *rect = new Rectangulo(18.0f, 18.0f);
        rect->ponColor(bodyColor);
        rectangles.push_back(rect);
        agregaDibujo(rect);
    }

    updateDrawings();
    return grows;
}

bool Snake::collidesWithSelf() const
{
    //Hector as we comented on the call please do the collider system with the snake, remember that this collider must be in paralelo with the collidesWithWalls

}

bool Snake::collidesWithWalls(int maxRows, int maxCols) const
{
	//Hector as we comented on the call please do the collider system, remember that the walls are on the borders of the board
}

const vector<Coord>& Snake::getBody() const
{
    return body;
}

Coord Snake::getHead() const
{
    if (body.size() == 0)
    {
        return Coord{0, 0};
    }
    return body[0];
}

void Snake::updateDrawings()
{
    for(int i=0; i<body.size(); i++)
    {
        float x = body[i].coln() * 20.0f + 1.0f;
        float y = body[i].fila() * 20.0f + 1.0f;

        rectangles[i]->ponPosicion(Vector{x, y});

        if (i == 0)
        {
            rectangles[i]->ponColor(headColor);
        }
        else
        {
            rectangles[i]->ponColor(bodyColor);
        }
    }
}

Apple::Apple() {
   rectangle = nullptr;
   gridPosition = Coord{0, 0};
   appleColor = Color{255, 0, 0};
}

void Apple::inicia() {

   std::srand((unsigned int) std::time(nullptr));
   if (rectangle == nullptr) {
      rectangle = new Rectangulo(18.0f, 18.0f);
      rectangle->ponColor(appleColor);
      agregaDibujo(rectangle);
   }

}

void Apple::termina() {

   if (rectangle != nullptr) {
      extraeDibujos();
      delete rectangle;
      rectangle = nullptr;
   }

}

void Apple::reposition(int maxRows, int maxCols, const vector<Coord>& snakeBody) {
   // Hector, as be commented in the call, its mandatory to generate a method a spawn a new apple on the board.
}

Coord Apple::getPosition() const {
   return gridPosition;
}

UI::UI () {
    titleText = nullptr;
    instructionsText = nullptr;
    scoreText = nullptr;
    state = 0;
    score = 0;
}

void UI::inicia () {
    if (nullptr == titleText) {
        titleText = new Texto ("DejaVuSans-Bold");
        titleText->ponTamano (48);
        titleText->ponColor (Color::Blanco);
        agregaDibujo (titleText);
    }

    if (nullptr == instructionsText) {
        instructionsText = new Texto ("DejaVuSans");
        instructionsText->ponTamano (22);
        instructionsText->ponColor (Color::Plata);
        agregaDibujo (instructionsText);
    }

    if (nullptr == scoreText) {
        scoreText = new Texto ("DejaVuSans");
        scoreText->ponTamano (20);
        scoreText->ponColor (Color::Blanco);
        agregaDibujo (scoreText);
    }

    updateUI ();
}

void UI::termina () {
    extraeDibujos ();
    if (nullptr != titleText) {
        delete titleText;
        titleText = nullptr;
    }
    if (nullptr != instructionsText) {
        delete instructionsText;
        instructionsText = nullptr;
    }
    if (nullptr != scoreText) {
        delete scoreText;
        scoreText = nullptr;
    }
}

void UI::setState (int newState, int currentScore) {
    state = newState;
    score = currentScore;
    updateUI ();
}

void UI::updateUI () {
    if (nullptr == titleText || nullptr == instructionsText || nullptr == scoreText) {
        return;
    }

    if (0 == state) { // State 0 is for load main menu UI
        titleText->ponVisible (true);
        titleText->ponCadena ("SNAKE GAME");
        titleText->ponColor (Color::Verde);
        titleText->ponPosicion (Vector { 400.0f - titleText->anchura () / 2.0f, 150.0f });
        instructionsText->ponVisible (true);
        instructionsText->ponCadena ("Press ENTER to Play\nControls: Arrows or WASD\nPress ESC to exit");
        instructionsText->ponPosicion (Vector { 400.0f - instructionsText->anchura () / 2.0f, 280.0f });
        scoreText->ponVisible (false);
    } else if (1 == state) { // State 1 is for load game UI
        titleText->ponVisible (false);
        instructionsText->ponVisible (false);
        scoreText->ponVisible (true);
        scoreText->ponCadena ("Score: " + to_string (score));
        scoreText->ponPosicion (Vector { 15.0f, 15.0f });
    } else if (2 == state) {// State 2 is for load game over UI
        titleText->ponVisible (true);
        titleText->ponCadena ("GAME OVER");
        titleText->ponColor (Color::Rojo);
        titleText->ponPosicion (Vector { 400.0f - titleText->anchura () / 2.0f, 150.0f });
        instructionsText->ponVisible (true);
        instructionsText->ponCadena ("Final Score: " + to_string (score) + "\n\nPress ENTER to restart\nPress ESC to exit");
        instructionsText->ponPosicion (Vector { 400.0f - instructionsText->anchura () / 2.0f, 280.0f });
        scoreText->ponVisible (false);
    }
}

SnakeGame::SnakeGame() { 
    maxRows = 30;
    maxCols = 40;
    board = nullptr;
    snake = nullptr;
    apple = nullptr;
    ui = nullptr;
    score = 0;
    state = 0;
    lastMoveTime = 0.0;
    moveInterval = 0.12;
}

const wstring SnakeGame::tituloVentana () const {
    return L"Snake Game";
}

// Window setup
void SnakeGame::regionVentana (Vector & position, Vector & size) const { 
    position = Vector { 100.0f, 100.0f };
    size     = Vector { 800.0f, 600.0f };
}

// Loaders
void SnakeGame::inicia () {
    board = new BoardActor (maxRows, maxCols);
    agregaActor (board);
    snake = new Snake ();
    agregaActor (snake);
    apple = new Apple ();
    agregaActor (apple);
    ui = new UI ();
    agregaActor (ui);
    setState (0);
}

// Unloaders
void SnakeGame::termina () {
    extraeActores ();

    if (nullptr != board) {
        delete board;
        board = nullptr;
    }

    if (nullptr != snake) {
        delete snake;
        snake = nullptr;
    }

    if (nullptr != apple) {
        delete apple;
        apple = nullptr;
    }

    if (nullptr != ui) {
        delete ui;
        ui = nullptr;
    }
}

void SnakeGame::preactualiza (double timeSec) {
    // Francisco, as comented in the call, here the game loop 
}

void SnakeGame::posactualiza (double timeSec) {
    // Don't touch here please, is not necesary to do nothing here @Hector @Francisco
}

// Francisco, maybe is necesary to change if the "preactualiza" method is not enough, but almost i build it as i think is optimal
void SnakeGame::setState (int newState) {
    if (0 == newState) {
        ui->setState (0);
    }
    if (1 == newState) {
        ui->setState (1, score);
    }
    if (2 == newState) {
        ui->setState (2, score);
    }
}

void SnakeGame::startGame (double timeSec) {
    score = 0;
    moveInterval = 0.12;
    lastMoveTime = timeSec;

    snake->inicia ();
    apple->reposition (maxRows, maxCols, snake->getBody ());
    
    setState (1);
}

void SnakeGame::hideGameElements () {
    if (nullptr != snake) {
        vector<Dibujable *> drawings = snake->dibujos ();
        for (int i = 0 ; i < drawings.size() ; i++) {
            drawings [i]->ponVisible (false);
        }
    }

    if (nullptr != apple) {
        vector<Dibujable *> drawings = apple->dibujos ();
        for (int i = 0 ; i < drawings.size() ; i++) {
            drawings [i]->ponVisible (false);
        }
    }
}

void SnakeGame::showGameElements () {

    if (nullptr != snake) {
        vector<Dibujable *> drawings = snake->dibujos ();
        for (int i = 0 ; i < drawings.size() ; i++) {
            drawings [i]->ponVisible (true);
        }
    }

    if (nullptr != apple) {
        vector<Dibujable *> drawings = apple->dibujos ();
        for (int i = 0 ; i < drawings.size() ; i++) {
            drawings [i]->ponVisible (true);
        }
    }
}
