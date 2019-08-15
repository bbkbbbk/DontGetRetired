#include "Player.hpp"

Player::Player(sf::Texture* PlayerTexture, string name, int x, int y, int w, int h): Entity(PlayerTexture, name, x, y, w, h){
    sprite.setTextureRect(sf::IntRect(0, height, width, height));
}

void Player::update(){
    xpos = sprite.getPosition().x;
    ypos = sprite.getPosition().y;
    rect.setPosition(sprite.getPosition());
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        counterFrame++;
        direction = East;
        if(cantMoveLeft == false){
            faceLeft = true;
            sprite.setTextureRect(sf::IntRect(width * counterFrame, height, width, height));
            sprite.move(-velocity, 0);
            rect.setPosition(sprite.getPosition());
        }
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        counterFrame++;
        direction = West;
        if(cantMoveRight == false){
            faceLeft = false;
            sprite.setTextureRect(sf::IntRect(width * counterFrame, 0, width, height));
            sprite.move(velocity, 0);
            rect.setPosition(sprite.getPosition());
        }
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        counterFrame++;
        direction = North;
        if(cantMoveUp == false){
            if(faceLeft)
                sprite.setTextureRect(sf::IntRect(width * counterFrame, height, width, height));
            else
                sprite.setTextureRect(sf::IntRect(width * counterFrame, 0, width, height));
            sprite.move(0, -velocity);
            rect.setPosition(sprite.getPosition());
            }
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
        counterFrame++;
        direction = South;
        if(cantMoveDown == false){
            if(faceLeft)
                sprite.setTextureRect(sf::IntRect(width * counterFrame, height, width, height));
            else
                sprite.setTextureRect(sf::IntRect(width * counterFrame, 0, width, height));
            sprite.move(0, velocity);
            rect.setPosition(sprite.getPosition());
        }
    }
    else
        Entity::update();
    
    if(counterFrame >= 2)
        counterFrame = 0;
}

void Player::colliding(){
    switch(direction){
        case East:
            cantMoveLeft = true;
            sprite.move(velocity, 0);
            break;
        case West:
            cantMoveRight = true;
            sprite.move(-velocity, 0);
            break;
        case North:
            cantMoveUp = true;
            sprite.move(0, velocity);
            break;
        case South:
            cantMoveDown = true;
            sprite.move(0, -velocity);
            break;
    }
}

void Player::canMove(){
    cantMoveLeft = false;
    cantMoveRight = false;
    cantMoveUp = false;
    cantMoveDown = false;
}



