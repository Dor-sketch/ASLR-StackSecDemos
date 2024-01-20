
#include "BaseUser.h"
#include "utils.h"

#include <cstring>

int main () {
    BaseUser John(1, 100, "John"); // John (ID 1) has 100 credits

    // John wants to buy a new car, but he doesn't have enough credits.
    printf("John's credit: %d\n", John.get_credit());

    // To get a new car, John needs to pass the credit check.
    printf("John's credit check: ");
    if (is_rich_user(&John)) {
        printf("Passed\n");
    } else {
        printf("Failed\n");
    }

    // but John is a hacker, he wants to get a new car without enough credits.
    // He tries to overflow the credit value - by spending all his credits + 1.
    // John's credit is an integer, which is 4 bytes.
    // John's credit is stored in the memory address of John + ... + 4 bytes.
    // John realize he can simply buy something that costs 101 credits.

    John.buy(101); // John buys something that costs 101 credits.
    printf("John's credit: %d\n", John.get_credit());

    // John's credit is now -1, which is 0xFFFFFFFF in hex.

    printf("John's credit check: ");
    if (is_rich_user(&John)) {
        printf("Passed\n");
    } else {
        printf("Failed\n");
    }

    int x = -1;
    unsigned int y = 1;
    if (x < y) {
        std::cout << "x is less than y" << std::endl;
    } else {
        std::cout << "x is greater than y" << std::endl;
    }

    return 0;
}