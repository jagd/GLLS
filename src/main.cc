#include "gllsparser.h"
#include "parsercommon.h"
#include <iostream>

int main() {
    GllsParser glls(std::cin);
    try {
        glls.run();
    } catch (ParserError &e){
        std::cerr <<  "Error on input line "
                  << e.line() << ": " << e.what()
                  << std::endl;
    }
    return 0;
}