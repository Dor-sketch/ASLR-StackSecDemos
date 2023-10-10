#include <cstdlib>
#include <iostream>

int main(int argc, char const *argv[])
{
    int *p = (int *)malloc(sizeof(int));
    std::cout << "p address is = " << p << std::endl;
    free(p);
    return 0;
}