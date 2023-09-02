#include <iostream>

using namespace std;

static int masterSerialNum = 0;

// Define the base class identify which will be a derived class from
// Students and from Professors

// That identity class will be the right permission to access the
// library

class LibAccess {
public:
  virtual string getName () = 0;
  virtual bool getPermit () = 0;
  virtual int getSerialN () = 0;
  
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
    cout << "oevrlaod of = operator\n";
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
    cout << "oevrlaod of = operator in Prof\n";
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
