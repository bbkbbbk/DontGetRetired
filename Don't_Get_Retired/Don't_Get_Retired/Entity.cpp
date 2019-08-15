#include "Entity.hpp"

Entity::Entity(sf::Texture* entityTexture, string name, int x, int y, int w, int h): name(name), xpos(x), ypos(y), width(w), height(h){
    rect.setSize(sf::Vector2f(width, height));
    rect.setPosition(sf::Vector2f(xpos, ypos));
    sprite.setPosition(rect.getPosition());
    sprite.setTexture(*entityTexture);
    sprite.setTextureRect(sf::IntRect(0, 0, width, height));
}

void Entity::update(){
    if(faceLeft)
        sprite.setTextureRect(sf::IntRect(width * counterFrame, height, width, height));
    else
        sprite.setTextureRect(sf::IntRect(width * counterFrame, 0, width, height));
    counterFrame++;
    if(counterFrame >= 2)
        counterFrame = 0;
}
