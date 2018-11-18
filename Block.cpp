//
// Created by stalk on 18.11.2018.
//

#include "Block.h"

//проверка на остаток hp
bool Block::isDead() {
    return --health == 0;
}

// установка крепкости
void Block::setArmor(int arm) {
    health = arm;
}