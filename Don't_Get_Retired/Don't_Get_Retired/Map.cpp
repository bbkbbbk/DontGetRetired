#include "Map.hpp"

Map::Map(TextureManager* tex){
    textures = *tex;
}

void Map::addRoom(Room r){
    currentRoom.push_back(r);
}
bool Map::checkRoom(Room checkRoom){
    for(auto r : currentRoom)
        if(checkRoom == r)
            return true;
    return false;
}

void Map::createDoor(int x, int y, string destination, int w, int h){
    Doors door(x, y, w, h);
    door.teleportTo(destination);
    doorsVec.push_back(door);
}

void Map::createDynamicObj(int x, int y, int w, int h, string m, string n){
    DynamicObj dObj(x, y, w, h, m, n);
    dynamicObj.push_back(dObj);
}

void Map::clearObj(){
    solidObject.clear();
    dynamicObj.clear();
    doorsVec.clear();
}

void Map::createObj(int x, int y, int w, int h){
    sf::RectangleShape object;
    object.setFillColor(sf::Color::White);
    object.setSize(sf::Vector2f(w, h));
    object.setPosition(x, y);
    solidObject.push_back(object);
}

void Map::changeMap(Room room){
    switch(room){
        case AClassroom:
            changeToAClassroom();
            break;
        case BClassroom:
            changeToBClassroom();
            break;
        case CRoom:
            changeToCRoom();
            break;
        case Lab:
            changeToLab();
            break;
        case KMITL:
            changeToKMITL();
            break;
        case Seven:
            changeToSeven();
            break;
        case Canteen:
            changeToCanteen();
            break;
        case VRoom:
            changeToVRoom();
            break;
    }
}

//map = C Room npc = pFie
//    mainNpc = new NPC(textures.getTexture("pFie"), "pFie", 460, 290, 70, 115);
void Map::changeToCRoom(){
    room = CRoom;
    xpos = 265;
    ypos = 125;
    width = 770;
    height = 839;
    
    mapRect.setPosition(xpos + 70, ypos + 80);
    mapRect.setSize(sf::Vector2f(width-140, height-200));
    mapSprite.setSize(sf::Vector2f(width, height));
    mapSprite.setTexture(textures.getTexture("CRoom"), true);
    mapSprite.setPosition(xpos, ypos);
    clearObj();
    createDoor(xpos, ypos + 340, "AClassroom");
    createDoor(xpos, ypos + 700, "BClassroom");
    createDoor(xpos + 750, ypos + 700, "Lab");
    createDoor(630, 950, "KMITLFromCRoom", 50, 30);
    createObj(xpos, ypos + (2 * wallHeight) - 30, width, 100);
    for(int i = 0; i < 2; i++)
        createObj(415 + (350 * i), 550, 120, 200);
}

// map = A classroom npc = ajTui
//    mainNpc = new NPC(textures.getTexture("ajTui"), "ajTui", 550, 350);
void Map::changeToAClassroom(){
    room = AClassroom;
    xpos = 300;
    ypos = 80;
    width = 700;
    height = 931;
    doorPos = 285;
    
    //boundery 80 is player height
    mapRect.setPosition(xpos + 70, ypos + 80);
    mapRect.setSize(sf::Vector2f(width-140, height-200));
    mapSprite.setSize(sf::Vector2f(width, height));
    mapSprite.setTexture(textures.getTexture("AClass"), true);
    mapSprite.setPosition(xpos, ypos);
    
    clearObj();
    createDoor(xpos + width - 10, ypos + doorPos, "CRoomFromAClassroom");
    createObj(xpos, ypos + (3 * wallHeight), width, 100);
    for(int row = 0; row < 3; row++)
        for(int column = 0; column < 2; column++)
            createObj(400 + (300 * column), 550 + (155 * row), 200, 20);
}

// map = B classroom npc = ajUkrit
//    mainNpc = new NPC(textures.getTexture("ajUkrit"), "ajUkrit", 550, 350);
void Map::changeToBClassroom(){
    room = BClassroom;
    xpos = 300;
    ypos = 80;
    width = 700;
    height = 931;
    doorPos = 285;

    mapRect.setPosition(xpos + 70, ypos + 80);
    mapRect.setSize(sf::Vector2f(width-140, height-200));
    mapSprite.setSize(sf::Vector2f(width, height));
    mapSprite.setTexture(textures.getTexture("BClass"), true);
    mapSprite.setPosition(xpos, ypos);
    
    clearObj();
    createDoor(xpos + width - 10, ypos + doorPos, "CRoomFromBClassroom");
    createObj(xpos, ypos + (3 * wallHeight), width, 100);
    for(int row = 0; row < 3; row++)
        for(int column = 0; column < 2; column++)
            createObj(400 + (300 * column), 550 + (155 * row), 200, 20);
}

//    mainNpc = new NPC(textures.getTexture("michael"), "Michael", 720, 360);
void Map::changeToLab(){
    room = Lab;
    xpos = 270;
    ypos = 80;
    width = 777;
    height = 931;
    doorPos = 285;
    
    mapRect.setPosition(xpos + 70, ypos + 80);
    mapRect.setSize(sf::Vector2f(width-140, height-200));
    mapSprite.setSize(sf::Vector2f(width, height));
    mapSprite.setTexture(textures.getTexture("LabHugo"), true);
    mapSprite.setPosition(xpos, ypos);
    
    clearObj();
    createDoor(xpos - 10, ypos + doorPos, "CRoomFromLab");
    createObj(xpos, ypos + (3 * wallHeight - 20), width, 100);
    for(int row = 0; row < 3; row++)
        for(int column = 0; column < 3; column++)
            createObj(360 + (235 * column), 550 + (115 * row), 130, 20);
}


void Map::changeToKMITL(){
    room = KMITL;
    xpos = 0;
    ypos = 0;
    width = 3150;
    height = 2661;
    
    mapRect.setPosition(770, 1000);
    mapRect.setSize(sf::Vector2f(1670, 1050));
    mapSprite.setSize(sf::Vector2f(width, height));
    mapSprite.setPosition(xpos, ypos);
    mapSprite.setTexture(textures.getTexture("KMITL"), true);
    
    clearObj();

    //bush infront building
    createObj(840, 1290, 460, 50);
    //tree pratep
    createObj(1445, 1000, 270, 90);
    createObj(2045, 1000, 320, 90);
    //pounds
    createObj(2095, 1115, 200, 150);
    //711 atm 1245 1890
    createObj(895, 1505, 450, 375);
    createObj(1370, 1490, 370, 50);
    createObj(1750, 1490, 20, 650);
    //left chair
    createObj(1440, 1925, 20, 100);
    //right chair
    createObj(1685, 1925, 20, 100);
    //ecc
    createObj(1900, 1500, 460, 500);
    
    createDoor(1110, 1850, "Seven");
    createDoor(855, 915, "CRoomFromKMITL", 420, 20);
    createDoor(1795, 915, "Canteen", 200, 20);
    createDoor(2120, 1970, "VRoom", 50, 30);
    
    createDynamicObj(840, 1040, 80, 100, "Is this Ajarn Ukrit's car ?!?", "car");
    createDynamicObj(1495, 1115, 200, 150, "The Lost city of Atlantis might lay beneath this pound", "poud1");
    createDynamicObj(1250, 1890, 50, 50, "I wish I have enough money to withdraw from an ATM...", "atm");
    createDynamicObj(1500, 1700, 60, 50, "The monkey on the left...", "monkey1");
    createDynamicObj(1600, 1700, 60, 50, "Not sure why you are so interested with these monkeys...", "monkey2");
    createDynamicObj(1550, 1750, 50, 50, "May be he is the king of those monkeys...", "king");
}

void Map::changeToSeven(){
    room = Seven;
    xpos = 270;
    ypos = 150;
    width = 770;
    height = 772;
    
    mapRect.setPosition(xpos + 70, ypos + 80);
    mapRect.setSize(sf::Vector2f(width-140, height-200));
    mapSprite.setSize(sf::Vector2f(width, height));
    mapSprite.setTexture(textures.getTexture("Seven"), true);
    mapSprite.setPosition(xpos, ypos);
    
    clearObj();
    createDoor(570, 900, "KMITLFromSeven");
    createObj(xpos, ypos, width, 100);
    createObj(430, 400, 80, 450);
    for(int row = 0; row < 2; row++)
        createObj(680, 390 + (180 * row), 250, 20);
    createObj(680, 710, 250, 20);
}

void Map::changeToCanteen(){
    room = Canteen;
    xpos = 125;
    ypos = 150;
    width = 1050;
    height = 768;
    
    mapRect.setPosition(xpos + 70, ypos + 80);
    mapRect.setSize(sf::Vector2f(width-140, height-200));
    mapSprite.setSize(sf::Vector2f(width, height));
    mapSprite.setTexture(textures.getTexture("Canteen"), true);
    mapSprite.setPosition(xpos, ypos);
    
    clearObj();
    createDoor(185, ypos + height - 10, "KMITLFromCanteen", 50, 30);
    createObj(xpos, ypos, width, 190);
    for(int row = 0; row < 3; row++)
        createObj(415 + (280 * row), 500, 50, 275);
    createDynamicObj(300, 350, 250, 20, "Drinks shop", "drinkShop");
    createDynamicObj(800, 350, 250, 20, "Lunch box shop", "lunchShop");
}

void Map::changeToVRoom(){
    room = VRoom;
    xpos = 300;
    ypos = 200;
    width = 700;
    height = 629;
    
    mapRect.setPosition(xpos + 70, ypos + 80);
    mapRect.setSize(sf::Vector2f(width-140, height-200));
    mapSprite.setSize(sf::Vector2f(width, height));
    mapSprite.setTexture(textures.getTexture("VRoom"), true);
    mapSprite.setPosition(xpos, ypos);
    
    clearObj();
    createDoor(370, ypos + height - 10, "KMITLFromVRoom", 50, 30);
    createObj(xpos, ypos, width, 150);
    createObj(740, 490, 175, 150);
    
}

Doors::Doors(int x, int y, int w, int h): xpos(x), ypos(y){
    Teleport["AClassroom"] = sf::Vector2f(900, 365);
    Teleport["BClassroom"] = sf::Vector2f(900, 365);
    Teleport["CRoomFromAClassroom"] = sf::Vector2f(300, 420);
    Teleport["CRoomFromBClassroom"] = sf::Vector2f(300, 800);
    Teleport["CRoomFromLab"] = sf::Vector2f(930, 800);
    Teleport["CRoomFromKMITL"] = sf::Vector2f(605, 825);
    Teleport["Lab"] = sf::Vector2f(300, 365);
    Teleport["Seven"] = sf::Vector2f(545, 780);
    Teleport["Canteen"] = sf::Vector2f(195, 790);
    Teleport["VRoom"] = sf::Vector2f(370, 690);
    Teleport["KMITLFromSeven"] = sf::Vector2f(1095, 1915);
    Teleport["KMITLFromCanteen"] = sf::Vector2f(1870, 940);
    Teleport["KMITLFromVRoom"] = sf::Vector2f(2095, 2015);
    Teleport["KMITLFromCRoom"] = sf::Vector2f(1045, 950);
    
    Rooms["AClassroom"] = Map::AClassroom;
    Rooms["BClassroom"] = Map::BClassroom;
    Rooms["CRoomFromAClassroom"] = Map::CRoom;
    Rooms["CRoomFromBClassroom"] = Map::CRoom;
    Rooms["CRoomFromLab"] = Map::CRoom;
    Rooms["CRoomFromKMITL"] = Map::CRoom;
    Rooms["Lab"] = Map::Lab;
    Rooms["KMITLFromSeven"] = Map::KMITL;
    Rooms["KMITLFromVRoom"] = Map::KMITL;
    Rooms["KMITLFromCRoom"] = Map::KMITL;
    Rooms["KMITLFromCanteen"] = Map::KMITL;

    Rooms["Seven"] = Map::Seven;
    Rooms["Canteen"] = Map::Canteen;
    Rooms["VRoom"] = Map::VRoom;

    doorRect.setSize(sf::Vector2f(w, h));
    doorRect.setFillColor(sf::Color::Yellow);
    doorRect.setPosition(xpos, ypos);
}

void Doors::teleportTo(string destination){
    Destination = Teleport[destination];
    destRoom = Rooms[destination];
}
sf::Vector2f Doors::getDestination(){
    return Destination;
}

Map::Room Doors::getRoom(){
    return destRoom;
}

