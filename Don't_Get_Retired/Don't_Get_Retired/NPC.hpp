#ifndef NPC_hpp
#define NPC_hpp

#include "Entity.hpp"
#include <fstream>

class NPC: public Entity{
public:
    NPC(sf::Texture* NPCTexture, string name, int x = 0, int y = 0, int w = 80, int h = 110);
    void talk();
    void update();
    void walkX(int x);
    void walkY(int y);
    void loadSpeech(string fileName);
    bool notice = false;
    bool doneTalking = false;
    void resetTalk(){ nTalk = 0; }
    void defaultSpeech();
    bool isWalking = false;
private:
    int xdest = 0;
    int ydest = 0;
    int nTalk = 0;
    int velocityX = 0;
    int velocityY = 0;
    int faceDirection = 1;
    vector<string> speech;
};

#endif /* NPC_hpp */
