#ifndef Player_hpp
#define Player_hpp

#include "Entity.hpp"

class Player:public Entity {
public:
    Player(sf::Texture* playerTexture, string name, int x, int y, int w = 80, int h = 110);
    void update();
    void canMove();
    void colliding();
    int point = 0;
    void setPoint(int p = 0){point = p;}
    sf::Sound collision;
private:
    int velocity = 25;
    bool cantMoveUp = false;
    bool cantMoveDown = false;
    bool cantMoveLeft = false;
    bool cantMoveRight = false;
    enum {East, West, North, South} direction;
};

#endif /* Player_hpp */
