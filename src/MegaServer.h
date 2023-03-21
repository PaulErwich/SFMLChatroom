//
// Created by perwi on 10/02/2023.
//

#ifndef CHATROOM_MEGASERVER_H
#define CHATROOM_MEGASERVER_H

#include <SFML/Network.hpp>
#include <thread>
#include <iostream>
#include <mutex>
#include <utility>
#include "Server.h"
#include "CustomPacket.h"

class MegaServer
{
 public:
  void init();
  void run();
  void listen(sf::TcpSocket& cSocket);
  void sendServerList(sf::TcpSocket& cSocket);

 private:
  std::vector<std::thread> workers;
  std::vector<std::unique_ptr<sf::TcpSocket>> connections;

  std::vector<int> connected_server_ports;

  std::unique_ptr<sf::TcpListener> listener;

  std::mutex mutex;

  bool running;
  int current_port;
};

#endif // CHATROOM_MEGASERVER_H
