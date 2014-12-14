#include "ShortResponse.hpp"

namespace Packet {
  uint16_t ShortResponse::headerNumber = static_cast<uint16_t>(APacket::PacketType::SHORTRESPONSE);

  ShortResponse::ShortResponse()
  : APacket(PacketType::SHORTRESPONSE), _response(0)
  {

  }

  ShortResponse::ShortResponse(uint8_t response)
  : APacket(PacketType::SHORTRESPONSE), _response(response)
  {

  }

  std::string ShortResponse::to_bytes_body() const
  {
    std::string ret = "";
    fill_bytes(ret, _response);
    return ret;
  }

  void ShortResponse::from_bytes_body(const std::string &bytes)
  {
    std::size_t pos = 1;
    get_bytes(bytes, pos, _response);
  }

  uint8_t ShortResponse::getResponse() const
  {
    return _response;
  }

}