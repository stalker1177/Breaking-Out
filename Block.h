//
// Created by stalk on 18.11.2018.
//

#ifndef BREAKINGOUT_BLOCK_H
#define BREAKINGOUT_BLOCK_H

#include <SFML/Graphics.hpp>

//класс Блок
class Block : public sf::RectangleShape {
private:
// крепкость блока
    int health;
public:
    Block() {};

//проверка на остаток hp
    bool isDead();

// установка крепкости
    void setArmor(int arm);

};


#endif //BREAKINGOUT_BLOCK_H
