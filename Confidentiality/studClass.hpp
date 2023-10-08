#include "libUsers.hpp"
#include "hashTemp.hpp"
#include "hmac.hpp"
#ifdef AES_ENCRYPT
#  include "AES/aes.hpp"
#endif // AES_ENCYRPT

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
#ifdef AES_ENCRYPT
  struct AES_ctx *ctx;
  string aes_iv;
#endif // AES_ENCYRPT

  // Custom specialization hash 
  string formHash() {
    return pHash->formHash(seed, (LibAccess*) this);
  }
  

public:
  Student(): name (""), serialNS(0), active(false), 
	     pHash(NULL) {
    seed = 0;
#ifdef AES_ENCRYPT     
    ctx = NULL; aes_iv = "";
#endif // AES_ENCYRPT 
  }
  Student (string s): name(s),
		      serialNS(++studentSerialNum),	             
		      active(true) {
    pHash = (HashFunctions*) new HMAC_SH3();
#ifdef AES_ENCRYPT
    ctx = new AES_ctx();
    aes_iv = to_string(rand());
#endif // AES_ENCYRPT 
  }
  Student (string s,
	   HashFunctions * & p): name(s),
				 serialNS(++studentSerialNum),
				 active(true) {
    pHash = p;
#ifdef AES_ENCRYPT
    ctx = new AES_ctx();
    aes_iv = to_string(rand());    
#endif // AES_ENCYRPT     
  }
  ~Student() {
    delete pHash;
#ifdef AES_ENCRYPT
    delete ctx;
#endif
  }

#ifdef AES_ENCRYPT
  // AES Init
  void initDataAtRest (string aes) {
    if (!ctx) {
          ctx = new AES_ctx();
	  aes_iv = to_string(rand());    
    }
      
    aes_key = aes;
    padTo (aes_key, AES_BLOCKLEN);
    padTo (aes_iv, AES_BLOCKLEN);
    const uint8_t* key = reinterpret_cast<const uint8_t*> (aes_key.c_str());
    const uint8_t* iv = reinterpret_cast<const uint8_t*> (aes_iv.c_str());
    AES_init_ctx_iv(ctx, key, iv);
  }
  // AES Encrypt routine
  void encryptDataAtRest() {
    if (!ctx)
      return;
    
    padTo (name, AES_BLOCKLEN);
    uint8_t* encryptIn = reinterpret_cast<uint8_t*> ((char*) name.c_str());
#   ifdef ECB
    AES_ECB_encrypt(ctx, encryptIn);
#   else    
    AES_CBC_encrypt_buffer(ctx, encryptIn, AES_BLOCKLEN);
#   endif    
    name = reinterpret_cast<char*> (encryptIn);    
    cout << "==> Encrypted: " << phex(encryptIn, AES_BLOCKLEN);
  }
  // AES Decrypt routine
  string decryptDataAtRest() {
    if (!ctx)
      return "";

    string rtn = name;
    padTo (rtn, AES_BLOCKLEN);
    uint8_t* decryptIn = reinterpret_cast<uint8_t*> ((char*) rtn.c_str());
#   ifdef ECB
    AES_ECB_decrypt(ctx, decryptIn);
#   else    
    AES_CBC_decrypt_buffer(ctx, decryptIn, AES_BLOCKLEN);
#   endif    
    rtn = reinterpret_cast<char*> (decryptIn);
    return rtn;
  }
#endif    

  // implementation of abstract methods  
  string getName () {
#ifdef AES_ENCRYPT
    return decryptDataAtRest();
#else
    return name;
#endif    
  }
  bool getPermit () { return active; }
  int  getSerialN () { return serialNS; }
  string getHash () { return formHash(); }

  void commissionClass (size_t k, string aes) {
    seed = k; 
#ifdef AES_ENCRYPT
    initDataAtRest(aes);
    encryptDataAtRest();
#endif // AES_ENCRYPT      
    cout << "\nStudent Class succesfully Commissioned \n";
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

