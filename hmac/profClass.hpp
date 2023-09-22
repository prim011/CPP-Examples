#include "libUsers.hpp"
#include "hashTemp.hpp"
#include "hmac.hpp"

using namespace std;

static int profSerialNum = 0;


/**
 * @brief Professors Class
 * 
 * This class contains all the information related
 * to Professors. It has been derived from the abstract
 * class LibAccess, which defines the interface to
 * the University Library as access premises, and 
 * from which it will need to implement four methods.
 * The class has been extended with a dedicated Hash 
 * function
 */
class Prof: private LibAccess {
  string name;
  int    serialNP;
  bool   active;
  size_t seed;
  HashFunctions *pHash;

  // Custom specialization hash 
  size_t formHash() {
    return pHash->formHash(seed, (LibAccess*) this);
  }

public:
  Prof () { serialNP = 0; active = false;
            seed = 0; pHash = NULL; name=""; }
  Prof (size_t sd, string s): name(s),
			      serialNP(++profSerialNum),
			      active(true), seed(sd) {
    pHash = (HashFunctions*) new HMAC_SH3();
  }
  Prof (size_t sd, string s, HashFunctions* & p): name(s),
						  serialNP(++profSerialNum),
						  active(true), seed(sd) {
    pHash = p;
  }
  ~Prof () { delete pHash; }
  
  // implementation of abstract methods
  string getName() { return name; }   
  bool getPermit () { return active; }
  int  getSerialN () { return serialNP; }  
  size_t getHash () { return formHash(); }

  void operator= (Prof &b) {
    name = b.name;
    serialNP = b.serialNP;
    active = b.active;
  }

  friend ostream& operator<< (ostream &os, Prof &P) {
    os << " Name : " << P.getName () << endl;
    os << " Serial Number : " << P.getSerialN() << endl; 
    os << " Active member: ";
    if (P.getPermit())
      os << "Yes" << endl;
    else
      os << "No" << endl;

    os << " Hash : " << P.getHash();
    return os;
  }
};
