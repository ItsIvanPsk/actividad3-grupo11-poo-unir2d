#include "SnakeGame.h"
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;
using namespace unir2d;

// --- BOARD ACTOR ---
BoardActor::BoardActor(int rows, int cols) {
    maxRows = rows;
    maxCols = cols;
    wallColor = Color{ 100, 100, 100 };
}

void BoardActor::inicia() {
    for (int r = 0; r < maxRows; r++) {
        for (int c = 0; c < maxCols; c++) {
            if (r == 0 || r == maxRows - 1 || c == 0 || c == maxCols - 1) {
                createWallBlock(r, c);
            }
        }
    }
}

void BoardActor::termina() {
    extraeDibujos();
    for (int i = 0; i < blocks.size(); i++) {
        delete blocks[i];
    }
    blocks.clear();
}

void BoardActor::createWallBlock(int row, int col) {
    Rectangulo* rect = new Rectangulo(18.0f, 18.0f);
    rect->ponColor(wallColor);
    rect->ponPosicion(Vector{ col * 20.0f + 1.0f, row * 20.0f + 1.0f });
    blocks.push_back(rect);
    agregaDibujo(rect);
}


// --- SNAKE ---
Snake::Snake()
{
    headColor = Color{ 0, 200, 0 };
    bodyColor = Color{ 0, 150, 0 };
}

void Snake::inicia()
{
    direction = Coord{ 0, 1 };
    nextDirection = direction;

    body.clear();
    body.push_back(Coord{ 15, 10 });
    body.push_back(Coord{ 15, 9 });
    body.push_back(Coord{ 15, 8 });

    for (int i = 0; i < rectangles.size(); i++)
    {
        delete rectangles[i];
    }
    rectangles.clear();
    extraeDibujos();

    for (int i = 0; i < body.size(); i++)
    {
        Rectangulo* rect = new Rectangulo(18.0f, 18.0f);
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
    for (int i = 0; i < rectangles.size(); i++)
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
            nextDirection = Coord{ -1, 0 };
        }
    }
    if (Teclado::pulsando(Tecla::abajo) || Teclado::pulsando(Tecla::S))
    {
        if (direction.fila() == 0)
        {
            nextDirection = Coord{ 1, 0 };
        }
    }

    if (Teclado::pulsando(Tecla::izquierda) || Teclado::pulsando(Tecla::A))
    {
        if (direction.coln() == 0)
        {
            nextDirection = Coord{ 0, -1 };
        }
    }

    if (Teclado::pulsando(Tecla::derecha) || Teclado::pulsando(Tecla::D))
    {
        if (direction.coln() == 0)
        {
            nextDirection = Coord{ 0, 1 };
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
        Rectangulo* rect = new Rectangulo(18.0f, 18.0f);
        rect->ponColor(bodyColor);
        rectangles.push_back(rect);
        agregaDibujo(rect);
    }

    updateDrawings();
    return grows;
}

void Snake::resetLength() {
    // Si la serpiente ya tiene su tamaño mínimo (3), no hacemos nada
    if (body.size() <= 3) {
        return;
    }

    extraeDibujos();

    // Borramos la memoria de los rectángulos que sobran (de la cola en adelante)
    for (size_t i = 3; i < rectangles.size(); i++) {
        delete rectangles[i];
    }

    // Redimensionamos los vectores para quedarnos solo con la cabeza y 2 bloques
    body.resize(3);
    rectangles.resize(3);

    // Volvemos a añadir los 3 dibujos restantes a la escena
    for (size_t i = 0; i < rectangles.size(); i++) {
        agregaDibujo(rectangles[i]);
    }

    updateDrawings();
}

bool Snake::collidesWithSelf() const
{
    if (body.size() < 2) {
        return false;
    }

    Coord head = body[0];
    for (size_t i = 1; i < body.size(); ++i) {
        if (head == body[i]) {
            return true;
        }
    }
    return false;
}

bool Snake::collidesWithWalls(int maxRows, int maxCols) const
{
    Coord head = getHead();
    if (head.fila() == 0 || head.fila() == maxRows - 1 ||
        head.coln() == 0 || head.coln() == maxCols - 1) {
        return true;
    }
    return false;
}

const vector<Coord>& Snake::getBody() const
{
    return body;
}

Coord Snake::getHead() const
{
    if (body.size() == 0)
    {
        return Coord{ 0, 0 };
    }
    return body[0];
}

void Snake::updateDrawings()
{
    for (int i = 0; i < body.size(); i++)
    {
        float x = body[i].coln() * 20.0f + 1.0f;
        float y = body[i].fila() * 20.0f + 1.0f;

        rectangles[i]->ponPosicion(Vector{ x, y });

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


// --- APPLE ---
Apple::Apple() {
    rectangle = nullptr;
    gridPosition = Coord{ 0, 0 };
    appleColor = Color{ 255, 255, 0 }; // Cambiado a amarillo
}

void Apple::inicia() {
    std::srand((unsigned int)std::time(nullptr));
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
    bool validPos = false;
    while (!validPos) {
        int r = 1 + std::rand() % (maxRows - 2);
        int c = 1 + std::rand() % (maxCols - 2);
        gridPosition = Coord{ r, c };

        validPos = true;
        for (const auto& part : snakeBody) {
            if (part.fila() == r && part.coln() == c) {
                validPos = false;
                break;
            }
        }
    }

    if (rectangle != nullptr) {
        float x = gridPosition.coln() * 20.0f + 1.0f;
        float y = gridPosition.fila() * 20.0f + 1.0f;
        rectangle->ponPosicion(Vector{ x, y });
    }
}

Coord Apple::getPosition() const {
    return gridPosition;
}


// --- RED BALL (TRAMPA) ---
RedBall::RedBall() {
    rectangle = nullptr;
    gridPosition = Coord{ 0, 0 };
    ballColor = Color{ 255, 0, 0 }; // Rojo
}

void RedBall::inicia() {
    if (rectangle == nullptr) {
        rectangle = new Rectangulo(18.0f, 18.0f);
        rectangle->ponColor(ballColor);
        agregaDibujo(rectangle);
    }
}

void RedBall::termina() {
    if (rectangle != nullptr) {
        extraeDibujos();
        delete rectangle;
        rectangle = nullptr;
    }
}

void RedBall::reposition(int maxRows, int maxCols, const vector<Coord>& snakeBody) {
    bool validPos = false;
    while (!validPos) {
        int r = 1 + std::rand() % (maxRows - 2);
        int c = 1 + std::rand() % (maxCols - 2);
        gridPosition = Coord{ r, c };

        validPos = true;
        for (const auto& part : snakeBody) {
            if (part.fila() == r && part.coln() == c) {
                validPos = false;
                break;
            }
        }
    }

    if (rectangle != nullptr) {
        float x = gridPosition.coln() * 20.0f + 1.0f;
        float y = gridPosition.fila() * 20.0f + 1.0f;
        rectangle->ponPosicion(Vector{ x, y });
    }
}

Coord RedBall::getPosition() const {
    return gridPosition;
}


// --- UI ---
UI::UI() {
    titleText = nullptr;
    instructionsText = nullptr;
    scoreText = nullptr;
    state = 0;
    score = 0;
}

void UI::inicia() {
    if (nullptr == titleText) {
        titleText = new Texto("DejaVuSans-Bold");
        titleText->ponTamano(48);
        titleText->ponColor(Color::Blanco);
        agregaDibujo(titleText);
    }

    if (nullptr == instructionsText) {
        instructionsText = new Texto("DejaVuSans");
        instructionsText->ponTamano(22);
        instructionsText->ponColor(Color::Plata);
        agregaDibujo(instructionsText);
    }

    if (nullptr == scoreText) {
        scoreText = new Texto("DejaVuSans");
        scoreText->ponTamano(20);
        scoreText->ponColor(Color::Blanco);
        agregaDibujo(scoreText);
    }

    updateUI();
}

void UI::termina() {
    extraeDibujos();
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

void UI::setState(int newState, int currentScore) {
    state = newState;
    score = currentScore;
    updateUI();
}

void UI::updateUI() {
    if (nullptr == titleText || nullptr == instructionsText || nullptr == scoreText) {
        return;
    }

    if (0 == state) {
        titleText->ponVisible(true);
        titleText->ponCadena("SNAKE GAME");
        titleText->ponColor(Color::Verde);
        titleText->ponPosicion(Vector{ 400.0f - titleText->anchura() / 2.0f, 150.0f });
        instructionsText->ponVisible(true);
        instructionsText->ponCadena("Press ENTER to Play\nControls: Arrows or WASD\nPress ESC to exit");
        instructionsText->ponPosicion(Vector{ 400.0f - instructionsText->anchura() / 2.0f, 280.0f });
        scoreText->ponVisible(false);
    }
    else if (1 == state) {
        titleText->ponVisible(false);
        instructionsText->ponVisible(false);
        scoreText->ponVisible(true);
        scoreText->ponCadena("Score: " + to_string(score));
        scoreText->ponPosicion(Vector{ 15.0f, 15.0f });
    }
    else if (2 == state) {
        titleText->ponVisible(true);
        titleText->ponCadena("GAME OVER");
        titleText->ponColor(Color::Rojo);
        titleText->ponPosicion(Vector{ 400.0f - titleText->anchura() / 2.0f, 150.0f });
        instructionsText->ponVisible(true);
        instructionsText->ponCadena("Final Score: " + to_string(score) + "\n\nPress ENTER to restart\nPress ESC to exit");
        instructionsText->ponPosicion(Vector{ 400.0f - instructionsText->anchura() / 2.0f, 280.0f });
        scoreText->ponVisible(false);
    }
}


// --- MAIN GAME ---
SnakeGame::SnakeGame() {
    maxRows = 30;
    maxCols = 40;
    board = nullptr;
    snake = nullptr;
    apple = nullptr;
    redBall = nullptr; // Inicializamos la bola roja
    ui = nullptr;
    score = 0;
    state = 0;
    lastMoveTime = 0.0;
    moveInterval = 0.12;
}

const wstring SnakeGame::tituloVentana() const {
    return L"Snake Game";
}

void SnakeGame::regionVentana(Vector& position, Vector& size) const {
    position = Vector{ 100.0f, 100.0f };
    size = Vector{ 800.0f, 600.0f };
}

void SnakeGame::inicia() {
    board = new BoardActor(maxRows, maxCols);
    agregaActor(board);

    snake = new Snake();
    agregaActor(snake);

    apple = new Apple();
    agregaActor(apple);

    redBall = new RedBall(); // Añadimos el nuevo actor
    agregaActor(redBall);

    ui = new UI();
    agregaActor(ui);

    setState(0);
}

void SnakeGame::termina() {
    extraeActores();

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
    if (nullptr != redBall) {
        delete redBall;
        redBall = nullptr;
    }
    if (nullptr != ui) {
        delete ui;
        ui = nullptr;
    }
}

void SnakeGame::preactualiza(double timeSec) {
    if (0 == state) {
        hideGameElements();

        if (Teclado::pulsando(Tecla::entrar)) {
            Teclado::consume(Tecla::entrar);
            startGame(timeSec);
        }
        if (Teclado::pulsando(Tecla::escape)) {
            ponEjecucion(EjecucionJuego::cancelado);
        }
    }
    else if (1 == state) {
        showGameElements();

        if (Teclado::pulsando(Tecla::escape)) {
            Teclado::consume(Tecla::escape);
            setState(0);
        }

        if (timeSec - lastMoveTime >= moveInterval) {
            lastMoveTime = timeSec;

            bool ate = snake->advance(apple->getPosition());

            // Colisión con la manzana normal
            if (ate == true) {
                score = score + 10;
                ui->setState(1, score);
                apple->reposition(maxRows, maxCols, snake->getBody());

                double speedBoost = (score / 10) * 0.003;
                moveInterval = 0.12 - speedBoost;
                if (moveInterval < 0.05) {
                    moveInterval = 0.05;
                }
            }

            // Colisión con la bolita roja (trampa)
            Coord head = snake->getHead();
            Coord redBallPos = redBall->getPosition();
            if (head.fila() == redBallPos.fila() && head.coln() == redBallPos.coln()) {
                snake->resetLength(); // Reinicia la serpiente
                redBall->reposition(maxRows, maxCols, snake->getBody()); // Cambia de sitio la bola roja
            }

            // Colisión con paredes o consigo misma
            if (snake->collidesWithWalls(maxRows, maxCols) == true || snake->collidesWithSelf() == true) {
                setState(2);
            }
        }
    }
    else if (2 == state) {
        if (Teclado::pulsando(Tecla::entrar)) {
            Teclado::consume(Tecla::entrar);
            startGame(timeSec);
        }
        if (Teclado::pulsando(Tecla::escape)) {
            Teclado::consume(Tecla::escape);
            setState(0);
        }
    }
}

void SnakeGame::posactualiza(double timeSec) {

}

void SnakeGame::setState(int newState) {
    if (0 == newState) {
        ui->setState(0);
    }
    if (1 == newState) {
        ui->setState(1, score);
    }
    if (2 == newState) {
        ui->setState(2, score);
    }
}

void SnakeGame::startGame(double timeSec) {
    score = 0;
    moveInterval = 0.12;
    lastMoveTime = timeSec;

    snake->inicia();
    apple->reposition(maxRows, maxCols, snake->getBody());
    redBall->reposition(maxRows, maxCols, snake->getBody()); // Posicionamos la trampa

    setState(1);
}

void SnakeGame::hideGameElements() {
    if (nullptr != snake) {
        vector<Dibujable*> drawings = snake->dibujos();
        for (int i = 0; i < drawings.size(); i++) {
            drawings[i]->ponVisible(false);
        }
    }

    if (nullptr != apple) {
        vector<Dibujable*> drawings = apple->dibujos();
        for (int i = 0; i < drawings.size(); i++) {
            drawings[i]->ponVisible(false);
        }
    }

    if (nullptr != redBall) {
        vector<Dibujable*> drawings = redBall->dibujos();
        for (int i = 0; i < drawings.size(); i++) {
            drawings[i]->ponVisible(false);
        }
    }
}

void SnakeGame::showGameElements() {
    if (nullptr != snake) {
        vector<Dibujable*> drawings = snake->dibujos();
        for (int i = 0; i < drawings.size(); i++) {
            drawings[i]->ponVisible(true);
        }
    }

    if (nullptr != apple) {
        vector<Dibujable*> drawings = apple->dibujos();
        for (int i = 0; i < drawings.size(); i++) {
            drawings[i]->ponVisible(true);
        }
    }

    if (nullptr != redBall) {
        vector<Dibujable*> drawings = redBall->dibujos();
        for (int i = 0; i < drawings.size(); i++) {
            drawings[i]->ponVisible(true);
        }
    }
}