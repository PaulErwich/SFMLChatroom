//
// Created by MLG Erwich on 09/02/2023.
//

#ifndef CHATROOM_CHATROOM_H
#define CHATROOM_CHATROOM_H

#include <SFML/Network.hpp>
#include <thread>
#include <mutex>
#include <iostream>

class Chatroom
{
 public:
  void init();
  void listen();
};

#endif // CHATROOM_CHATROOM_H
