//
// Created by MLG Erwich on 29/01/2023.
//

#ifndef CHATROOM_CLIENT_H
#define CHATROOM_CLIENT_H

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <atomic>
#include <thread>
#include "CustomPacket.h"

class Client
{
 public:
  Client(sf::Window& window);

  void initialConnect();
  void serverConnect(int server_port);
  void connect(int room_num);
  void input(sf::Event event);
  void run();

  void click_input(sf::Event event, sf::RenderWindow& window);

  void render(sf::RenderWindow& window);

  std::atomic<bool> running = false;
  std::atomic<bool> connected = false;

 private:
  bool in_server;
  bool in_room;

  std::unique_ptr<sf::TcpSocket> socket;

  std::string message_text;
  sf::Text message;
  sf::Font font;

  std::vector<std::string> msg_history;
  std::vector<std::unique_ptr<sf::Text>> text_history;

  int msg_number;

  sf::RectangleShape chat_box;
  sf::RectangleShape history_box;

  sf::RectangleShape room_box;
  sf::RectangleShape room_box_2;

  sf::RectangleShape server_box;
  std::vector<std::unique_ptr<std::pair<sf::RectangleShape, int>>> server_list;
  void setupServerListItem(sf::RectangleShape& list_item);
  static bool in_box(sf::RectangleShape& box, sf::Vector2i mouse_pos);

  sf::Packet packet;
  sf::Vector2f window_size;

  std::string name;
};

#endif // CHATROOM_CLIENT_H
