//igpo
// Created by MLG Erwich on 29/01/2023.
//

#include "Server.h"

void Server::init()
{
  if (listener == nullptr) listener = std::make_unique<sf::TcpListener>();

  if (socket == nullptr) socket = std::make_unique<sf::TcpSocket>();

  // bind listener to a port

  //if (listener->listen(53000) != sf::Socket::Done)
  {
    // error...
  }
}

void Server::run()
{
  while (running)
  {
    std::cout << "Started running\n";
    // add pointer for new_connection and get reference to it
    std::pair<sf::TcpSocket, int>& cSock = connections.emplace_back(std::make_unique<std::pair<sf::TcpSocket, int>>()).operator*();

    // when we get a new_connection (Socket)
    // if the new_connection is invalid - return

    if (listener->accept(cSock.first) != sf::Socket::Done)
    {
      // don't add this client & perform any incorrect connection actions
      // remove the invalid connection

      connections.pop_back();
      return;
    }

    std::cout << "Client connected @ " << cSock.first.getRemotePort() << std::endl;

    bool room_select = true;

    int room_number[8];
    std::size_t received;

    while (room_select)
    {
      auto status = cSock.first.receive(room_number, 8, received);

      if (status == sf::Socket::Done)
      {
        if (room_number[0] == 1)
        {
          cSock.second = 1;
        }
        else if (room_number[0] == 2)
        {
          cSock.second = 2;
        }
        room_select = false;
      }
      //std::cout << room_number << std::endl;
    }

    workers.emplace_back([&]{
                           listen(cSock);
      // we know the socket disconnected, so remove it from the list
      std::cout << "detected disconnect\n";
      std::lock_guard<std::mutex>lck(mutex); // guard access to socket list

      for (int i = 0; i < connections.size(); i++)
      {
        // check if the connection matches else move onto the next one
        if (connections[i]->first.getRemotePort() == cSock.first.getRemotePort())
        {
          // erase the i-th element of the connections (the correct one)
          connections.erase(std::next(connections.begin(), i));

          // we've changed the vector, so we need to break out of the loop
          break;
        }
      }
                         });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));



    // add new_connection to connections
    // set up the connection for listening & sending
  }
}

void Server::connectMegaServer()
{
  if (socket == nullptr) return;

  if (socket->connect("127.0.0.1", 53000) == sf::Socket::Status::Done)
  {
    std::cout << "Connected to MegaServer\n";

    CustomPacket initial_packet(PacketSender::SERVER);
    sf::Packet packet;
    packet << initial_packet;
    socket->send(packet);

    std::thread setup_thread ([&]{ setupListener(); run(); });
    setup_thread.detach();
  }
  else
  {
    std::cout << "No MegaServer\n";
  }

}

void Server::setupListener()
{
  bool receiving_port = true;
  CustomPortPacket packet_data;
  sf::Packet packet;

  while (receiving_port)
  {
    auto status = socket->receive(packet);

    if (status == sf::Socket::Status::Done)
    {
      packet >> packet_data;
      receiving_port = false;
      std::cout << "Received server port\n";
    }
  }

  if (listener->listen(packet_data.port) != sf::Socket::Done)
  {
    // error
    std::cout << "Port not open\n";
  }
  else
  {
    std::cout << "Listening to port " << listener->getLocalPort() << "\n";
  }

  connected_to_mega = true;
}

void Server::listen(std::pair<sf::TcpSocket, int>& cSocket)
{
  bool continue_receiving = true;

  sf::Packet packet;
  CustomMessagePacket msg_data;

  while (continue_receiving)
  {
    // is there data ready for us?
    auto status = cSocket.first.receive(packet);

    if (status == sf::Socket::Status::Disconnected)
    {
      continue_receiving = false; // client disconnected
      std::cout << "Disconnected " << std::endl;
    }

    packet >> msg_data;

    // add a null terminator, and print as string
    //if (received < 1028) { data[received] = '\0';}
    send(packet, cSocket);
    std::string test;
    /*switch (msg_data.packet_sender)
    {
      case PacketSender::CLIENT:
        test = "Client";
        break;
      case PacketSender::SERVER:
        test = "Server";
        break;
    }*/

    std::cout << msg_data.sender_port << " " << msg_data.message << " " << test << '\n';
  }
  cSocket.first.disconnect();
}

void Server::send(sf::Packet& packet, std::pair<sf::TcpSocket, int>& cSocket)
{
  //auto message = reinterpret_cast<char*>(buffer.data());

  std::lock_guard<std::mutex>lck(mutex);

  for (auto & connection : connections)
  {
    // stop messages being sent back to sender_port
    if (connection->first.getRemotePort() == cSocket.first.getRemotePort()) continue;

    if (connection->second == cSocket.second)
    {
      connection->first.send(packet);
    }
  }
}

unsigned short Server::getPort()
{
  return listener->getLocalPort();
}
