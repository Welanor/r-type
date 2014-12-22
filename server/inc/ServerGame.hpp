#ifndef SERVERGAME_H
#define SERVERGAME_H

#include <vector>
#include <chrono>
#include <thread>
#include <memory>

#include "ClientGame.hpp"
#include "ServerRoom.hpp"
#include "Packet/APacket.hpp"
#include "NetworkFactory.hpp"

class ServerGame
{
public:
  ServerGame(const ServerRoom& gameInfo, const std::string& port);
  virtual ~ServerGame() = default;

  void run();

  const std::string& listeningPort() const {return _listeningPort;};

  void broadcastPacket(const Packet::APacket& pack) const;
  void broadcastPacketToOther(const Packet::APacket& pack, const std::shared_ptr<ClientGame>& sender) const;
protected:
  void joinGame(const std::weak_ptr<Network::AListenSocket>& that,
                const std::shared_ptr<Network::Identity>& id, const Network::Buffer& data);

protected:
  bool									               _runGame;
  std::unique_ptr<Network::ANetwork>       			   _net;
  std::vector<std::shared_ptr<Network::AListenSocket>> _udpListener;
  std::vector<std::shared_ptr<ClientGame>>             _clients;
  std::string 										   _listeningPort;
};

#endif // SERVERGAME_H
