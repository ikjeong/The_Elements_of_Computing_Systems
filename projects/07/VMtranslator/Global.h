/**
    Global Constants and Header, Exception Function
*/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

enum class CommandType {
    NOTHING = 0,
    C_ARITHMETIC = 1,
    C_PUSH = 2,
    C_POP = 3,
    C_LABEL = 4,
    C_GOTO = 5,
    C_IF = 6,
    C_FUNCTION = 7,
    C_RETURN = 8,
    C_CALL = 9
};

void makeError(const std::string& message) {
    std::cout << "Error: " << message << std::endl;
    exit(-1);
}

#endif