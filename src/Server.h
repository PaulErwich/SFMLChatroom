//
// Created by MLG Erwich on 29/01/2023.
//

#ifndef CHATROOM_SERVER_H
#define CHATROOM_SERVER_H

#include <SFML/Network.hpp>
#include <thread>
#include <iostream>
#include <mutex>
#include <utility>
#include "CustomPacket.h"

class Server
{
 public:
  void init();
  void run();

  void connectMegaServer();

  void setupListener();

  void listen(std::pair<sf::TcpSocket, int>& cSocket);
  void send(sf::Packet& packet, std::pair<sf::TcpSocket, int>& cSocket);
  unsigned short getPort();

 private:
  std::vector<std::thread> workers;
  std::vector<std::unique_ptr<std::pair<sf::TcpSocket, int>>> connections;

  std::unique_ptr<sf::TcpSocket> socket;
  std::unique_ptr<sf::TcpListener> listener;

  std::mutex mutex;

  bool running = true;
  bool connected_to_mega = false;
};

#endif // CHATROOM_SERVER_H
