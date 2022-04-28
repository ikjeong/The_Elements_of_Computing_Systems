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
 * Exception class used when an error occurs during compile.
 * Inherits std::runtime_error.
 */
class compile_exception : public std::runtime_error {
public:
    /**
     * Exception message: Compile Exception: message
     *                    Line number: line
     */
    compile_exception(const std::string& message, const int line)
    : runtime_error("Compile Exception: " + message + "\nLine number: " + std::to_string(line)) { }

    /**
     * Exception message: Compile Exception: message
     */
    compile_exception(const std::string& message)
    : runtime_error("Compile Exception: " + message) { }
};

#endif