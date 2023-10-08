#ifndef __LIBUSERS_H__
#define __LIBUSERS_H__

#include <iostream>
#include "util.hpp"

using namespace std;

/**
 * @brief Abstract base class defining access to the Library
 *
 * Define the abstract base class identify from which  Students 
 * and Professors classes will be derived.
 * This identity class will be the right permission to access the
 * library
 */
class LibAccess {
protected:
  size_t seed;
#ifdef AES_ENCRYPT
  string aes_key;
#endif
  
public:
  // virtual methods (to be implemented by derived classes)
  // Assuming this is all we need to (digitally)
  // identify Students and Professors, alike
  virtual string getName () = 0;
  virtual bool getPermit () = 0;
  virtual int getSerialN () = 0;
  virtual string getHash () = 0;
  virtual void commissionClass (size_t, string) = 0;
  
  // printing out method specific for this class
  friend ostream& operator<< (ostream& os, LibAccess &l)
  {
    os << "\tUser name : " << l.getName();
    os << "\tSerial Number: " << l.getSerialN();
    os << "\tactive: ";
    if (l.getPermit()) {
      os << "Yes" << endl;
    } else {
      os << "No" << endl;
    }
    os << " Hash : " << l.getHash ();
    return os;
  } 
};

 

#endif // __LIBUSERS_H__
