#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <memory>
#include <string>
#include <map>

#define PROTOCOLEVERSION 0x01000000

#include "APacket.hpp"
#include "ClientHelper.hpp"
#include "ABasicSocket.hpp"
#include "RingBuffer.hpp"

class Server;

class Client : public Network::SocketClientHelper, public std::enable_shared_from_this<Client>
{
private:
  static std::map<Packet::APacket::PacketType, size_t (Client::*)(const Network::Buffer&)> _netWorkBinds;

public:
  Client(const std::shared_ptr<Network::ABasicSocket>& sock, Server& serv);
  ~Client() = default;

  void sendPacket(const Packet::APacket& pack);

protected:
  void onRead(size_t readSize) override;
  void onWrite(size_t writeSize) override;
  void onDisconnet() override;

private:
  size_t netShortResponse(const Network::Buffer& data);
  size_t netHandshake(const Network::Buffer& data);
  size_t netAskListRoom(const Network::Buffer& data);
  size_t netCreateRoom(const Network::Buffer& data);
  size_t netJoinRoom(const Network::Buffer& data);
  size_t netStartGame(const Network::Buffer& data);
  size_t netLeaveRoom(const Network::Buffer& data);
  size_t netMessage(const Network::Buffer& data);

private:
  Server& _server;

  std::string _login;
  int		  _protoVersion;
  int		  _currentRoom;
};

#endif
