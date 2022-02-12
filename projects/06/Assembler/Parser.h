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

    Caution
    - When generated, current_command_ is initialized to ""(Empty string).
    - Therefore, you need to use advance() before using another function.
*/

#ifndef __PARSER_H__
#define __PARSER_H__

#include "Global.h"

#define string_end std::string::npos
typedef std::string::size_type string_iter;

class Parser {
private:
    std::ifstream input_;
    std::string current_command_;
    std::string next_command_;
    CommandType type_;
    int file_line_;

private:
    void initializer();
    std::string readCommand();
    void deleteComment(std::string& command);
    void deleteWhiteSpace(std::string& command);
    void checkCommandType(const std::string& command);
    bool isEmptyCommand(const std::string& command) const;

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