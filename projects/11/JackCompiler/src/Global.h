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
#include <map>
#include <tuple>
/* If gcc version is under 9, use '-lstdc++fs' when compiling. */
#include <filesystem>

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

namespace Token {
    enum class TokenType {
        NOTHING = 0,
        KEYWORD = 1,
        SYMBOL = 2,
        IDENTIFIER = 3,
        INT_CONST = 4,
        STRING_CONST = 5
    };

    /**
     * @return TokenType to string.
     */
    std::string tokenTypeToString(TokenType tokenType);
};
using namespace Token;

namespace Var {
    enum class VarKind {
        NONE = 0,       // if it isn't in table, return NONE.
        STATIC = 1,
        FIELD = 2,
        ARGUMENT = 3,
        VAR = 4,
        CLASS = 5,      // class isn't in table. so symbolTable need to return NONE.
        SUBROUTINE = 6, // subroutine isn't in table. so symbolTable need to return NONE.
        VARNAME = 7     // require check. It will be static, field, argument, var.
    };

    /**
     * @return TokenType to string.
     */
    std::string varKindToString(VarKind varKind);

    /**
     * @return String to VarKind.
     */
    VarKind stringToVarKind(const std::string& varKind);
};
using namespace Var;

enum class Segment {
    CONST = 0,
    ARG = 1,
    LOCAL = 2,
    STATIC = 3,
    THIS = 4,
    THAT = 5,
    POINTER = 6,
    TEMP = 7
};

enum class Command {
    ADD = 0,
    SUB = 1,
    NEG = 2,
    EQ = 3,
    GT = 4,
    LT = 5,
    AND = 6,
    OR = 7,
    NOT = 8
};

#endif