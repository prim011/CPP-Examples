#include "libUsers.hpp"
#include "hashTemp.hpp"
#include "hmac.hpp"

using namespace std;

static int studentSerialNum = 0;


/**
 * @brief Student Class
 * 
 * This class contains all the information related
 * to Students. It has been derived from the abstract
 * class LibAccess, which defines the interface to
 * the University Library as access premises, 
 * and from which it will need to implement four 
 * methods. The class has been extended with a private
 * hash function.
 */
class Student: private LibAccess {
  string name;
  int    serialNS;
  bool   active;
  HashFunctions *pHash;

  // Custom specialization hash 
  string formHash() {
    return pHash->formHash(seed, (LibAccess*) this);
  }
 public:
  Student() { serialNS = 0; pHash = NULL;
              active = false; seed = 0; name=""; }
  Student (string s): name(s),
		      serialNS(++studentSerialNum),	             
		       active(true) {
    pHash = (HashFunctions*) new HMAC_SH3();
  }
  Student (string s,
	   HashFunctions * & p): name(s),
				 serialNS(++studentSerialNum),
				 active(true) {
    pHash = p;
  }
  ~Student() { delete pHash; }

  // implementation of abstract methods  
  string getName () { return name; }
  bool getPermit () { return active; }
  int  getSerialN () { return serialNS; }
  string getHash () { return formHash(); }

  void commissionClass (size_t k) {
    seed = k;
    cout << "\nStudentClass succesfully Commissioned \n";
  } 

  void operator= (const Student &b) {
    name = b.name;
    serialNS = b.serialNS;
    active = b.active;
  }
  friend ostream& operator<< (ostream &os, Student &S) {
    os << " Name : " << S.getName () << endl;
    os << " Serial Number : " << S.getSerialN() << endl;
    os << " Active member: ";
    if (S.getPermit())
      os << "Yes" << endl;
    else
      os << "No" << endl;

    os << " Hash : " << S.getHash ();
    return os;
  }
  
};

