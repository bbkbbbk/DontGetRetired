#ifndef DynamicObj_hpp
#define DynamicObj_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

class DynamicObj{
public:
    DynamicObj(int x, int y, int w, int h, string m, string n);
    sf::RectangleShape mesRect;
    string getMessage(){ return message; }
    string getName(){ return name; }
private:
    string message;
    string name;
};

#endif /* DynamicObj_hpp */
