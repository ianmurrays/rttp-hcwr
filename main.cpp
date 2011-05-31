#include <iostream>

#ifdef _RTTP_TEST // Need to avoid declaring this when testing.
int main (int argc, char * const argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
#endif