#include "NPC.hpp"

NPC::NPC(sf::Texture* NPCTexture, string name, int x, int y, int w, int h): Entity(NPCTexture, name, x, y, w, h){
    sprite.setTextureRect(sf::IntRect(0, 0, width, height));
    rect.setSize(sf::Vector2f(width, height - 50));
    loadSpeech("speechs/" + name + "Speech.txt");
}

void NPC::defaultSpeech(){
    loadSpeech("speechs/" + name + "Speech.txt");
}

void NPC::walkX(int x){
    xdest = x;
    velocityX = 15;
    velocityY = 0;
    isWalking = true;
    if(xpos > x){
        faceLeft = true;
        faceDirection = -1;
    }
    else{
        faceLeft = false;
        faceDirection = 1;
    }
}

void NPC::walkY(int y){
    ydest = y;
    velocityX = 0;
    velocityY = 15;
    isWalking = true;
    if(ypos > y)
        faceDirection = -1;
    else
        faceDirection = 1;
}

void NPC::update(){
    xpos = sprite.getPosition().x;
    ypos = sprite.getPosition().y;
    rect.setPosition(sprite.getPosition());
    
    if(xpos == xdest or ypos == ydest)
        isWalking = false;
    
    if(faceLeft)
        sprite.setTextureRect(sf::IntRect(width * counterFrame, height, width, height));
    else
        sprite.setTextureRect(sf::IntRect(width * counterFrame, 0, width, height));
    
    if(isWalking){
        sprite.move(faceDirection * velocityX, faceDirection * velocityY);
        counterFrame++;
        if(counterFrame >= 3)
            counterFrame = 0;
    }
    else{
        counterFrame++;
        if(counterFrame >= 2)
            counterFrame = 0;
    }
}

void NPC::loadSpeech(string fileName){
    ifstream SpeechFile;
    SpeechFile.open(resourcePath() + "data/" + fileName);
    if(!SpeechFile.is_open())
        cout << "Cannot load " << fileName << " speech" << endl;
    speech.clear();
    string line;
    while(getline(SpeechFile, line)){
        speech.push_back(line);
    }
    SpeechFile.close();
}

void NPC::talk(){
    if(!speech.empty()){
        if(nTalk%2 == 0)
            cout << endl << name << ": " << speech[nTalk/2] << endl << endl << endl << endl << endl << endl;
        nTalk++;
        if(nTalk/2 >= speech.size()){
            nTalk = 0;
            doneTalking = true;
            notice = false;
        }
    }
    else
        cout << "Speech empty" << endl;
}
