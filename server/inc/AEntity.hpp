#ifndef AENTITY_H
#define AENTITY_H

#include <memory>

#include "EntitiesTypes.hpp"

class AEntity
{
public:
  AEntity(int networkId, EntitiesTypes type);
  virtual ~AEntity() = default;

  int getNetId() const {return _networkId;};
  EntitiesTypes getType() const {return _type;};
protected:
  int           _networkId;
  EntitiesTypes _type;
};

#endif // AENTITY_H
