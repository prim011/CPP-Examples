#include <iostream>

using namespace std;

static int masterSerialNum = 0;

/**
 * @brief Abstract base class defining access to the Library
 *
 * Define the abstract base class identify from which  Students 
 * and Professors classes will be derived.
 * This identity class will be the right permission to access the
 * library
 */
class LibAccess {
public:
  // virtual methods (to be implemented by derived classes)
  // Assuming this is all we need to (digitally)
  // identify Students and Professors, alike
  virtual string getName () = 0;
  virtual bool getPermit () = 0;
  virtual int getSerialN () = 0;
  
  // printing out method specific for this class
  friend ostream& operator<< (ostream& os, LibAccess &l)
  {
    os << "\tUser name :" << l.getName();
    os << "\tSerial Number: " << l.getSerialN();
    os << "\tactive: ";
    if (l.getPermit()) {
      os << "Yes" << endl;
    } else {
      os << "No" << endl;
    }
    return os;
  }  
};

/**
 * @brief Student Class
 * 
 * This class contains all the information related
 * to Students. It has been derived from the abstract
 * class LibAccess, which defines the interface to
 * the University Library as access premises, 
and 
 * from which it will need to implement three methods.
 */
class Student: private LibAccess {
  string name;
  int    serialNS;
  bool   active;
 public:
  Student() { serialNS = 0; active = false; name="";}
  Student (string s): name(s), serialNS(++masterSerialNum), active(true) {}
  // implementation of abstract methods  
  string getName () { return name; }
  bool getPermit () { return active; }
  int  getSerialN () { return serialNS; }

  void operator= (Student &b) {
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

    return os;
  }
  
};


/**
 * @brief Professors Class
 * 
 * This class contains all the information related
 * to Professors. It has been derived from the abstract
 * class LibAccess, which defines the interface to
 * the University Library as access premises, and 
 * from which it will need to implement three methods.
 */
class Prof: private LibAccess {
  string name;
  int    serialNP;
  bool   active;
 public:
  Prof () { serialNP = 0; active = false; name="";}
  Prof (string s): name(s), serialNP(++masterSerialNum), active(true) {}
  // implementation of abstract methods
  string getName() { return name; }   
  bool getPermit () { return active; }
  int  getSerialN () { return serialNP; }  

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

    return os;
  }
};
