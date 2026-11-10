#include <iostream>
#include <exception>
#include "Demo.h"
int main() {
    try { runDemo();
    }
    catch (const std::exception& ex) {

        std::cerr << "Database error: " << ex.what() << '\n';

        return 1;
    }
    return 0;
}
