#ifndef LOBBY_H
#define LOBBY_H

#include <memory>
#include <atomic>
#include <map>
#include <vector>

#include "Client.hpp"
#include "Room.hpp"
#include "ServerRoom.hpp"

class Lobby
{
public:
  Lobby();
  virtual ~Lobby() = default;

  size_t newRoom(const t_room& r);
  std::vector<t_room> roomLists() const;
  const std::map<size_t, ServerRoom>& getRoomMap() const {return _rooms;};
  bool joinRoom(const std::shared_ptr<Client>& cli, size_t roomId);

private:
  size_t					   _roomId;
  std::map<size_t, ServerRoom> _rooms;
};

#endif // LOBBY_H
