//#include "NPC.hpp"
//#include "Player.hpp"
//#include "Map.hpp"
//#include "TextureManager.hpp"
//#include <vector>
//
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(1300, 1100), "Don't Get Retired");
//    window.setVerticalSyncEnabled(true);
//    window.setFramerateLimit(8);
//
//    TextureManager textures;
//    textures.loadTexture();
//    
//    NPC ajTui(textures.getTexture("ajTui"), "ajTui", 550, 350);
//    Player player(textures.getTexture("man"), "man", 895, 365);
//    
//    Map map(textures.getTexture("AClass"), 300, 80);
//
//    ajTui.loadSpeech("ajTuiSpeech.txt");
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//
//            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
//                window.close();
//            }
//        }
//
//        if(player.rect.getGlobalBounds().intersects(ajTui.rect.getGlobalBounds())){
//            player.colliding();
//            ajTui.talk();
//        }
//        else
//            player.canMove();
//
//////////Update and draw
//        ajTui.update();
//        player.update();
//
//        window.clear();
//
//        window.draw(map.mapRect);
//        window.draw(ajTui.sprite);
//        window.draw(player.sprite);
//
//        window.display();
//    }
//
//    return EXIT_SUCCESS;
//}
//
