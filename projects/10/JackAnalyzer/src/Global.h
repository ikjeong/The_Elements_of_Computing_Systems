/**
 * Global Constants and Header, Exception Class
 */

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <regex>
/* If gcc version is under 9, use '-lstdc++fs' when compiling. */
#include <filesystem>

enum class TokenType {
    NOTHING = 0,
    KEYWORD = 1,
    SYMBOL = 2,
    IDENTIFIER = 3,
    INT_CONST = 4,
    STRING_CONST = 5
};

/**
 * Exception class used when a file-related error occurs.
 * Inherits std::runtime_error.
 */
class file_exception : public std::runtime_error {
public:
    /**
     * @param path "File Exception: fail to load file(Path: " + path + ")."
     */
    file_exception(const std::string& path)
    : runtime_error("File Exception: fail to load file(Path: " + path + ").") { }
};

/**
 * Exception class used when an error occurs during analysis.
 * Inherits std::runtime_error.
 */
class analyze_exception : public std::runtime_error {
public:
    /**
     * @param message "Analyze Exception: fail to analyze (" + message + ")."
     */
    analyze_exception(const std::string& message)
    : runtime_error("Analyze Exception: fail to analyze (" + message + ").") { }
};

#endif