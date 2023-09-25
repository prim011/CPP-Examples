#pragma once 

// uncomment the following only for debug purpose
//#define _HMAC_DEBUG_

class HMAC_SH3: public HashFunctions {
public:
  size_t formHash(size_t seed, LibAccess *p) {

    if (p == NULL)
      return 0;
    
#ifdef _HMAC_DEBUG_    
    cout << "Key: " << seed
	 << "\tName: " << p->getName()
	 << "\tSerial #: " << p->getSerialN() << endl;
#endif // _HMAC_DEBUG_
    
    return (seed ^=
	    hash<string>{}(p->getName()) +
	    hash<int>{}(p->getSerialN()) +
	    0x9e3779b9 + (seed<<6) + (seed>>2));
  }
};
