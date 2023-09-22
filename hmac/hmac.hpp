#pragma once 

class HMAC_SH3: public HashFunctions {
public:
  size_t formHash(size_t seed, LibAccess *p) {
    return (seed ^=
	    hash<string>{}(p->getName()) +
	    hash<int>{}(p->getSerialN()) +
	    0x9e3779b9 + (seed<<6) + (seed>>2));
  }
};
