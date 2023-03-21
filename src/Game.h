
#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <SFML/Graphics.hpp>
#include "Client.h"
#include "Server.h"
#include "MegaServer.h"

class Game
{
 public:
  Game(sf::RenderWindow& window, char server);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void mouseClicked(sf::Event event);
  void keyPressed(sf::Event event);
  void writeMessage(sf::Event event);

 private:
  sf::RenderWindow& window;

  char isServer = '0';

  std::unique_ptr<Client> client = nullptr;
  std::unique_ptr<Server> server = nullptr;
  std::unique_ptr<MegaServer> mega_server = nullptr;
};

#endif // PLATFORMER_GAME_H
