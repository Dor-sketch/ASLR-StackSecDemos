#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// --- UNSAFE GETS REPLACEMENT ---
char *unsafeGets(char *str) {
    char *ret = str;
    while (true) {
	int c = getchar();
	if (c == '\n' || c == EOF)
	    break;
	*str++ = c;
    }
    *str = '\0';
    return ret;
}

// Unsafe string copy that doesn't check for buffer overflows.
void unsafeCopy(char *dest, const char *src) {
    while (*src) {
	*dest++ = *src++;
    }
    *dest = '\0';
}

// --- UNSAFE TYPE COMPARISON ---
bool is_rich_user(BaseUser *user) {
	unsigned int bound = 1e3;
	return (user->get_credit() >= bound);
}