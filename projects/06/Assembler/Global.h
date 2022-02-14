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
enum class SymbolType { nothing = 0, address = 1, dest = 2, comp = 3, jump = 4, label = 5 };

class fileException : public std::runtime_error {
public:
    fileException(const std::string& path)
    : runtime_error("File Exception: fail to load file(Path: " + path + ").") { }
};

class functionCallException : public std::runtime_error {
public:
    functionCallException(const std::string& message)
    : runtime_error("Incorrect function call(" + message + ")") { }
};

class translateException : public std::runtime_error {
public:
    translateException(const std::string& command)
    : runtime_error("Translate Exception: fail to translate command(" + command + ").") { }
};

#endif