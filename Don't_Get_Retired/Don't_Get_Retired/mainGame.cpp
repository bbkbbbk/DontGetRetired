#include "Player.hpp"
#include "Map.hpp"
#include "TextureManager.hpp"
#include <vector>

void loadGame(Player *player, int* day){
    cout << "Loading game..." << endl;
    ifstream infile;
    infile.open(resourcePath() + "data/playerData.txt");
    if(!infile.is_open())
        cout << "Cannot load game" << endl;
    string line;
    getline(infile, line);
    infile.close();
    
    stringstream strm(line);
    int d, p;
    strm >> d >> p;
    if(d == 0 and p == 0)
        cout << "There is no previous data" << endl;
    else{
        player->setPoint(p);
        *day = d;
    }
}

void saveGame(int point, int day){
    cout << "Saving game..." << endl;
    ofstream outFile;
    outFile.open(resourcePath() + "data/playerData.txt");
    if(!outFile.is_open())
        cout << "Cannot save game" << endl;
    
    outFile << day << " " << point << endl;
    outFile.close();
}

int day1(sf::RenderWindow* window, sf::View* view, TextureManager* texture, Map* currentMap, Player* player, int* day){
    currentMap->clearRoom();
    sf::Clock timer;
    sf::RectangleShape markRect;
    markRect.setTexture(texture->getTexture("redMark"));
    markRect.setSize(sf::Vector2f(64, 64));
    
    sf::SoundBuffer buff1, buff2, buff3, buff4;
    if(!buff1.loadFromFile(resourcePath() + "assets/sounds/level.ogg")) {return EXIT_FAILURE;}
    sf::Sound levelUp;
    levelUp.setBuffer(buff1);
    if(!buff2.loadFromFile(resourcePath() + "assets/sounds/door.ogg")) {return EXIT_FAILURE;}
    sf::Sound enter;
    enter.setBuffer(buff2);
    if(!buff3.loadFromFile(resourcePath() + "assets/sounds/seven.ogg")) {return EXIT_FAILURE;}
    sf::Sound enterSeven;
    enterSeven.setBuffer(buff3);
    if(!buff4.loadFromFile(resourcePath() + "assets/sounds/collision.ogg")) {return EXIT_FAILURE;}
    sf::Sound col;
    col.setBuffer(buff4);
    
    sf::Font font;
    if(!font.loadFromFile(resourcePath() + "assets/upheavtt.ttf")) {return EXIT_FAILURE;}
    sf::Text text("Day " + to_string(*day), font, 140);
    text.setFillColor(sf::Color::Black);
    text.setPosition(900, 500);

    NPC pFie(texture->getTexture("pFie"), "pFie", 460, 290, 70, 115);
    NPC ajSun(texture->getTexture("ajSun"), "ajSun", 550, 350);
    NPC sevenGirl(texture->getTexture("sevenGirl"), "sevenGirl", 345, 615, 70, 115);

    pFie.loadSpeech("days/d1/pFie1.txt");
    NPC* mainNpc = nullptr;
    pFie.notice = true;
    mainNpc = &pFie;
    player->setFaceLeft();
    
    currentMap->addRoom(Map::CRoom);
    currentMap->addRoom(Map::AClassroom);
    currentMap->addRoom(Map::Seven);

    bool inDay =  true;
    bool show = false;
    bool recievePFieQ = false;
    bool talkedAjSun = false;
    bool donePFieQ = false;
    bool recieveAjSunQ = false;
    bool doneAjSun = false;
    bool boughtFood = false;
    bool wait = false;
    int drink = 0;

    while(inDay){
        sf::Event event;
        while (window->pollEvent(event)){
            if (event.type == sf::Event::Closed){
                inDay = false;
                window->close();
            }
        }
        
        if(mainNpc != nullptr){
            if(player->rect.getGlobalBounds().intersects(mainNpc->rect.getGlobalBounds())){
                player->colliding();
                mainNpc->talk();
                col.play();
            }
            else
                player->canMove();
            
            if(mainNpc->getName() == "pFie"){
                //first talk with pFie
                if(!recievePFieQ and mainNpc->doneTalking ){
                    recievePFieQ = true;
                    pFie.doneTalking = false;
                }
                //after talked with ajSun and finish conversation with pFie
                if(talkedAjSun and mainNpc->doneTalking and !recieveAjSunQ and !donePFieQ){
                    donePFieQ = true;
                    ajSun.notice = true;
                    ajSun.loadSpeech("days/d1/validArgument.txt");
                }
            }
            if(mainNpc->getName() == "ajSun"){
                //must recieve quest from pfei first in order to be able unlock talkedAjSun bool
                if(recievePFieQ and mainNpc->doneTalking and !talkedAjSun){
                    talkedAjSun = true;
                    pFie.notice = true;
                    pFie.loadSpeech("days/d1/pFie2.txt");
                }
                //after recieve quest from ajSun, his speech will change into a short version that breifly explain his quest
                if(donePFieQ and !recieveAjSunQ and mainNpc->doneTalking ){
                    recieveAjSunQ = true;
                    ajSun.loadSpeech("days/d1/validArgumentShort.txt");
                }
                if(boughtFood and mainNpc->doneTalking and !doneAjSun
                   and !wait){
                    doneAjSun = true;
                    wait = true;
                    player->point += 5;
                    levelUp.play();
                    *day = 2;
                    timer.restart();
                }
            }
        }
        
        if(!doneAjSun)
            ajSun.doneTalking = false;
        else
            if(timer.getElapsedTime() > sf::seconds(5.0f))
                inDay = false;
        
        if(drink != 0){
            ajSun.notice = true;
            if(drink == 1)
                ajSun.loadSpeech("days/d1/wrong.txt");
            else if(drink == 2)
                ajSun.loadSpeech("days/d1/right.txt");
        }
        
        for(auto obj: currentMap->solidObject)
            if(player->rect.getGlobalBounds().intersects(obj.getGlobalBounds())){
                col.play();
                player->colliding();
            }

        if(!player->rect.getGlobalBounds().intersects(currentMap->mapRect.getGlobalBounds())){
            col.play();
            player->colliding();
        }
        else
            player->canMove();

        for(auto& door: currentMap->doorsVec){
            if(player->rect.getGlobalBounds().intersects(door.doorRect.getGlobalBounds())){
                player->sprite.setPosition(door.getDestination());
                view->setCenter(player->sprite.getPosition());
                currentMap->changeMap(door.getRoom());
                if(mainNpc != nullptr)
                    mainNpc->resetTalk();
                if(currentMap->room == Map::AClassroom)
                    mainNpc = &ajSun;
                else if(currentMap->room == Map::CRoom)
                    mainNpc = &pFie;
                else if(currentMap->room == Map::Seven)
                    mainNpc = &sevenGirl;
                else
                    mainNpc = nullptr;
                
                if(currentMap->room == Map::Seven)
                    enterSeven.play();
                else
                    enter.play();
            }
        }

        for(auto& dObj: currentMap->dynamicObj){
            if(player->rect.getGlobalBounds().intersects(dObj.mesRect.getGlobalBounds())){
                col.play();
                player->colliding();
                cout << player->getName() << ": " << dObj.getMessage() << endl << endl << endl << endl << endl << endl << endl << endl;
                if(currentMap->room == Map::Canteen){
                    boughtFood = true;
                    if(dObj.getName() == "drinkShop")
                        drink = 1;
                    else if(dObj.getName() == "lunchShop")
                        drink = 2;
                }
            }
            else
                player->canMove();
        }

        if(currentMap->room == Map::KMITL){
            window->setView(*view);
            view->setCenter(player->sprite.getPosition());
        }
        else
            window->setView(window->getDefaultView());

        window->clear();
        window->draw(currentMap->mapSprite);
        if(mainNpc != nullptr and currentMap->checkRoom(currentMap->room)){
            mainNpc->update();
            window->draw(mainNpc->sprite);
            markRect.setPosition(mainNpc->getX() + 70, mainNpc->getY() - 70);
            if(mainNpc->notice)
                window->draw(markRect);
        }
        
        if(timer.getElapsedTime() < sf::seconds(1.0f) and !doneAjSun)
            window->draw(text);
        
        player->update();
        window->draw(player->sprite);
        window->display();
    }
}

int day2(sf::RenderWindow* window, sf::View* view, TextureManager* texture, Map* currentMap, Player* player, int* day){
    currentMap->clearRoom();
    sf::Clock timer;
    sf::RectangleShape markRect;
    markRect.setTexture(texture->getTexture("redMark"));
    markRect.setSize(sf::Vector2f(64, 64));
    
    sf::SoundBuffer buff1, buff2, buff3, buff4;
    if(!buff1.loadFromFile(resourcePath() + "assets/sounds/level.ogg")) {return EXIT_FAILURE;}
    sf::Sound levelUp;
    levelUp.setBuffer(buff1);
    if(!buff2.loadFromFile(resourcePath() + "assets/sounds/door.ogg")) {return EXIT_FAILURE;}
    sf::Sound enter;
    enter.setBuffer(buff2);
    if(!buff3.loadFromFile(resourcePath() + "assets/sounds/seven.ogg")) {return EXIT_FAILURE;}
    sf::Sound enterSeven;
    enterSeven.setBuffer(buff3);
    if(!buff4.loadFromFile(resourcePath() + "assets/sounds/collision.ogg")) {return EXIT_FAILURE;}
    sf::Sound col;
    col.setBuffer(buff4);
    
    sf::Font font;
    if(!font.loadFromFile(resourcePath() + "assets/upheavtt.ttf")) {return EXIT_FAILURE;}
    sf::Text text("Day " + to_string(*day), font, 140);
    text.setFillColor(sf::Color::Black);
    text.setPosition(900, 500);
    
    NPC pFie(texture->getTexture("pFie"), "pFie", 460, 290, 70, 115);
    NPC ajTui(texture->getTexture("ajTui"), "ajTui", 550, 350);
    NPC sevenGirl(texture->getTexture("sevenGirl"), "sevenGirl", 345, 615, 70, 115);
    sevenGirl.rect.setSize(sf::Vector2f(180 , 100));
    
    pFie.loadSpeech("days/d2/pFie.txt");
    ajTui.loadSpeech("days/d2/buyMarker.txt");
    NPC* mainNpc = nullptr;
    pFie.notice = true;
    ajTui.notice = true;
    mainNpc = &pFie;
    player->setFaceLeft();
    
    currentMap->addRoom(Map::CRoom);
    currentMap->addRoom(Map::BClassroom);
    currentMap->addRoom(Map::Seven);
    
    bool inDay =  true;
    bool show = false;
    bool wait = false;
    bool recieveAjTui = false;
    bool boughtMarker = false;
    bool doneAjTui = false;
    
    while(inDay){
        sf::Event event;
        while (window->pollEvent(event)){
            if (event.type == sf::Event::Closed){
                inDay = false;
                window->close();
            }
        }
        
        if(mainNpc != nullptr){
            if(player->rect.getGlobalBounds().intersects(mainNpc->rect.getGlobalBounds())){
                player->colliding();
                col.play();
                mainNpc->talk();
            }
            else
                player->canMove();
            
            if(mainNpc->getName() == "pFie"){
                if(mainNpc->doneTalking )
                    pFie.defaultSpeech();
            }
            if(mainNpc->getName() == "ajTui"){
                if(!recieveAjTui and  mainNpc->doneTalking){
                    recieveAjTui = true;
                    ajTui.loadSpeech("days/d2/buyMarkerShort.txt");
                    sevenGirl.loadSpeech("days/d2/sevenGirl.txt");
                }
                if(recieveAjTui and boughtMarker and mainNpc->doneTalking and !wait){
                    player->point += 5;
                    *day = 3;
                    levelUp.play();
                    doneAjTui = true;
                    wait = true;
                    timer.restart();
                }
            }
            if(mainNpc->getName() == "sevenGirl"){
                if(mainNpc->doneTalking and recieveAjTui){
                    mainNpc->defaultSpeech();
                    boughtMarker = true;
                    ajTui.notice = true;
                }
            }
        }
        
        if(boughtMarker)
            ajTui.loadSpeech("days/d2/ajTui.txt");
        
        if(!doneAjTui)
            ajTui.doneTalking = false;
        else
            if(timer.getElapsedTime() > sf::seconds(3.0f))
                inDay = false;
        
        for(auto obj: currentMap->solidObject)
            if(player->rect.getGlobalBounds().intersects(obj.getGlobalBounds())){
                player->colliding();
                col.play();
            }
        
        if(!player->rect.getGlobalBounds().intersects(currentMap->mapRect.getGlobalBounds())){
            player->colliding();
            col.play();
        }
        else
            player->canMove();
        
        for(auto& door: currentMap->doorsVec){
            if(player->rect.getGlobalBounds().intersects(door.doorRect.getGlobalBounds())){
                player->sprite.setPosition(door.getDestination());
                view->setCenter(player->sprite.getPosition());
                currentMap->changeMap(door.getRoom());
                if(currentMap->room == Map::BClassroom)
                    mainNpc = &ajTui;
                else if(currentMap->room == Map::CRoom)
                    mainNpc = &pFie;
                else if(currentMap->room == Map::Seven)
                    mainNpc = &sevenGirl;
                else
                    mainNpc = nullptr;
                
                if(mainNpc != nullptr)
                    mainNpc->resetTalk();
                
                if(currentMap->room == Map::Seven)
                    enterSeven.play();
                else
                    enter.play();
            }
        }
        
        for(auto& dObj: currentMap->dynamicObj){
            if(player->rect.getGlobalBounds().intersects(dObj.mesRect.getGlobalBounds())){
                player->colliding();
                col.play();
                cout << player->getName() << ": " << dObj.getMessage() << endl << endl << endl << endl << endl << endl << endl << endl;
            }
            else
                player->canMove();
        }
        
        if(currentMap->room == Map::KMITL){
            window->setView(*view);
            view->setCenter(player->sprite.getPosition());
        }
        else
            window->setView(window->getDefaultView());
        
        window->clear();
        window->draw(currentMap->mapSprite);
        if(mainNpc != nullptr and currentMap->checkRoom(currentMap->room)){
            mainNpc->update();
            window->draw(mainNpc->sprite);
            markRect.setPosition(mainNpc->getX() + 70, mainNpc->getY() - 70);
            if(mainNpc->notice)
                window->draw(markRect);
        }
        
        if(timer.getElapsedTime() < sf::seconds(1.0f) and !doneAjTui)
            window->draw(text);
        
        player->update();
        window->draw(player->sprite);
        window->display();
    }
}

int day3(sf::RenderWindow* window, sf::View* view, TextureManager* texture, Map* currentMap, Player* player, int* day){
    currentMap->clearRoom();
    sf::Clock timer;
    sf::RectangleShape markRect;
    markRect.setTexture(texture->getTexture("redMark"));
    markRect.setSize(sf::Vector2f(64, 64));
    
    sf::SoundBuffer buff1, buff2, buff3, buff4;
    if(!buff1.loadFromFile(resourcePath() + "assets/sounds/level.ogg")) {return EXIT_FAILURE;}
    sf::Sound levelUp;
    levelUp.setBuffer(buff1);
    if(!buff2.loadFromFile(resourcePath() + "assets/sounds/door.ogg")) {return EXIT_FAILURE;}
    sf::Sound enter;
    enter.setBuffer(buff2);
    if(!buff3.loadFromFile(resourcePath() + "assets/sounds/seven.ogg")) {return EXIT_FAILURE;}
    sf::Sound enterSeven;
    enterSeven.setBuffer(buff3);
    if(!buff4.loadFromFile(resourcePath() + "assets/sounds/collision.ogg")) {return EXIT_FAILURE;}
    sf::Sound col;
    col.setBuffer(buff4);
    
    sf::Font font;
    if(!font.loadFromFile(resourcePath() + "assets/upheavtt.ttf")) {return EXIT_FAILURE;}
    sf::Text text("Day " + to_string(*day), font, 140);
    text.setFillColor(sf::Color::Black);
    text.setPosition(900, 500);
    
    NPC pFie(texture->getTexture("pFie"), "pFie", 460, 290, 70, 115);
    NPC drV(texture->getTexture("drV"), "drV", 870, 665);
    NPC sevenGirl(texture->getTexture("sevenGirl"), "sevenGirl", 345, 615, 70, 115);
    
    pFie.loadSpeech("days/d3/pFie.txt");
    drV.loadSpeech("days/d3/drV1.txt");
    NPC* mainNpc = nullptr;
    pFie.notice = true;
    mainNpc = &pFie;
    player->setFaceLeft();
    drV.setFaceLeft();
    
    currentMap->addRoom(Map::CRoom);
    currentMap->addRoom(Map::VRoom);
    currentMap->addRoom(Map::Seven);
    
    bool inDay =  true;
    bool show = false;
    bool wait = false;
    bool recievePFie = false;
    bool donePFie = false;
    bool enterBRoom = false;
    
    while(inDay){
        sf::Event event;
        while (window->pollEvent(event)){
            if (event.type == sf::Event::Closed){
                inDay = false;
                window->close();
            }
        }
        
        if(mainNpc != nullptr){
            if(player->rect.getGlobalBounds().intersects(mainNpc->rect.getGlobalBounds())){
                col.play();
                player->colliding();
                mainNpc->talk();
            }
            else
                player->canMove();
            
            if(mainNpc->getName() == "pFie"){
                if(!recievePFie and enterBRoom and mainNpc->doneTalking){
                    recievePFie = true;
                    drV.notice = true;
                    drV.loadSpeech("days/d3/drV2.txt");
                    pFie.loadSpeech("days/d3/findDrVShort.txt");
                }
            }
            
            if(mainNpc->getName() == "drV"){
                if(mainNpc->doneTalking and recievePFie and !wait){
                    levelUp.play();
                    wait = true;
                    *day = 4;
                    player->point += 5;
                    donePFie = true;
                    timer.restart();
                }
            }
        }
        
        if(!donePFie)
            pFie.doneTalking = false;
        else
            if(timer.getElapsedTime() > sf::seconds(3.0f))
                inDay = false;
        
        for(auto obj: currentMap->solidObject)
            if(player->rect.getGlobalBounds().intersects(obj.getGlobalBounds())){
                col.play();
                player->colliding();
            }
        
        if(!player->rect.getGlobalBounds().intersects(currentMap->mapRect.getGlobalBounds())){
            col.play();
            player->colliding();
        }
        else
            player->canMove();
        
        for(auto& door: currentMap->doorsVec){
            if(player->rect.getGlobalBounds().intersects(door.doorRect.getGlobalBounds())){
                player->sprite.setPosition(door.getDestination());
                view->setCenter(player->sprite.getPosition());
                currentMap->changeMap(door.getRoom());
                
                if(currentMap->room == Map::VRoom)
                    mainNpc = &drV;
                else if(currentMap->room == Map::CRoom)
                    mainNpc = &pFie;
                else if(currentMap->room == Map::Seven)
                    mainNpc = &sevenGirl;
                else
                    mainNpc = nullptr;
                
                if(mainNpc != nullptr)
                    mainNpc->resetTalk();
                
                if(currentMap->room == Map::BClassroom){
                    enterBRoom = true;
                    pFie.notice = true;
                    pFie.loadSpeech("days/d3/findDrV.txt");
                }
                
                if(currentMap->room == Map::Seven)
                    enterSeven.play();
                else
                    enter.play();
            }
        }
        
        
        for(auto& dObj: currentMap->dynamicObj){
            if(player->rect.getGlobalBounds().intersects(dObj.mesRect.getGlobalBounds())){
                col.play();
                player->colliding();
                cout << player->getName() << ": " << dObj.getMessage() << endl << endl << endl << endl << endl << endl << endl << endl;
            }
            else
                player->canMove();
        }
        
        if(currentMap->room == Map::KMITL){
            window->setView(*view);
            view->setCenter(player->sprite.getPosition());
        }
        else
            window->setView(window->getDefaultView());
        
        window->clear();
        window->draw(currentMap->mapSprite);
        if(mainNpc != nullptr and currentMap->checkRoom(currentMap->room)){
            mainNpc->update();
            window->draw(mainNpc->sprite);
            markRect.setPosition(mainNpc->getX() + 70, mainNpc->getY() - 70);
            if(mainNpc->notice)
                window->draw(markRect);
        }
        
        if(timer.getElapsedTime() < sf::seconds(1.0f) and !donePFie)
            window->draw(text);
        
        player->update();
        window->draw(player->sprite);
        window->display();
    }
}

int day4(sf::RenderWindow* window, sf::View* view, TextureManager* texture, Map* currentMap, Player* player, int* day){
    currentMap->clearRoom();
    sf::Clock timer;
    sf::RectangleShape markRect;
    markRect.setTexture(texture->getTexture("redMark"));
    markRect.setSize(sf::Vector2f(64, 64));
    
    sf::SoundBuffer buff1, buff2, buff3, buff4;
    if(!buff1.loadFromFile(resourcePath() + "assets/sounds/level.ogg")) {return EXIT_FAILURE;}
    sf::Sound levelUp;
    levelUp.setBuffer(buff1);
    if(!buff2.loadFromFile(resourcePath() + "assets/sounds/door.ogg")) {return EXIT_FAILURE;}
    sf::Sound enter;
    enter.setBuffer(buff2);
    if(!buff3.loadFromFile(resourcePath() + "assets/sounds/seven.ogg")) {return EXIT_FAILURE;}
    sf::Sound enterSeven;
    enterSeven.setBuffer(buff3);
    if(!buff4.loadFromFile(resourcePath() + "assets/sounds/collision.ogg")) {return EXIT_FAILURE;}
    sf::Sound col;
    col.setBuffer(buff4);
    
    sf::Font font;
    if(!font.loadFromFile(resourcePath() + "assets/upheavtt.ttf")) {return EXIT_FAILURE;}
    sf::Text text("Day " + to_string(*day), font, 140);
    text.setFillColor(sf::Color::Black);
    text.setPosition(900, 500);
    
    NPC pFie(texture->getTexture("pFie"), "pFie", 460, 290, 70, 115);
    NPC ajUkrit(texture->getTexture("ajUkrit"), "ajUkrit", 550, 350);
    NPC cat(texture->getTexture("cat"), "cat", 1600, 1550, 140, 110);
    NPC sevenGirl(texture->getTexture("sevenGirl"), "sevenGirl", 345, 615, 70, 115);

    pFie.loadSpeech("days/d4/pFie1.txt");
    NPC* mainNpc = nullptr;
    pFie.notice = true;
    mainNpc = &pFie;
    
    currentMap->addRoom(Map::CRoom);
    currentMap->addRoom(Map::AClassroom);
    currentMap->addRoom(Map::KMITL);
    currentMap->addRoom(Map::Seven);
    
    bool inDay =  true;
    bool show = false;
    bool wait = false;
    bool recieveAjUkrit = false;
    bool findCat = false;
    bool doneAjUkrit = false;
    bool talkedPFie = false;
    
    while(inDay){
        sf::Event event;
        while (window->pollEvent(event)){
            if (event.type == sf::Event::Closed){
                inDay = false;
                window->close();
            }
        }
        
        if(mainNpc != nullptr){
            if(player->rect.getGlobalBounds().intersects(mainNpc->rect.getGlobalBounds())){
                col.play();
                player->colliding();
                mainNpc->talk();
            }
            else
                player->canMove();
            
            if(mainNpc->getName() == "pFie"){
                if(mainNpc->doneTalking){
                    ajUkrit.loadSpeech("days/d4/findCat.txt");
                    ajUkrit.notice = true;
                    mainNpc->doneTalking = false;
                }
            }
            
            if(mainNpc->getName() == "ajUkrit"){
                if(mainNpc->doneTalking and !recieveAjUkrit){
                    recieveAjUkrit = true;
                    ajUkrit.loadSpeech("days/d4/findCatShort.txt");
                    pFie.loadSpeech("days/d4/pFie2.txt");
                }
                if(mainNpc->doneTalking and findCat and !wait){
                    doneAjUkrit = true;
                    levelUp.play();
                    wait = true;
                    *day = 5;
                    player->point += 5;
                    timer.restart();
                }
            }
            
            if(mainNpc->getName() == "cat"){
                if(mainNpc->doneTalking and recieveAjUkrit){
                    ajUkrit.loadSpeech("days/d4/ajUkrit.txt");
                    ajUkrit.notice = true;
                    findCat = true;
                }
            }
        }
        
        if(!doneAjUkrit)
            ajUkrit.doneTalking = false;
        else
            if(timer.getElapsedTime() > sf::seconds(3.0f))
                inDay = false;
        
        for(auto obj: currentMap->solidObject)
            if(player->rect.getGlobalBounds().intersects(obj.getGlobalBounds())){
                col.play();
                player->colliding();
            }
        
        if(!player->rect.getGlobalBounds().intersects(currentMap->mapRect.getGlobalBounds())){
            col.play();
            player->colliding();
        }
        else
            player->canMove();
        
        for(auto& door: currentMap->doorsVec){
            if(player->rect.getGlobalBounds().intersects(door.doorRect.getGlobalBounds())){
                player->sprite.setPosition(door.getDestination());
                view->setCenter(player->sprite.getPosition());
                currentMap->changeMap(door.getRoom());
                
                if(currentMap->room == Map::AClassroom)
                    mainNpc = &ajUkrit;
                else if(currentMap->room == Map::CRoom)
                    mainNpc = &pFie;
                else if(currentMap->room == Map::KMITL)
                    mainNpc = &cat;
                else if(currentMap->room == Map::Seven)
                    mainNpc = &sevenGirl;
                else
                    mainNpc = nullptr;
                
                if(mainNpc != nullptr)
                    mainNpc->resetTalk();
                
                if(currentMap->room == Map::Seven)
                    enterSeven.play();
                else
                    enter.play();
            }
        }
        
        
        for(auto& dObj: currentMap->dynamicObj){
            if(player->rect.getGlobalBounds().intersects(dObj.mesRect.getGlobalBounds())){
                col.play();
                player->colliding();
                cout << player->getName() << ": " << dObj.getMessage() << endl << endl << endl << endl << endl << endl << endl << endl;
            }
            else
                player->canMove();
        }
        
        if(currentMap->room == Map::KMITL){
            window->setView(*view);
            view->setCenter(player->sprite.getPosition());
        }
        else
            window->setView(window->getDefaultView());
        
        window->clear();
        window->draw(currentMap->mapSprite);
        if(mainNpc != nullptr and currentMap->checkRoom(currentMap->room)){
            mainNpc->update();
            window->draw(mainNpc->sprite);
            markRect.setPosition(mainNpc->getX() + 70, mainNpc->getY() - 70);
            if(mainNpc->notice)
                window->draw(markRect);
        }
        
        if(timer.getElapsedTime() < sf::seconds(1.0f) and !doneAjUkrit)
            window->draw(text);
        
        player->update();
        window->draw(player->sprite);
        window->display();
    }
}

int day5(sf::RenderWindow* window, sf::View* view, TextureManager* texture, Map* currentMap, Player* player, int* day){
    currentMap->clearRoom();
    sf::Clock timer;
    sf::RectangleShape markRect;
    markRect.setTexture(texture->getTexture("redMark"));
    markRect.setSize(sf::Vector2f(64, 64));
    
    sf::SoundBuffer buff1, buff2, buff3, buff4;
    if(!buff1.loadFromFile(resourcePath() + "assets/sounds/level.ogg")) {return EXIT_FAILURE;}
    sf::Sound levelUp;
    levelUp.setBuffer(buff1);
    if(!buff2.loadFromFile(resourcePath() + "assets/sounds/door.ogg")) {return EXIT_FAILURE;}
    sf::Sound enter;
    enter.setBuffer(buff2);
    if(!buff3.loadFromFile(resourcePath() + "assets/sounds/seven.ogg")) {return EXIT_FAILURE;}
    sf::Sound enterSeven;
    enterSeven.setBuffer(buff3);
    if(!buff4.loadFromFile(resourcePath() + "assets/sounds/collision.ogg")) {return EXIT_FAILURE;}
    sf::Sound col;
    col.setBuffer(buff4);
    
    sf::Font font;
    if(!font.loadFromFile(resourcePath() + "assets/upheavtt.ttf")) {return EXIT_FAILURE;}
    sf::Text text("Day " + to_string(*day), font, 140);
    text.setFillColor(sf::Color::Black);
    text.setPosition(900, 500);
    
    NPC pFie(texture->getTexture("pFie"), "pFie", 460, 290, 70, 115);
    NPC michael(texture->getTexture("michael"), "michael", 720, 360);
    NPC ajKo(texture->getTexture("ajKo"), "ajKo", 570, 370, 85, 110);
    NPC sevenGirl(texture->getTexture("sevenGirl"), "sevenGirl", 345, 615, 70, 115);
    
    pFie.loadSpeech("days/d5/pFie1.txt");
    NPC* mainNpc = nullptr;
    pFie.notice = true;
    mainNpc = &pFie;
    michael.setFaceLeft();
    ajKo.setFaceLeft();
    
    currentMap->addRoom(Map::CRoom);
    currentMap->addRoom(Map::VRoom);
    currentMap->addRoom(Map::Lab);
    currentMap->addRoom(Map::Seven);
    
    bool inDay =  true;
    bool wait = false;
    bool talkedPFie = false;
    bool recieveMichael = false;
    bool talkedAjKo = false;
    bool doneMichael = false;
    
    while(inDay){
        sf::Event event;
        while (window->pollEvent(event)){
            if (event.type == sf::Event::Closed){
                inDay = false;
                window->close();
            }
        }
        
        if(mainNpc != nullptr){
            if(player->rect.getGlobalBounds().intersects(mainNpc->rect.getGlobalBounds())){
                col.play();
                player->colliding();
                mainNpc->talk();
            }
            else
                player->canMove();
            
            if(mainNpc->getName() == "pFie"){
                if(mainNpc->doneTalking and !talkedPFie){
                    talkedPFie = true;
                    michael.notice = true;
                    michael.loadSpeech("days/d5/enerygyLost.txt");
                }
            }
            
            if(mainNpc->getName() == "michael"){
                if(talkedPFie and !recieveMichael and mainNpc->doneTalking){
                    recieveMichael = true;
                    pFie.loadSpeech("days/d5/pFie2.txt");
                    michael.loadSpeech("days/d5/enerygyLostShort.txt");
                    ajKo.notice = true;
                    ajKo.loadSpeech("days/d5/ajKo.txt");
                }
                if(talkedAjKo and mainNpc->doneTalking and !doneMichael){
                    doneMichael = true;
                    levelUp.play();
                    *day = 6;
                    player->point += 5;
                    timer.restart();
                }
            }
            
            if(mainNpc->getName() == "ajKo"){
                if(mainNpc->doneTalking and recieveMichael and !talkedAjKo){
                    talkedAjKo = true;
                    michael.loadSpeech("days/d5/michael.txt");
                    michael.notice = true;
                }
            }
        }
        
        if(!doneMichael)
            michael.doneTalking = false;
        else
            if(timer.getElapsedTime() > sf::seconds(3.0f))
                inDay = false;
        
        for(auto obj: currentMap->solidObject)
            if(player->rect.getGlobalBounds().intersects(obj.getGlobalBounds())){
                col.play();
                player->colliding();
            }
        
        if(!player->rect.getGlobalBounds().intersects(currentMap->mapRect.getGlobalBounds())){
            col.play();
            player->colliding();
        }
        else
            player->canMove();
        
        for(auto& door: currentMap->doorsVec){
            if(player->rect.getGlobalBounds().intersects(door.doorRect.getGlobalBounds())){
                player->sprite.setPosition(door.getDestination());
                view->setCenter(player->sprite.getPosition());
                currentMap->changeMap(door.getRoom());
                
                if(currentMap->room == Map::VRoom)
                    mainNpc = &ajKo;
                else if(currentMap->room == Map::CRoom)
                    mainNpc = &pFie;
                else if(currentMap->room == Map::Lab)
                    mainNpc = &michael;
                else if(currentMap->room == Map::Seven)
                    mainNpc = &sevenGirl;
                else
                    mainNpc = nullptr;
                
                if(mainNpc != nullptr)
                    mainNpc->resetTalk();
                
                if(currentMap->room == Map::Seven)
                    enterSeven.play();
                else
                    enter.play();
            }
        }
        
        
        for(auto& dObj: currentMap->dynamicObj){
            if(player->rect.getGlobalBounds().intersects(dObj.mesRect.getGlobalBounds())){
                col.play();
                player->colliding();
                cout << player->getName() << ": " << dObj.getMessage() << endl << endl << endl << endl << endl << endl << endl << endl;
            }
            else
                player->canMove();
        }
        
        if(currentMap->room == Map::KMITL){
            window->setView(*view);
            view->setCenter(player->sprite.getPosition());
        }
        else
            window->setView(window->getDefaultView());
        
        window->clear();
        window->draw(currentMap->mapSprite);
        if(mainNpc != nullptr and currentMap->checkRoom(currentMap->room)){
            mainNpc->update();
            window->draw(mainNpc->sprite);
            markRect.setPosition(mainNpc->getX() + 70, mainNpc->getY() - 70);
            if(mainNpc->notice)
                window->draw(markRect);
        }
        
        if(timer.getElapsedTime() < sf::seconds(1.0f) and !doneMichael)
            window->draw(text);
        
        player->update();
        window->draw(player->sprite);
        window->display();
    }
}

int menu(sf::RenderWindow* window, TextureManager* texture, Player* player, int* day){
    sf::Clock timer;
    sf::RectangleShape rect, startBtn, loadBtn;
    rect.setSize(sf::Vector2f(1300, 1100));
    rect.setTexture(texture->getTexture("menu"));
    startBtn.setSize(sf::Vector2f(300, 100));
    startBtn.setTexture(texture->getTexture("startBtn"));
    startBtn.setPosition(270, 300);
    loadBtn.setSize(sf::Vector2f(550, 100));
    loadBtn.setTexture(texture->getTexture("loadBtn"));
    loadBtn.setPosition(150, 500);
    
    sf::SoundBuffer buff;
    if (!buff.loadFromFile(resourcePath() + "assets/sounds/press.ogg")) {
        return EXIT_FAILURE;
    }
    sf::Sound pressing;
    pressing.setBuffer(buff);

    int left = -255;
    int right = 1725;
    NPC ajUkrit(texture->getTexture("ajUkrit"), "ajUkrit", right, 850);
    NPC michael(texture->getTexture("michael"), "michael", right + 195, 870);
    NPC ajTui(texture->getTexture("ajTui"), "ajTui", right + 660, 900);
    NPC ajVeera(texture->getTexture("ajVeera"), "ajVeera", right + 885, 960);
    NPC ajMonde(texture->getTexture("ajMonde"), "ajMonde", right + 1035, 920);
    NPC man1(texture->getTexture("man1"), "man1", right - 420, 890);

    NPC pFie(texture->getTexture("pFie"), "pFie", left, 900, 70, 115);
    NPC drV(texture->getTexture("drV"), "drV", left - 570, 920);
    NPC ajSun(texture->getTexture("ajSun"), "ajSun", left - 1050, 950);
    NPC ajKo(texture->getTexture("ajKo"), "ajKo", left - 1200, 860, 85, 110);
    NPC jochen(texture->getTexture("jochen"), "jochen", left - 915, 900, 85, 110);
    NPC seven(texture->getTexture("sevenGirl"), "sevenGirl", left - 390, 950, 70, 115);
    NPC man2(texture->getTexture("man2"), "man2", left - 150, 880);

    vector<NPC*> npcVec;
    npcVec.push_back(&ajUkrit);
    npcVec.push_back(&ajKo);
    npcVec.push_back(&michael);
    npcVec.push_back(&man2);
    npcVec.push_back(&man1);
    npcVec.push_back(&ajTui);
    npcVec.push_back(&jochen);
    npcVec.push_back(&pFie);
    npcVec.push_back(&drV);
    npcVec.push_back(&ajMonde);
    npcVec.push_back(&ajSun);
    npcVec.push_back(&seven);
    npcVec.push_back(&ajVeera);


    bool menu = true;
    man2.walkX(left);
    seven.walkX(right);
    man1.walkX(right);
    ajUkrit.walkX(right);
    michael.walkX(right);
    pFie.walkX(left);
    drV.walkX(left);
    ajTui.walkX(right);
    ajSun.walkX(left);
    ajKo.walkX(left);
    ajVeera.walkX(right);
    ajMonde.walkX(right);
    jochen.walkX(left);
    bool exit = false;

    while (menu)
    {
        if(exit)
            if(timer.getElapsedTime() > sf::seconds(1.5f))
                menu = false;
        sf::Event event;
        while (window->pollEvent(event)){
            if(event.type == sf::Event::Closed){
                menu = false;
                window->close();
            }
            if(event.type == sf::Event::MouseButtonPressed){
                if(startBtn.getGlobalBounds().contains(window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)))){
                    *day = 1;
                    pressing.play();
                    timer.restart();
                    exit = true;
                }
                if(loadBtn.getGlobalBounds().contains(window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)))){
                    pressing.play();
                    loadGame(player, day);
                    pressing.play();
                    timer.restart();
                    exit = true;
                }
            }
            
            if(event.type == sf::Event::MouseMoved){
                if(startBtn.getGlobalBounds().contains(window->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)))){
                    startBtn.setTexture(texture->getTexture("startPBtn"));
                }
                else
                    startBtn.setTexture(texture->getTexture("startBtn"));
                if(loadBtn.getGlobalBounds().contains(window->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)))){
                    loadBtn.setTexture(texture->getTexture("loadPBtn"));
                }
                else
                    loadBtn.setTexture(texture->getTexture("loadBtn"));
            }
        }
        
        window->clear();
        window->draw(rect);
        window->draw(startBtn);
        window->draw(loadBtn);

        for(auto& npc : npcVec){
            npc->update();
            window->draw(npc->sprite);
            if(npc->getX() == left)
                npc->walkX(right);
            if(npc->getX() == right)
                npc->walkX(left);
        }
        window->display();
    }
}

string calGrade(int d, int p){
    if(d == 0 or p == NULL)
        return "X";
    int re = (p/(5*d)) * 100;
    if(re >= 80)
        return "A";
    else if (80 > re and re >= 70)
        return "B";
    else if (70 > re and re >= 60)
        return "C";
    else if(60 > re and re >= 50)
        return "D";
    else if(re < 50)
        return "F";
}

int main(){
    int day = 1;
    bool passedFirstSemester = false;
    sf::RenderWindow window(sf::VideoMode(1300, 1100), "Don't Get Retired");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(8);
    sf::Image icon;
    if(!icon.loadFromFile(resourcePath() + "assets/icon.png")){
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    sf::Music music;
    if (!music.openFromFile(resourcePath() + "assets/NewBarkTown.ogg")) {
        return EXIT_FAILURE;
    }
    music.play();
    
    sf::View view(sf::FloatRect(0, 0, 1890, 1753));
    view.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    TextureManager texture;
    texture.loadTexture();
    Map currentMap(&texture);
    Player player(texture.getTexture("man1"), "player", 1045, 950);
    
    menu(&window, &texture, &player, &day);
    
    window.setView(view);
    view.setCenter(player.sprite.getPosition());
    while(window.isOpen()){
        cout << "\n\n\n\n\n\n\n\n";
        player.sprite.setPosition(1045, 950);
        currentMap.changeToKMITL();
        cout << "Day: " << day << "\n\n\n\n\n\n";
        switch(day){
            case 1:
                day1(&window, &view, &texture, &currentMap, &player, &day);
                break;
            case 2:
                day2(&window, &view, &texture, &currentMap, &player, &day);
                break;
            case 3:
                day3(&window, &view, &texture, &currentMap, &player, &day);
                break;
            case 4:
                day4(&window, &view, &texture, &currentMap, &player, &day);
                break;
            case 5:
                day5(&window, &view, &texture, &currentMap, &player, &day);
                break;
            case 6:
                passedFirstSemester = true;
                window.close();
                break;
            default:
                day1(&window, &view, &texture, &currentMap, &player, &day);
                break;
        }
    }
    string Gpa = calGrade(day - 1, player.point);
    
    window.close();
    if(passedFirstSemester){
        cout << "Time flies..." << endl;
        cout << "You have passed through the first semester." << endl;
        cout << "Your GPA is now calculating..." << endl;
        cout << "1 semester GPA: " << Gpa << endl;
        if(Gpa == "F"){
            cout << "Unfortunately, your GPA is below the expectation..." << endl;
            cout << "I'm sorry to tell you that you will have to with drawl from the college...\n\n";
        }
        else{
            cout << "Congratulation!! you can survive the first semester." << endl;
            cout << "Stay tune for the next semester and see whether you can survive or not!" << endl;
            cout << "see you soon!!\n\n";
        }
    }
    else{
        if(Gpa != "X"){
            cout << "Day: " << day << "   Point: " << player.point << endl;
            cout << "Current GPA: " <<Gpa << endl;
        }
    }

    saveGame(player.point, day);
    
    return EXIT_SUCCESS;
}
