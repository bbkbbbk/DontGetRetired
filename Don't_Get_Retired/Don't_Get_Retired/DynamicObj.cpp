#include "DynamicObj.hpp"

DynamicObj::DynamicObj(int x, int y, int w, int h, string m, string n){
    mesRect.setFillColor(sf::Color::Green);
    mesRect.setSize(sf::Vector2f(w, h));
    mesRect.setPosition(x, y);
    message = m;
    name = n;
}
