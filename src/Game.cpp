
#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window, char server)
  : window(game_window), isServer(server)
{
  srand(time(NULL));
}

Game::~Game()
{

}

bool Game::init()
{
  if (isServer == '0')
  {
    server = std::make_unique<Server>();
    server->init();
    server->connectMegaServer();
    //server->run();
  }
  else if (isServer == '1')
  {
    client = std::make_unique<Client>(window);
  }
  else if (isServer == '2')
  {
    std::cout << "building mega server\n";
    mega_server = std::make_unique<MegaServer>();
    mega_server->init();
    mega_server->run();
  }

  return true;
}

void Game::update(float dt)
{

}

void Game::render()
{
  //rendering goes here
  //window.draw(message);

  if (client != nullptr)
  {
    client->render(window);
  }
}

void Game::mouseClicked(sf::Event event)
{
  //get the click position
  sf::Vector2i click = sf::Mouse::getPosition(window);
  if (client != nullptr)
  {
    client->click_input(event, window);
  }

}

void Game::keyPressed(sf::Event event)
{

}

void Game::writeMessage(sf::Event event)
{
  if (client != nullptr) client->input(event);

}
