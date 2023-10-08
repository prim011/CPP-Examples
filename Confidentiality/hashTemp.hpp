#pragma once

#include "libUsers.hpp"

class HashFunctions {
public:
  virtual ~HashFunctions () = default;

  virtual string formHash(size_t, LibAccess*) = 0;
};
