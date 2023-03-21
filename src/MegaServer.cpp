//
// Created by perwi on 10/02/2023.
//

#include "MegaServer.h"

void MegaServer::init()
{
  current_port = 53001;
  running = true;

  if (listener == nullptr) listener = std::make_unique<sf::TcpListener>();

  // Bind listener to an open port
  // First connection for clients
  if (listener->listen(53000) != sf::Socket::Done)
  {
    // error
  }
}

void MegaServer::run()
{
  while (running)
  {
    // reference for the new connection
    sf::TcpSocket& cSock = connections.emplace_back(std::make_unique<sf::TcpSocket>()).operator*();

    if (listener->accept(cSock) != sf::Socket::Done)
    {
      // don't add client and perform actions on it
      // remove invalid connection
      connections.pop_back();
      return;
    }

    std::cout << "Device connected @ " << cSock.getRemotePort() << "\n";

    //sendServerList(cSock);

    workers.emplace_back([&]{listen(cSock);});

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void MegaServer::listen(sf::TcpSocket& cSocket)
{
  bool continue_receiving = true;

  sf::Packet received;
  sf::Packet sending;
  CustomPacket empty_packet;
  CustomPortPacket port_packet;
  CustomMessagePacket message_packet;

  while (continue_receiving)
  {
    // is data ready for us?
    auto status = cSocket.receive(received);

    if (status == sf::Socket::Status::Disconnected)
    {
      continue_receiving = false; // client disconnected
      std::cout << "Disconnected " << std::endl;
      return;
    }

    received >> empty_packet;

    switch (empty_packet.packet_type)
    {
      case PacketType::NONE:
        switch (empty_packet.packet_sender)
        {
          case PacketSender::SERVER:
            port_packet.packet_sender = PacketSender::MEGA_SERVER;
            port_packet.port = current_port;
            sending << port_packet;
            cSocket.send(sending);
            connected_server_ports.emplace_back(current_port);
            current_port++;
            break;
          case PacketSender::CLIENT:
            sendServerList(cSocket);
            break;
          default:
            break;
        }
        break;
      case PacketType::MESSAGE:
        received >> message_packet;
        break;
      case PacketType::PORT:
        received >> port_packet;

        switch (port_packet.packet_sender)
        {
          case PacketSender::SERVER:

            break;
          case PacketSender::CLIENT:
            break;
          case PacketSender::MEGA_SERVER:
            break;
        }
        break;
    }

    // add a null terminator, and print as string
    //if ()

  }
}

void MegaServer::sendServerList(sf::TcpSocket& cSocket)
{
  for (auto & port : connected_server_ports)
  {
    CustomPortPacket port_packet(PacketSender::MEGA_SERVER, port);
    sf::Packet packet;
    std::cout << "port data " << port_packet.port << std::endl;
    packet << port_packet;

    if (cSocket.send(packet) == sf::Socket::Status::Done)
    {
      std::cout << "Sent server @ " << port_packet.port << "\n";
    }
  }
}