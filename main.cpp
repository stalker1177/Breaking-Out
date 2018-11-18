#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include "Ball.h"
#include "Block.h"
#include "UserName.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

void menu(sf::RenderWindow &window, bool *isPlaying, Username *name);

void TheEnd(sf::RenderWindow &window, unsigned int *score, bool *isPlaying, int *block_count, Username *name);

void HighScoreWin(sf::RenderWindow &window);

int main() {
    Username name;
    const int LiveCount = 3;
    int lives = LiveCount;
    unsigned int score = 0;
    const int gameWidth = 800;
    const int gameHeight = 600;
    int block_count = 1;
    float ballRadius = 10.f;
    sf::Vector2f paddleSize(100, 15);
    sf::Vector2f blockSize(40, 20);
    Block block[200];
    static int n;

    sf::Font font;
    if (!font.loadFromFile(R"(C:\Users\stalk\CLionProjects\BreakingOut\resources\sansation.ttf)"))
        return EXIT_FAILURE;

    sf::Text ScoreText;
    ScoreText.setFont(font);
    ScoreText.setCharacterSize(24);
    ScoreText.setPosition(20, 450);
    ScoreText.setColor(sf::Color::Black);

    sf::Text LivesText;
    LivesText.setFont(font);
    LivesText.setCharacterSize(24);
    LivesText.setPosition(20, 480);
    LivesText.setColor(sf::Color::Red);

    sf::Text Pause;
    Pause.setFont(font);
    Pause.setCharacterSize(40);
    Pause.setPosition(115, 300);
    Pause.setColor(sf::Color::Black);
    Pause.setString("Pause!\nto resume the Game press \"Tab\"");

    sf::RectangleShape Paddle;
    Paddle.setSize(paddleSize - sf::Vector2f(3, 3));
    Paddle.setOutlineThickness(2);
    Paddle.setOutlineColor(sf::Color(11, 122, 138));
    Paddle.setFillColor(sf::Color(5, 211, 196));
    Paddle.setOrigin(paddleSize / 2.f);
    Paddle.setPosition(gameWidth / 2.f, gameHeight - 20);

    Ball ball(ballRadius, 2, sf::Color(18, 189, 195), sf::Color(30, 85, 89), 250.f);

    const float paddleSpeed = 400.f;

    sf::Clock clock;

    bool isPlaying = false;
    bool isPause = false;
    bool restart = true;
    bool Loose = true;

    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "Breaking Out",
                            sf::Style::Titlebar | sf::Style::Close);

    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    while (window.isOpen()) {

        sf::Event event{};  // Добавляем обработку событий
        while (window.pollEvent(event)) {

            // Если закрываем окно или нажинаем кнопку Esc
            if ((event.type == sf::Event::Closed) ||
                ((event.type == sf::Event::KeyPressed) &&
                 (event.key.code == sf::Keyboard::Escape))) {
                isPause = true;
                menu(window, &isPlaying, &name);

                break;
            }
            // Если нажимаем пробел когда мячик заблокирован, он запускается
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space) && (ball.isStop())) {
                ball.Run();
            }
            // включить/выключить паузу
            if ((event.type == sf::Event::KeyPressed) && ((event.key.code == sf::Keyboard::Tab) && !isPause)) {
                isPause = true;
            } else if ((event.type == sf::Event::KeyPressed) && ((event.key.code == sf::Keyboard::Tab) && isPause)) {
                isPause = false;
            }
        }


        if (isPlaying) {

            //выводим счет на экран во время игры
            std::string scorestr = "Score: ";
            scorestr += std::to_string(score);
            ScoreText.setString(scorestr);

            //выводим количество жизней
            std::string Lives = "Lives: ";
            Lives += std::to_string(lives);
            LivesText.setString(Lives);

            float deltaTime = clock.restart().asSeconds();

            // Пауза блокирует движения
            if (!isPause)   // перемещение доски влево если нажата клавиша
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
                    (Paddle.getPosition().x - paddleSize.x / 2 > 5.f)) {
                    Paddle.move(-paddleSpeed * deltaTime, 0.f);
                    if (ball.isStop()) {
                        ball.move(-paddleSpeed * deltaTime, 0.f);
                    }
                }

            if (!isPause)   // перемещение доски вправо если нажата клавиша
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
                    (Paddle.getPosition().x + paddleSize.x / 2 < gameWidth - 5.f)) {
                    Paddle.move(paddleSpeed * deltaTime, 0.f);
                    if (ball.isStop()) {
                        ball.move(paddleSpeed * deltaTime, 0.f);
                    }
                }

            if (!isPause)   //если мяч остановлен - он двигается вместе с доской
                if (!ball.isStop())
                    ball.move(ball.getdx() * deltaTime * ball.Speed, 0);

            // Проверка столкновений мячика с блоками
            for (int i = 0; i < n; i++)
                if (sf::FloatRect(ball.getPosition().x, ball.getPosition().y, ball.getRadius(),
                                  ball.getRadius()).intersects(block[i].getGlobalBounds())) {
                    if (block[i].isDead()) {    //если у блока кончилось "здоровье" убираем его
                        score++;
                        block[i].setPosition(-100, 0);
                    }
                    ball.reverse_y();
                }

            if (!isPause)
                if (!ball.isStop())
                    ball.move(0, ball.getdy() * deltaTime * ball.Speed);

            for (int i = 0; i < n; i++)
                if (sf::FloatRect(ball.getPosition().x, ball.getPosition().y, ball.getRadius(),
                                  ball.getRadius()).intersects(block[i].getGlobalBounds())) {
                    if (block[i].isDead()) {
                        score++;
                        block[i].setPosition(-100, 0);
                        block_count--;
                    }
                    ball.reverse_x();
                }

            // проверяем столкновения с краями экрана
            if (ball.getPosition().x - ball.getRadius() - 1 < 0 ||
                ball.getPosition().x + ball.getRadius() + 1 > gameWidth)
                ball.reverse_x();

            if (ball.getPosition().y - ball.getRadius() - 1 < 0)
                ball.reverse_y();

            // если мяч упал вниз
            if (ball.getPosition().y > gameHeight) {
                if (--lives < 0) {  // если жизни закончились, прерываем игру,
                    isPlaying = false;
                    restart = true; //ставим флаг для рестарта
                    Loose = true;  //устанавливаем флаг о том что был проигрыш
                    TheEnd(window, &score, &isPlaying, &block_count, &name); //вызов окна конца игры
                } else {// иначе возвращаем мяч на доску
                    ball.setPosition(Paddle.getPosition().x, gameHeight - (2 * ball.getRadius() + 25));
                    ball.toStop();
                }
            }


            if (block_count == 0) { //Если все блоки убраны, предлагаем перезапустить уровень и продолжить
                isPlaying = false;
                restart = true;
                TheEnd(window, &score, &isPlaying, &block_count, &name);
            }

            //обработка ударения о доску
            if (sf::FloatRect(ball.getPosition().x, ball.getPosition().y, ball.getRadius(),
                              ball.getRadius()).intersects(
                    Paddle.getGlobalBounds()))
                ball.setdy(-((rand() % 5 + 8)) * 0.1f);


        }

        // Очистка окна
        window.clear(sf::Color(184, 248, 218));

        //Отрисовываем сообщение о паузе
        if (isPause) {
            window.draw(Pause);
        }

        if (isPlaying) {
            //Рестарт
            if (restart) {
                n = 0;
                //Создаем блоки
                for (int i = 1; i <= 17; i++)
                    for (int j = 1; j <= 10; j++) {
                        block[n].setSize(blockSize - sf::Vector2f(2, 2));
                        block[n].setOutlineThickness(2);
                        block[n].setOutlineColor(sf::Color::Black);
                        block[n].setFillColor(sf::Color(27, 141, 190));
                        block[n].setOrigin(blockSize / 2.f);
                        block[n].setArmor(rand() % 3 + 1);
                        block[n].setPosition(i * 44, j * 23);
                        n++;
                    }
                block_count = n;
                ball.setSpeed(ball.Speed * 1.1f);
                ball.toStop();
                if (Loose) {
                    ball.setSpeed(250);
                    score = 0;
                    Loose = false;
                }
                lives = LiveCount;
                Paddle.setPosition(gameWidth / 2.f, gameHeight - 20);
                ball.setPosition(Paddle.getPosition().x, gameHeight - (2 * ball.getRadius() + 25));
                restart = false;
            }
            // Отрисовка всех элементов игры
            window.draw(Paddle);
            window.draw(ball);
            window.draw(ScoreText);
            window.draw(LivesText);
            for (int i = 0; i < n; i++)
                window.draw(block[i]);
        } else {
            //если игра не запущена - открыто меню
            menu(window, &isPlaying, &name);
        }
        // Отображение элементов
        window.display();
    }
    return 0;
}

//Функция меню
void menu(sf::RenderWindow &window, bool *isPlaying, Username *name) {
    //Объявление текста
    sf::Text Start, HighScore, Exit, UserName, changeName;
    sf::Font font;

    //Загрузка шрифта
    font.loadFromFile(R"(C:\Users\stalk\CLionProjects\BreakingOut\resources\sansation.ttf)");
    UserName.setFont(font);
    UserName.setCharacterSize(40);
    //Заголовок
    sf::Text Head;
    Head.setFont(font);
    Head.setCharacterSize(50);
    Head.setString("Breaking Out - The Game");
    Head.setColor(sf::Color(254, 29, 34));
    Head.setPosition(170, 500);

    // Имя игрока
    sf::String User = "Default Player";
    std::string UserText = "Player: ";
    changeName.setFont(font);
    changeName.setCharacterSize(30);
    changeName.setString("change name");

    //Кнопки управления
    sf::Text bindings;
    bindings.setFont(font);
    bindings.setCharacterSize(30);
    bindings.setString("\"Tab\" - pause\n\"Space\" - start\n\"Esc\" - menu\n\"<-\" - left\n\"->\" - right\n");
    bindings.setPosition(500, 200);

    //Кнопка запуска игры
    Start.setFont(font);
    Start.setCharacterSize(40);
    Start.setString("PLAY");

    //Кнопка открытия рейтинга
    HighScore.setFont(font);
    HighScore.setCharacterSize(40);
    HighScore.setString("HIGH SCORE");

    //Кнопка выхода из игры
    Exit.setFont(font);
    Exit.setCharacterSize(40);
    Exit.setString("Exit the Game.");

    bool isMenu = true;
    int menuNum = 0;

    //Расстановка элементов по окну
    Start.setPosition(100, 200);
    HighScore.setPosition(100, 250);
    Exit.setPosition(100, 300);
    UserName.setPosition(60, 70);
    changeName.setPosition(280, 110);


    while (isMenu) {
        sf::Event event{};
        while (window.pollEvent(event)) {

            // обработка закрытия окна
            if (event.type == sf::Event::Closed) {
                isMenu = false;
                window.close();
                break;
            }
        }

        //Установка цвета по умолчанию для элементов
        Start.setColor(sf::Color::White);
        HighScore.setColor(sf::Color::White);
        Exit.setColor(sf::Color::White);
        UserName.setColor(sf::Color::White);
        changeName.setColor(sf::Color(129, 181, 221));
        UserText = "Player: " + User;
        UserName.setString(UserText);


        menuNum = 0;
        window.clear(sf::Color(129, 181, 221));

        //Смена цвета на синий при наведени мыши и смена флага menuNum
        if (sf::IntRect(100, 200, 105, 50).contains(sf::Mouse::getPosition(window))) {
            Start.setColor(sf::Color::Blue);
            menuNum = 1;
        }

        if (sf::IntRect(100, 250, 238, 50).contains(sf::Mouse::getPosition(window))) {
            HighScore.setColor(sf::Color::Blue);
            menuNum = 2;
        }
        if (sf::IntRect(180, 70, 400, 50).contains(sf::Mouse::getPosition(window))) {
            changeName.setColor(sf::Color::Blue);
            menuNum = 3;
        }
        if (sf::IntRect(100, 300, 260, 50).contains(sf::Mouse::getPosition(window))) {
            Exit.setColor(sf::Color::Blue);
            menuNum = 4;
        }

        //Обраблотка нажатия на кнопку меню
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            switch (menuNum) {
                case (1): {
                    isMenu = false;//если нажали первую кнопку, то выходим из меню
                    *isPlaying = true;
                    break;
                }
                case (2): {
                    HighScoreWin(window);
                    break;
                }
                case (3): {
                    (*name).Enter(window);
                    User = (*name).getName();
                    break;
                }
                case (4): {
                    isMenu = false;
                    window.close();
                    break;
                }
                default:
                    break;
            }
        }
        //Отрисовка всех элементов меню
        window.draw(Exit);
        window.draw(Start);
        window.draw(HighScore);
        window.draw(UserName);
        window.draw(changeName);
        window.draw(bindings);
        window.draw(Head);
        window.display();
    }

}

//Окно конца игры
void TheEnd(sf::RenderWindow &window, unsigned int *score, bool *isPlaying, int *block_count, Username *name) {
    sf::Font font;
    font.loadFromFile(R"(C:\Users\stalk\CLionProjects\BreakingOut\resources\sansation.ttf)");

    //Текст сообщения конца игры
    sf::Text message;
    message.setFont(font);
    message.setCharacterSize(40);
    message.setColor(sf::Color::White);
    sf::String mes;

    //Добавление результатов в рейтинг
    std::string scorestring;
    std::ofstream out("highscore.txt", std::ios::app);
    std::ifstream in("highscore.txt");
    getline(in, scorestring);
    if (!scorestring.empty()) { //Проверка пустой ли файл
        if (*score > stoi(scorestring.substr(
                scorestring.find_last_of(':') + 1))) { //если нет, проверяем перезапишет ли он первое место
            std::string buf;
            std::string masBuf[9];
            masBuf[0] = scorestring;
            int n = 1;
            while (getline(in, buf) && n < 9) {
                masBuf[n++] = buf;
            }
            out.close();
            std::ofstream out("highscore.txt");
            std::string inFile = (*name).getName();
            out << inFile;
            out << ":";
            out << *score;
            out << "\n";
            for (int i = 0; i < n; i++) {
                out << masBuf[i] << "\n";
            }
        }
    } else { // Если да, добавляем первого участника без проверок
        std::string inFile = (*name).getName();
        out << inFile;
        out << ":";
        out << *score;
        out << "\n";
    }
    out.close();
    in.close();


    //В зависимости от того, проиграл игрок, или закончил уровень, выводим разные сообщения
    if ((*block_count) == 0)
        mes = "You WIN\nYour score is: " + std::to_string(*score) +
              "\n\nPress \"Space\" to resume\n\nPress \"Esc\" return to the main menu";
    else if (*score >
             stoi(scorestring.substr(scorestring.find_last_of(':') + 1))) { // если побил рекорд, выводим поздравление
        mes = "New HIGHSCORE! : " + std::to_string(*score) +
              "\nCongratulations!\n" +
              "\nPress \"Space\" to restart\n\nPress \"Esc\" return to the main menu";
    } else {
        mes = "You LOOSE\nYour score is: " + std::to_string(*score) +
              "\n\nPress \"Space\" to restart\n\nPress \"Esc\" return to the main menu";
    }
    message.setString(mes);


    bool isOpen = true;
    message.setPosition(40, 100);

    while (isOpen) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            //Если нажат пробел, перезапускаем уровень
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space)) {
                isOpen = false;
                *isPlaying = true;

                break;
            }
            //Если закрыто окно или нажат esc выходим в меню
            if ((event.type == sf::Event::Closed) ||
                ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
                isOpen = false;
                break;
            }
        }
        //Очистка окна
        window.clear(sf::Color(49, 200, 232));
        //Отрисовка элементов
        window.draw(message);
        window.display();
    }

}


//Вывод рейтинга
void HighScoreWin(sf::RenderWindow &window) {
    std::ifstream file("highscore.txt");
    std::string scores[10];

    //Считывание рейтинга из файла
    int n = 0;
    while (getline(file, scores[n++])) {
    }
    file.close();

    sf::Font font;
    font.loadFromFile(R"(C:\Users\stalk\CLionProjects\BreakingOut\resources\sansation.ttf)");

    //Заголовок
    sf::Text Head;
    Head.setFont(font);
    Head.setCharacterSize(45);
    Head.setPosition(300, 10);
    Head.setColor(sf::Color::Black);
    Head.setString("HighScore");

    //Заголовки
    sf::Text Headers;
    Headers.setFont(font);
    Headers.setCharacterSize(35);
    Headers.setPosition(100, 80);
    Headers.setColor(sf::Color::Black);
    Headers.setString("Place           PlayerName                    Score");


    //вывод таблицы рейтинга
    sf::Text score[n], name[n], num[n];
    for (int i = 0; i < n - 1; i++) {
        score[i].setFont(font);
        score[i].setCharacterSize(30);
        score[i].setPosition(650, 140 + i * 30);
        score[i].setColor(sf::Color::White);
        name[i].setFont(font);
        name[i].setCharacterSize(30);
        name[i].setPosition(275, 140 + i * 30);
        name[i].setColor(sf::Color::White);
        num[i].setFont(font);
        num[i].setCharacterSize(30);
        num[i].setPosition(120, 140 + i * 30);
        num[i].setColor(sf::Color::White);

        num[i].setString(std::to_string(i + 1));
        name[i].setString(scores[i].substr(0, scores[i].find_first_of(':')));
        score[i].setString(scores[i].substr(scores[i].find_first_of(':') + 1));
    }

    bool isOpen = true;


    while (isOpen) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            // Обработка закрытия окна
            if ((event.type == sf::Event::Closed) ||
                ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
                isOpen = false;
                break;
            }
        }

        //Очистка окна
        window.clear(sf::Color(49, 200, 232));

        //Отрисовка всех элементов
        for (int i = 0; i < n; i++) {
            window.draw(score[i]);
            window.draw(name[i]);
            window.draw(num[i]);

        }
        window.draw(Head);
        window.draw(Headers);
        window.display();
    }
}