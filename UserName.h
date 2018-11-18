//
// Created by stalk on 18.11.2018.
//

#ifndef BREAKINGOUT_USERNAME_H
#define BREAKINGOUT_USERNAME_H

#include <SFML/Graphics.hpp>

//Имя игрока
class Username {
    sf::String Name = "Default Player";
public:
    sf::String getName();

    //Окно ввода имени игрока
    void Enter(sf::RenderWindow &window);
};


#endif //BREAKINGOUT_USERNAME_H
