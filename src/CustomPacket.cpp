//
// Created by MLG Erwich on 14/02/2023.
//

#include "CustomPacket.h"

#include <utility>

CustomPacket::CustomPacket()
{
  packet_type = PacketType::NONE;
  packet_sender = PacketSender::CLIENT;
}

CustomPacket::CustomPacket(PacketSender _packet_sender) : packet_sender(_packet_sender)
{
  packet_type = PacketType::NONE;
}

CustomPortPacket::CustomPortPacket() { port = 53000; packet_type = PacketType::PORT; }
CustomPortPacket::CustomPortPacket(PacketSender _packet_sender, int _port) : CustomPacket(_packet_sender), port(_port)
{
  packet_type = PacketType::PORT;
}

CustomMessagePacket::CustomMessagePacket()
{
  sender_port = 0;
}

CustomMessagePacket::CustomMessagePacket(
  PacketSender sender_type, int port, std::string  message) : CustomPacket(sender_type), sender_port(port), message(std::move(message))
{
  packet_type = PacketType::MESSAGE;
}

sf::Packet& operator <<(sf::Packet& packet, const CustomPacket& data)
{ return packet << data.packet_type << data.packet_sender; }

sf::Packet& operator >>(sf::Packet& packet, CustomPacket& data)
{ return packet >> data.packet_type >> data.packet_sender; }

sf::Packet& operator <<(sf::Packet& packet, const CustomPortPacket& data)
{ return packet << data.packet_type << data.packet_sender << data.port; }

sf::Packet& operator >>(sf::Packet& packet, CustomPortPacket& data)
{ return packet >> data.packet_type >> data.packet_sender >> data.port; }

sf::Packet& operator <<(sf::Packet& packet, const CustomMessagePacket& data)
{ return packet << data.packet_type << data.packet_sender << data.sender_port << data.message ; }

sf::Packet& operator >>(sf::Packet& packet, CustomMessagePacket& data)
{ return packet >> data.packet_type >> data.packet_sender >> data.sender_port >> data.message ; }

sf::Packet& operator <<(sf::Packet& packet, const PacketSender& sender)
{
  int temp = 0;
  switch (sender)
  {
    case PacketSender::SERVER:
      temp = 0;
      break;
    case PacketSender::CLIENT:
      temp = 1;
      break;
    case PacketSender::MEGA_SERVER:
      temp = 2;
      break;
  }

  return packet << temp;
}

sf::Packet& operator >>(sf::Packet& packet, PacketSender& sender)
{
  int temp = 0;
  packet >> temp;

  switch (temp)
  {
    case 0:
      sender = PacketSender::SERVER;
      break;
    case 1:
      sender = PacketSender::CLIENT;
      break;
    case 2:
      sender = PacketSender::MEGA_SERVER;
      break;
    default:
      sender = PacketSender::CLIENT;
      break;
  }

  return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const PacketType& packet_type)
{
  int temp = 0;
  switch (packet_type)
  {
    case PacketType::MESSAGE:
      temp = 1;
      break;
    case PacketType::PORT:
      temp = 2;
      break;
    default:
      temp = 0;
  }

  return packet << temp;
}

sf::Packet& operator >>(sf::Packet& packet, PacketType& packet_type)
{
  int temp = 0;
  packet >> temp;

  switch (temp)
  {
    case 1:
      packet_type = PacketType::MESSAGE;
      break;
    case 2:
      packet_type = PacketType::PORT;
      break;
    default:
      packet_type = PacketType::NONE;
      break;
  }

  return packet;
}

