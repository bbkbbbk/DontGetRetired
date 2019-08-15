#ifndef Map_hpp
#define Map_hpp

#include "NPC.hpp"
#include "Player.hpp"
#include "DynamicObj.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

class Doors;
class DynamicObj;

class Map{
public:
    Map(TextureManager* tex);
    enum Room {AClassroom, BClassroom, CRoom, Lab, KMITL, Seven, Canteen, VRoom} room;
    void clearObj();
    void createObj(int x, int y, int w, int h);
    void createDoor(int x, int y, string destination, int w = 30, int h = 50);
    void changeMap(Room room);
    void changeToAClassroom();
    void changeToBClassroom();
    void changeToCRoom();
    void changeToLab();
    void changeToKMITL();
    void changeToSeven();
    void changeToCanteen();
    void changeToVRoom();
    void changeDay();
    void addRoom(Room r);
    bool checkRoom(Room r);
    void clearRoom(){ currentRoom.clear(); }
    void createDynamicObj(int x, int y, int w, int h, string m, string n);
    sf::RectangleShape mapSprite;
    sf::RectangleShape mapRect;
    vector<Doors> doorsVec;
    vector<sf::RectangleShape> solidObject;
    vector<DynamicObj> dynamicObj;
    vector<NPC> npcVec;
    vector<Room> currentRoom;
private:
    sf::Sprite* player;
    sf::RenderWindow* window;
    TextureManager textures;
    int xpos = 300;
    int ypos = 80;
    int width = 700;
    int height = 931;
    int doorPos = 345;
    int wallHeight = 60;
};

class Doors{
public:
    Doors(int x, int y, int w, int h);
    Map::Room getRoom();
    sf::Vector2f getDestination();
    void teleportTo(string destination);
    sf::RectangleShape doorRect;
private:
    int xpos;
    int ypos;
    int width = 30;
    int height = 50;
    Map::Room destRoom;
    sf::Vector2f Destination;
    map<std::string, Map::Room> Rooms;
    map<string, sf::Vector2f> Teleport;
};

#endif /* Map_hpp */
