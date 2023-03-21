//
// Created by MLG Erwich on 29/01/2023.
//

#include "Client.h"

Client::Client(sf::Window& window)
{
  window_size = sf::Vector2f(window.getSize().x, window.getSize().y);
  socket = std::make_unique<sf::TcpSocket>();

  name = "Paul";

  in_room = false;
  in_server = false;

  font.loadFromFile("Data/Fonts/OpenSans-Bold.ttf");

  message.setFont(font);
  message.setCharacterSize(25);

  msg_number = 0;

  chat_box.setFillColor(sf::Color(66, 69, 73));
  chat_box.setSize(sf::Vector2f(window.getSize().x * 0.9f, window.getSize().y * 0.1f));
  chat_box.setPosition(sf::Vector2f(window.getSize().x * 0.05f, window.getSize().y - 100));

  message.setPosition(sf::Vector2f(chat_box.getPosition().x + 10, chat_box.getPosition().y +
                                                                    chat_box.getGlobalBounds().height / 8));

  history_box.setFillColor(sf::Color(40, 43, 48));
  history_box.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));

  room_box.setSize(sf::Vector2f(window.getSize().x / 4, window.getSize().y / 4));
  room_box.setFillColor(sf::Color::Red);

  room_box_2.setSize(sf::Vector2f(window.getSize().x / 4, window.getSize().y / 4));
  room_box_2.setPosition(sf::Vector2f(window.getSize().x / 2, 0));
  room_box_2.setFillColor(sf::Color::White);

  server_box.setSize(sf::Vector2f(window_size.x * 0.8f, window_size.y * 0.8f));
  server_box.setPosition(window_size.x * 0.1f, window_size.y * 0.1f);
  server_box.setFillColor(sf::Color(40, 43, 48));
  /*server_list.emplace_back(std::make_unique<sf::RectangleShape>());
  setupServerListItem(*server_list[0]);
  server_list.emplace_back(std::make_unique<sf::RectangleShape>());
  setupServerListItem(*server_list[1]);*/
  initialConnect();
}

void Client::setupServerListItem(sf::RectangleShape& list_item)
{
  list_item.setSize(sf::Vector2f(window_size.x * 0.7f, window_size.y * 0.1f));
  list_item.setPosition(window_size.x * 0.15, window_size.y * (0.12 * server_list.size()));
  list_item.setFillColor(sf::Color(sf::Color::Magenta));
}

void Client::initialConnect()
{
  if (socket == nullptr) return;

  if (socket->connect("127.0.0.1", 53000) == sf::Socket::Status::Done)
  {

    std::cout << "Connected to Mega Server\n";
    connected = true;
    sf::Packet sent_packet;
    CustomPacket packet_data(PacketSender::CLIENT);
    sent_packet << packet_data;
    socket->send(sent_packet);
    std::thread run_thread ([&]{run();});
    run_thread.detach();

  }
  else
  {
    std::cout << "Failed to connect to Mega Server\n";
  }
}

void Client::serverConnect(int server_port)
{
  std::cout << "trying to connect to " << server_port << std::endl;
  if (socket->connect("127.0.0.1", server_port) == sf::Socket::Status::Done)
  {
    std::cout << "Connected to server\n";
    std::cout << socket->getLocalPort() << std::endl;
    std::cout << socket->getRemotePort() << std::endl;
    in_server = true;
  }
  else
  {
    std::cout << "Failed to connect to server\n";
  }
}

void Client::connect(int room_num)
{
  // don't try connecting without a socket
  if (socket == nullptr) return;

  //if (socket->connect("127.0.0.1", 53000) == sf::Socket::Status::Done)
  {
    std::cout << "You're Connected!" << std::endl;

    socket->send(&room_num, 8);
    connected = true;
    std::thread run_thread ([&]{run();});
    run_thread.detach();
  }
  //else
  {
    //std::cout << "Failed to connect";
  }
}

void Client::input(sf::Event event)
{
  if (running)
  {
    // grab user input
    if (event.text.unicode == 13)
    {
      if (connected)
      {
        CustomMessagePacket message_packet(
          PacketSender::CLIENT, socket->getLocalPort(), name + " " + message_text);
        packet << message_packet;

        socket->send(packet);
        packet.clear();
        msg_history.emplace_back(message_text);
        sf::Text temp(message_text, font, 20);
        temp.setPosition(history_box.getGlobalBounds().width - temp.getGlobalBounds().width - 20,
                         temp.getGlobalBounds().height * msg_number + 10 * msg_number);
        text_history.emplace_back(std::make_unique<sf::Text>(temp));
        msg_number++;
        message_text.clear();
      }
    }
    else if (event.text.unicode == 8)
    {
      message_text.pop_back();
      message.setString(message_text);
    }
    else
    {
      message_text += event.text.unicode;
    }
    message.setString(message_text);
  }
}

void Client::run()
{
  running = true;
  // receive messages
  while (running && connected)
  {
    //char static_buffer[1028];
    sf::Text msg;
    sf::RectangleShape new_server_item;
    sf::Packet received_packet;
    sf::Packet copy_packet;
    CustomPacket packet_data;
    CustomMessagePacket msg_data;
    CustomPortPacket port_data;

    while (connected)
    {
      //std::size_t received;

      auto status = socket->receive(received_packet);
      copy_packet = received_packet;
      received_packet >> packet_data;
      if (status == sf::Socket::Status::Disconnected)
      {
        connected = false;
        std::cout << "clean disconnection" << std::endl;
        socket->disconnect();
        break;
      }
      else
      {
        switch (packet_data.packet_sender)
        {
          case PacketSender::SERVER:
            //test = "Server";
            break;
          case PacketSender::CLIENT:
            copy_packet >> msg_data;

            msg_history.emplace_back(std::to_string(msg_data.sender_port) + " " + msg_data.message);
            msg = sf::Text(std::to_string(msg_data.sender_port) + " " + msg_data.message, font, 20);
            msg.setPosition(20,
                             msg.getGlobalBounds().height * msg_number + 10 * msg_number);
            text_history.emplace_back(std::make_unique<sf::Text>(msg));

            msg_number++;
            break;
          case PacketSender::MEGA_SERVER:
            switch (packet_data.packet_type)
            {
              case PacketType::MESSAGE:
                break;
              case PacketType::PORT:
                copy_packet >> port_data;
                //sf::RectangleShape new_server_item;
                server_list.emplace_back(std::make_unique<std::pair<sf::RectangleShape, int>>());
                std::cout << "received port is " << port_data.port << "\n";
                server_list.back()->second = port_data.port;
                setupServerListItem(server_list.back()->first);
                std::cout << "Received port packet" << std::endl;

                break;
              default:
                break;
            }

            //test = "Mega Server";
            break;
        }



        // treat all data as string and append null terminator

        //if (received < 1028) static_buffer[received] = '\0';



        std::string test;


        std::cout << msg_data.message << " " << test << "\n";
      }
    }
  }
}

void Client::click_input(sf::Event event, sf::RenderWindow& window)
{
  sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
  if (event.mouseButton.button == sf::Mouse::Button::Left)
  {

    if (!in_room && in_server)
    {
      if (in_box(room_box, mouse_pos))
      {
        in_room = true;
        connect(1);
      }

      if (in_box(room_box_2, mouse_pos))
      {
        in_room = true;
        connect(2);
      }
    }

    if (!in_server)
    {
      for (auto& server : server_list)
      {
        if (in_box(server->first, mouse_pos))
        {
          socket->disconnect();

          serverConnect(server->second);

        }
      }
    }
  }
}

bool Client::in_box(sf::RectangleShape& box, sf::Vector2i mouse_pos)
{
  if (mouse_pos.x > box.getGlobalBounds().left && mouse_pos.x < box.getGlobalBounds().left + box.getGlobalBounds().width &&
      mouse_pos.y > box.getGlobalBounds().top && mouse_pos.y < box.getGlobalBounds().top + box.getGlobalBounds().height)
  {
    return true;
  }
  return false;
}

void Client::render(sf::RenderWindow& window)
{
  if (!in_server)
  {
    window.draw(server_box);
    for (auto& list_item : server_list)
    {
      window.draw(list_item->first);
    }
  }
  else
  {
    if (!in_room)
    {
      window.draw(room_box);
      window.draw(room_box_2);
    }
    else
    {
      window.draw(history_box);
      window.draw(chat_box);

      for (const std::unique_ptr<sf::Text>& text : text_history)
      {
        window.draw(*text);
      }

      window.draw(message);
    }
  }
}

