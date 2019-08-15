
/*There is a problem with sf::Texture lifetime so I created this class to fix the problem.
 The problem is when tried to load texture within the Entity class, the texture is destroyed right
 after the class is constructed which result in a white backgrounf instead of an image.
 This class will holds all the textures and return a pointer to a texture which solves the problem */
#ifndef TextureManager_hpp
#define TextureManager_hpp

#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include "ResourcePath.hpp"

class TextureManager{
public:
    void loadTexture();
    void addTexture(std::string name, std::string path);
    sf::Texture* getTexture(std::string name);
private:
    std::map<std::string, sf::Texture> Textures;
};

#endif
