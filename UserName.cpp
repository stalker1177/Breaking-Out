//
// Created by stalk on 18.11.2018.
//

#include "UserName.h"

sf::String Username::getName() {
    return Name.toAnsiString();
}

//Окно ввода имени игрока
void Username::Enter(sf::RenderWindow &window) {
    sf::Font font;
    font.loadFromFile(R"(C:\Users\stalk\CLionProjects\BreakingOut\resources\sansation.ttf)");


    sf::Text message;
    message.setFont(font);
    message.setCharacterSize(40);
    message.setColor(sf::Color::White);
    sf::String mes;
    mes = "Please enter your name, and press Enter";
    message.setString(mes);

    sf::Text name;
    name.setFont(font);
    name.setCharacterSize(40);
    name.setColor(sf::Color::White);


    bool isOpen = true;
    message.setPosition(40, 400);
    name.setPosition(80, 220);
    name.setString(Name);
    while (isOpen) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            // Обработка события ввода текста
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) { // проверка кодировки
                    if (static_cast<char>(event.text.unicode) != 8) { //ввод символов если это не бекспейс
                        Name += static_cast<char>(event.text.unicode);
                        name.setString(Name);
                    } else { //убираем последний элемент если нажат бэкспейс
                        Name = Name.substring(0, Name.getSize() - 1);
                        name.setString(Name);
                    }


                }

            }
            //если нажат enter закрываем окно
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Enter)) {
                isOpen = false;
                break;
            }
            //если окно закрыто иначе,тоже ничего не меняем
            if ((event.type == sf::Event::Closed) ||
                ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
                isOpen = false;
                break;
            }
        }
        window.clear(sf::Color(49, 200, 232));
        //отрисовка элементов
        window.draw(message);
        window.draw(name);
        window.display();
    }
}