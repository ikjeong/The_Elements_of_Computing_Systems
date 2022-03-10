/**
    Global Constants and Header, Exception Class
*/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
/* If gcc version is under 9, use '-lstdc++fs' */
#include <filesystem>

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

class file_exception : public std::runtime_error {
public:
    file_exception(const std::string& path)
    : runtime_error("File Exception: fail to load file(Path: " + path + ").") { }
};

class translate_exception : public std::runtime_error {
public:
    translate_exception(const std::string& command)
    : runtime_error("Translate Exception: fail to translate command(" + command + ").") { }
};

#endif