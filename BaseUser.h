#include <cstring>
#include <iostream>
#include <openssl/sha.h>
#include <ostream>
#include <unistd.h>

const int PW_BUFFER_SIZE = 4;
// Adjust based on system architecture if needed (e.g. 8 for 64-bit).
const int MEM_ADDRESS_SIZE = sizeof(void *);

class BaseUser {
public:
    BaseUser(const char *name, const char *pw) {
		strcpy(this->name, name);
		SHA256((const unsigned char *)pw, strlen(pw), this->password_hash);
    }

    BaseUser(int id, int cr, const char *n) {
		ID = id;
		credit = cr;
		strcpy(name, n);
    }

    virtual void gainAccess() { std::cout << "Access granted." << std::endl; }
    virtual void denyAccess() { std::cout << "Access denied." << std::endl; }
    int get_credit() { return this->credit; }
	void buy(int cost) { this->credit -= cost; }

    void checkAccess(const char *pw) {
		unsigned char hash[SHA256_DIGEST_LENGTH];
		SHA256((const unsigned char *)pw, strlen(pw), hash);

		if (memcmp(hash, password_hash, SHA256_DIGEST_LENGTH) == 0) {
		    gainAccess();
		} else {
		    denyAccess();
		}
    }

private:
    void set_credit(int credit) { this->credit = credit; }
	
    char name[100];
    unsigned char password_hash[SHA256_DIGEST_LENGTH];
    int ID;
    int credit;
};
