#ifndef _HANDSHAKE_HPP_
# define _HANDSHAKE_HPP_

# include <iostream>
# include <string>
# include "APacket.hpp"

namespace Packet {
  class Handshake : public APacket
  {
  private:
  	static uint16_t headerNumber;

  public:
    Handshake();
    Handshake(const std::string &, uint32_t protocoleVersion);


    uint32_t getProtocolVersion() const;
    const std::string &getLogin() const;

  private:
    std::string to_bytes_body() const override;
    void from_bytes_body(const std::string &bytes) override;
	uint16_t getHeaderNumber() const override {return headerNumber;};

    uint32_t      _protocolVersion;
    std::string   _login;
  };
};

#endif