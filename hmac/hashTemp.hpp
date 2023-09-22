#pragma once

#include "libUsers.hpp"

class HashFunctions {
public:
  virtual ~HashFunctions () = default;

  virtual size_t formHash(size_t, LibAccess*) = 0;
};
