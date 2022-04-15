/**
    Global Constants and Header, Exception Class
*/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
/* If gcc version is under 9, use '-lstdc++fs' */
#include <filesystem>

enum class TokenType {
    NOTHING = 0,
    KEYWORD = 1,
    SYMBOL = 2,
    IDENTIFIER = 3,
    INT_CONST = 4,
    STRING_CONST = 5
};

class file_exception : public std::runtime_error {
public:
    file_exception(const std::string& path)
    : runtime_error("File Exception: fail to load file(Path: " + path + ").") { }
};

#endif