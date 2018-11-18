//
// Created by stalk on 18.11.2018.
//

#include "Ball.h"

// Класс мяча
Ball::Ball(float radius, float outlineThic, sf::Color FillColor, sf::Color OutlineColor, float speed = 40.f) {
    setRadius(radius - outlineThic);
    setOutlineThickness(outlineThic);
    setOutlineColor(OutlineColor);
    setFillColor(FillColor);
    setOrigin(radius / 2, radius / 2);
    Speed = speed;
}

void Ball::setSpeed(float speed = 6.f) {
    Speed = speed;
}


void Ball::reverse_x() {
    dx = -dx;
}

void Ball::reverse_y() {
    dy = -dy;
}

void Ball::setdx(float dx) {
    Ball::dx = dx;
}

void Ball::setdy(float dy) {
    Ball::dy = dy;
}

float Ball::getdx() {
    return dx;
}

float Ball::getdy() {
    return dy;
}

void Ball::toStop() {
    Ball::stop = true;
}

void Ball::Run() {
    Ball::stop = false;
}

bool Ball::isStop() {
    return stop;
}
