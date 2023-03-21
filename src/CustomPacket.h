//
// Created by MLG Erwich on 14/02/2023.
//

#ifndef CHATROOM_CUSTOMPACKET_H
#define CHATROOM_CUSTOMPACKET_H

#include <SFML/Network.hpp>

enum class PacketSender
{
  SERVER = 0,
  CLIENT = 1,
  MEGA_SERVER = 2
};

enum class PacketType
{
  NONE = 0,
  MESSAGE = 1,
  PORT = 2,
};

struct CustomPacket
{
  CustomPacket();
  CustomPacket(PacketSender _packet_sender);

  PacketType packet_type;
  PacketSender packet_sender;
};

struct CustomPortPacket : CustomPacket
{
  CustomPortPacket();
  CustomPortPacket(PacketSender _packet_sender, int _port);

  int port;
};

struct CustomMessagePacket : CustomPacket
{
  CustomMessagePacket();
  CustomMessagePacket(PacketSender sender_type, int port, std::string  message);

  int sender_port;
  std::string message;
};

sf::Packet& operator <<(sf::Packet& packet, const CustomPacket& data);
sf::Packet& operator >>(sf::Packet& packet, CustomPacket& data);

sf::Packet& operator <<(sf::Packet& packet, const CustomMessagePacket& data);
sf::Packet& operator >>(sf::Packet& packet, CustomMessagePacket& data);

sf::Packet& operator <<(sf::Packet& packet, const CustomPortPacket& data);
sf::Packet& operator >>(sf::Packet& packet, CustomPortPacket& data);

sf::Packet& operator <<(sf::Packet& packet, const PacketSender& sender);
sf::Packet& operator >>(sf::Packet& packet, PacketSender& sender);

sf::Packet& operator <<(sf::Packet& packet, const PacketType& packet_type);
sf::Packet& operator >>(sf::Packet& packet, PacketType& packet_type);

#endif // CHATROOM_CUSTOMPACKET_H
