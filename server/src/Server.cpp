#include "Server.hpp"

#include "GetListRoom.hpp"

Server::Server(const std::string& addr, const std::string& port)
  : _net(Network::NetworkFactory::createNetwork()),
    _lobbyListener(Network::NetworkFactory::createListenSocket(addr, port, Network::ASocket::SockType::TCP, true)),
    _threadPool(5)
{
  _lobbyListener->setAcceptorCallback(std::bind(&Server::acceptNewClient, this, std::placeholders::_1));
  _net->registerListener(_lobbyListener);
  std::cout << "Server listening on " << _lobbyListener->getListeningIpAddr() << ":"
            << _lobbyListener->getListeningPort() << std::endl;
}

void Server::run()
{
  while (1)
    _net->poll(true);
}

void Server::broadcastAPacket(const Packet::APacket& pack) const
{
  for (auto& i : _clients)
    {
      i->sendPacket(pack);
    }
}

void Server::broadcastRoomList() const
{
  Packet::GetListRoom glr(_lobby.roomLists());
  broadcastAPacket(glr);
}

void Server::unregisterClient(const std::shared_ptr<Client>& cli)
{
  _clients.erase(std::remove_if(_clients.begin(), _clients.end(),
  [&cli](std::shared_ptr<Client>& cl) -> bool {
    return (cli == cl);
  }), _clients.end());
}

void Server::acceptNewClient(const std::weak_ptr<Network::AListenSocket>& that)
{
  std::shared_ptr<Network::AListenSocket> listener = that.lock();
  std::shared_ptr<Network::ABasicSocket> nClientSock = listener->acceptClient();
  std::shared_ptr<Client> nclient(new Client(nClientSock, *this));

  _clients.push_back(nclient);
  _net->registerClient(nClientSock);
  std::cout << "New client: " << nClientSock->getRemoteIpAddr() << ":" << nClientSock->getRemotePort() << std::endl;
}

const std::unique_ptr<Network::AListenSocket>& Server::createNewGame(const ServerRoom& gameInfo)
{
  std::shared_ptr<ServerGame> game(new ServerGame(gameInfo, _lobbyListener->getListeningIpAddr()));

  _games.push_back(game);
  _threadPool.addTask(&ServerGame::run, game);
  return game->getNetworkInfo();
}
