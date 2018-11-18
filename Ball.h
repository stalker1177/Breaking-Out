//
// Created by stalk on 18.11.2018.
//

#ifndef BREAKINGOUT_BALL_H
#define BREAKINGOUT_BALL_H

#include <SFML/Graphics.hpp>

// Класс мяча
class Ball : public sf::CircleShape {
private:
    float dx = 1;
    float dy = -1;
    bool stop = true;
public:
    float Speed;


    Ball(float radius, float outlineThic, sf::Color FillColor, sf::Color OutlineColor, float speed);

    void setSpeed(float speed);


    void reverse_x();

    void reverse_y();

    void setdx(float dx);

    void setdy(float dy);

    float getdx();

    float getdy();

    void toStop();

    void Run();

    bool isStop();

};


#endif //BREAKINGOUT_BALL_H
