#include "TextureManager.hpp"

void TextureManager::addTexture(std::string name, std::string path){
    sf::Texture tmpTexture;
    if (!tmpTexture.loadFromFile(resourcePath() + "/assets/" + path)){
        std::cout << "Cannot load texture." << std::endl;
    }
    Textures[name] = tmpTexture;
}

sf::Texture* TextureManager::getTexture(std::string name){
    sf::Texture* ptrTexture = &Textures[name];
    return ptrTexture;
}

void TextureManager::loadTexture(){
    addTexture("man1", "sprites/manTexture.png");
    addTexture("man2", "sprites/BclassmateTexture.png");
    addTexture("man3", "sprites/CclassmateTexture.png");
    
    addTexture("pFie", "sprites/pFieTexture.png");
    addTexture("ajSun", "sprites/ajSunTexture.png");
    addTexture("ajTui", "sprites/ajTuiTexture.png");
    addTexture("ajUkrit", "sprites/ajUkritTexture.png");
    addTexture("drV", "sprites/drVTexture.png");
    addTexture("michael", "sprites/michaelTexture.png");
    
    addTexture("ajVeera", "sprites/ajVeeraTexture.png");
    addTexture("ajKo", "sprites/ajKoTexture.png");
    addTexture("ajMonde", "sprites/ajMondeTexture.png");
    addTexture("jochen", "sprites/jochenTexture.png");

    addTexture("sevenGirl", "sprites/sevenGirlTexture.png");
    addTexture("cat", "sprites/catTexture.png");

    addTexture("AClass", "setting/AclassTexture.png");
    addTexture("BClass", "setting/BclassTexture.png");
    addTexture("CRoom", "setting/commonRoom.png");
    addTexture("Lab", "setting/LabTexture.png");
    addTexture("LabHugo", "setting/LabTextureHugo.png");
    addTexture("KMITL", "setting/kmitlTexture.png");
    addTexture("Seven", "setting/sevenTexture.png");
    addTexture("Canteen", "setting/CanteenTexture.png");
    addTexture("VRoom", "setting/VroomTexture.png");
    addTexture("menu", "menu.png");

    addTexture("redMark", "redMarkTexture.png");
    addTexture("startBtn", "startBtn.png");
    addTexture("startPBtn", "startPressedBtn.png");
    addTexture("loadBtn", "loadBtn.png");
    addTexture("loadPBtn", "loadPressedBtn.png");
}
