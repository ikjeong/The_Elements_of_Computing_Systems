/**
    Parser Module(Class)

    Routines
    - hasMoreCommands
    - advance: read next command
    - commandType: return current command's type
    - arg1: return argument1 (if Arithmetic command, return command)
    - arg2: return argument2

    Caution
    - When generated, current_command_ is initialized to ""(Empty string).
    - Therefore, you need to use advance() before using another function.

    Arithmetic command
    - add
    - sub
    - neg
    - eq
    - gt
    - lt
    - and
    - or
    - not

    Memory access command
    - push segment index
    - pop segment index
*/

#ifndef __PARSER_H__
#define __PARSER_H__

#include "Global.h"

#define string_end std::string::npos
typedef std::string::size_type string_iter;

const std::vector<std::string> ARITHMETIC_COMMAND = {
    "add",
    "sub",
    "neg",
    "eq",
    "gt",
    "lt",
    "and",
    "or",
    "not"
};

class Parser {
private:
    std::ifstream input_;
    std::string current_command_;
    std::string next_command_;
    CommandType type_;
    std::string arg1_;
    int arg2_;

    void init();
    void clearTypeAndArgs();
    bool isEmptyCommand(const std::string& command) const;
    bool isArithmeticCommand(const std::string& command) const;
    std::string readCommand();
    void deleteComment(std::string& command);
    void parseCurrentCommand();
    CommandType checkCommandType(const std::string& command) const;
    
public:
    Parser(std::string path);
    ~Parser();

    bool hasMoreCommands() const;
    void advance();
    CommandType commandType() const;
    std::string arg1() const;
    int arg2() const;
};

#endif