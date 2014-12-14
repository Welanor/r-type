#include "Client.hpp"

#include <initializer_list>
#include <type_traits>

#include "Server.hpp"

#include "Handshake.hpp"

std::map<Packet::APacket::PacketType, bool (Client::*)(const Network::Buffer&)> Client::_netWorkBinds =
{
  {Packet::APacket::PacketType::SHORTRESPONSE, &Client::netShortResponse},
  {Packet::APacket::PacketType::HANDSHAKE, &Client::netHandshake},
  {Packet::APacket::PacketType::GETLISTROOM, &Client::netGetListRoom},
  {Packet::APacket::PacketType::CREATEROOM, &Client::netCreateRoom},
  {Packet::APacket::PacketType::JOINROOM, &Client::netJoinRoom}
};

Client::Client(const std::shared_ptr<Network::ABasicSocket>& sock, Server& serv)
  : SocketClientHelper(sock), _server(serv)
{
}

void Client::onRead(size_t nbRead)
{
  const size_t headerSize = sizeof(uint16_t);
  Network::Buffer buff;
  Packet::APacket::PacketType pack;
  bool isPacket = false;

  if (nbRead == 0)
    {
      _server.unregisterClient(shared_from_this());
      return;
    }

  while (!isPacket && _readBuff.getLeftRead() >= headerSize)
    {
      _readBuff.readBuffer(buff, headerSize);
      pack = Packet::APacket::toPacketType(buff);
      if (pack != Packet::APacket::PacketType::UNKNOW)
        {
          isPacket = true;
          bool ret = (this->*_netWorkBinds.at(pack))(buff);
          // if (!ret)
          //handle not good packet
        }
    }
}

void Client::onWrite(size_t)
{
}

bool Client::netShortResponse(const Network::Buffer&)
{
  return false;
}

bool Client::netHandshake(const Network::Buffer& data)
{
  Packet::Handshake hand;
  size_t  nbUsed;

  try {
      hand.from_bytes(data);
      _login = hand.getLogin();
      _protoVersion = hand.getProtocolVersion();
    }
  catch (std::exception& e)
    {
      return false;
    }
  return true;
}

bool Client::netGetListRoom(const Network::Buffer&)
{
  return false;
}

bool Client::netCreateRoom(const Network::Buffer&)
{
  return false;
}

bool Client::netJoinRoom(const Network::Buffer&)
{
  return false;
}
