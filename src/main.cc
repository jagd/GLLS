#include "glls.h"
#include "parsercommon.h"
#include <iostream>

int main() {
    try {
        for (const auto x : glls(std::cin)) {
            std::cout << x << ' ';
        }
    } catch (ParserError &e){
        std::cerr <<  "Error on input line "
                  << e.line() << ": " << e.what()
                  << std::endl;
    }
    std::cout << '\n';
    return 0;
}