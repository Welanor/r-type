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

  std::cout << "Onread" << std::endl;
  if (nbRead == 0)
    {
      std::cout << "Unregister client" << std::endl;
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
          if (!ret)
            _readBuff.rollbackReadBuffer(headerSize);
        }
      else
        {
          _readBuff.rollbackReadBuffer(headerSize - 1);
          std::cout << "Unknown Packet: " << std::hex << (int)(buff[0]) << (int)(buff[1]) << std::dec << std::endl;
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
  size_t readSize = 200;
  Packet::Handshake hand;
  size_t  nbUsed;
std::cout << "handshake" << std::endl;
  try {
      Network::Buffer buff;
      Network::Buffer tmpBuff(data);
      _readBuff.readBuffer(buff, readSize);
      readSize = buff.size();
      tmpBuff += buff;
      nbUsed = hand.from_bytes(tmpBuff);
      _readBuff.rollbackReadBuffer(readSize - nbUsed);
      _login = hand.getLogin();
      _protoVersion = hand.getProtocolVersion();
      std::cout << _login << " " << _protoVersion << std::endl;
    }
  catch (std::exception& e)
    {
    	std::cout << "fail shaking hands!" << std::endl;
      _readBuff.rollbackReadBuffer(readSize);
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
