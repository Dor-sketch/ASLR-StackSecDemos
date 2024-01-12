#include <cstring>
#include <iostream>
#include <openssl/sha.h>
#include <ostream>
#include <unistd.h>


const int PW_BUFFER_SIZE = 4;
// Adjust based on system architecture if needed (e.g. 8 for 64-bit).
const int MEM_ADDRESS_SIZE = sizeof(void *);

class BaseUser {
private:
  char name[100];
  unsigned char password_hash[SHA256_DIGEST_LENGTH];

public:
  BaseUser(const char *name, const char *pw) {
    strcpy(this->name, name);
    SHA256((const unsigned char *)pw, strlen(pw), this->password_hash);
  }

  virtual void gainAccess() { std::cout << "Access granted." << std:: endl; }
  virtual void denyAccess() { std::cout << "Access denied." << std::endl; }

  void checkAccess(const char *pw) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char *)pw, strlen(pw), hash);
    if (memcmp(hash, password_hash, SHA256_DIGEST_LENGTH) == 0) {
      gainAccess();
    } else {
      denyAccess();
    }
  }
};
