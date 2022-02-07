/**
    Parser Moudle(Class)

    Routines
    - hasMoreCommands
    - advance: read next command
    - commandType: return current command's type
    - symbol: if A-Command or L-Command, return symbol or decimal number
    - dest: if C-Command, return dest symbol
    - comp: if C-Command, return comp symbol
    - jump: if C-Command, return jump symbol
*/

#ifndef __PARSER_H__
#define __PARSER_H__

#include "global.h"

enum class CommandType { nothing = 0, address = 1, compute = 2, label = 3 };
enum class Error { fileOpen = 100, functionCall = 200};

class Parser {
private:
    std::ifstream input;
    std::string currentCommand;
    CommandType type;
    int currentLineNumber;

private:
    void Initializer();
    std::string readCommand();
    void deleteCommentAndWhiteSpace(std::string& command);
    void checkCommandType(std::string& command);

public:
    Parser(std::string path);
    ~Parser();

    bool hasMoreCommands() const;
    void advance();
    CommandType commandType() const;
    std::string symbol() const;
    std::string dest() const;
    std::string comp() const;
    std::string jump() const;
    void resetCursor();
};

#endif