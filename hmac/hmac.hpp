#pragma once 

// uncomment the following only for debug purpose
//#define _HMAC_DEBUG_


#ifdef STD_HMAC
#include "hmac_sha256/hmac_sha256.h"
#include <iomanip>
#include <sstream>
#include <vector>

#define SHA256_HASH_SIZE 8
#endif // STD_HMAC

class HMAC_SH3: public HashFunctions {
public:
  string formHash(size_t seed, LibAccess * p) {

    if (p == NULL)
      return 0;
    
#ifdef _HMAC_DEBUG_    
    cout << "Key: " << seed
	 << "\tName: " << p->getName()
	 << "\tSerial #: " << p->getSerialN() << endl;
#endif // _HMAC_DEBUG_

#ifdef STD_HMAC
    const string str_data = p->getName() + to_string(p->getSerialN());
    const string str_key = to_string(seed);
    vector<uint8_t> out(SHA256_HASH_SIZE);
    size_t tmp{0};
    stringstream ss_result;

    // Call hmac-sha256 function
    hmac_sha256(str_key.data(), str_key.size(),   // key
		str_data.data(), str_data.size(), // data to digest
		out.data(), out.size());          // output

    // Convert `out` to string with std::hex
    for (uint8_t x : out) {
      ss_result << std::hex << std::setfill('0') << std::setw(2) << (int)x;
    }

    ss_result >> tmp;
    
#ifdef _HMAC_DEBUG_    
    cout << "HMAC: " << tmp << endl;
#endif
    return ss_result.str();

#else // STD_HMAC

    return (to_string( seed ^=
	    hash<string>{}(p->getName()) +
	    hash<int>{}(p->getSerialN()) +
		       0x9e3779b9 + (seed<<6) + (seed>>2) ));
#endif // STD_HMAC
  }
};
