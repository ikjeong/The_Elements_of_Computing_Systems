/**
    Global Constants and Header, Exception Class
*/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <bitset>
#include <map>

enum class CommandType { nothing = 0, address = 1, compute = 2, label = 3 };

class fileException : public std::runtime_error {
public:
    fileException(std::string path)
    : runtime_error("File Exception: fail to load file(Path: " + path + ").") {}
};

#endif