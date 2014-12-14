#ifndef _CREATEROOM_HPP_
# define _CREATEROOM_HPP_

# include <string>
# include <memory>
# include "APacket.hpp"
# include "Room.hpp"

namespace Packet {
  class CreateRoom: public APacket
  {
  private:
  	static uint16_t headerNumber;

  public:
    CreateRoom();
    CreateRoom(const t_room &);

    const std::shared_ptr<t_room>  &getRoom() const;

  private:
    std::string to_bytes_body() const override;
    std::size_t from_bytes_body(const std::string &bytes) override;
    uint16_t getHeaderNumber() const override {return headerNumber;};

    std::shared_ptr<t_room>  _room;
  };
};

#endif