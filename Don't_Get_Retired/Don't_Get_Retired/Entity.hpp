#ifndef Entity_hpp
#define Entity_hpp

#include "TextureManager.hpp"

using namespace std;

class Entity{
public:
    Entity(sf::Texture* entityTexture, string name, int x = 0, int y = 0, int w = 80, int h = 110);
    virtual void update();
    sf::Sprite sprite;
    sf::RectangleShape rect;
    string getName(){ return name; }
    void setFaceLeft(){ faceLeft = true; }
    float getX(){return xpos;}
    float getY(){return ypos;}
protected:
    int xpos;
    int ypos;
    int width;
    int height;
    int counterFrame = 0;
    string name;
    // declaration of boolean faceLeft is needed because direction west and east will cause confusion in moving up and down whether which direction should sprite facing
    bool faceLeft = false;
};

#endif /* Entity_hpp */
