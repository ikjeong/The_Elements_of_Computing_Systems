/**
    Implementation of Parser.h
*/

#include "Parser.h"

/* =========== PRIVATE ============= */

void Parser::init() {
    current_command_ = "";
    next_command_ = readCommand();
    parseCurrentCommand();
}

void Parser::clearTypeAndArgs() {
    type_ = CommandType::NOTHING;
    arg1_ = "";
    arg2_ = -1;
}

bool Parser::isEmptyCommand(const std::string& command) const {
    return (static_cast<int>(command.size()) == 0);
}

bool Parser::isArithmeticCommand(const std::string& command) const {
    auto iter = std::find(ARITHMETIC_COMMAND.begin(), ARITHMETIC_COMMAND.end(), command);
    return iter != ARITHMETIC_COMMAND.end();
}

bool Parser::isVMFile(const std::string& path) const {
    return path.find(".vm") != std::string::npos;
}

std::string Parser::readCommand() {
    std::string buffer = "";
    while (!input_.eof()) {
        std::getline(input_, buffer);
        deleteComment(buffer);
        if (!isEmptyCommand(buffer)) return buffer;
    }
    return buffer;
}

void Parser::deleteComment(std::string& command) {
    string_iter pos = command.find("//");
    if (pos != string_end) command.erase(pos, string_end);
}

void Parser::parseCurrentCommand() {
    clearTypeAndArgs();

    std::stringstream ss(current_command_);
    ss.str(current_command_);
    
    std::string word;
    if (ss >> word) type_ = checkCommandType(word);
    if (type_ == CommandType::C_ARITHMETIC) {
        arg1_ = word;
        arg2_ = -1;
    } else {
        if (ss >> word) arg1_ = word;
        if (ss >> word) arg2_ = std::stoi(word);
    }

    if (ss >> word) throw translate_exception(current_command_);
}

CommandType Parser::checkCommandType(const std::string& command) const {
    if (isArithmeticCommand(command)) return CommandType::C_ARITHMETIC;
    if (command == "push") return CommandType::C_PUSH;
    if (command == "pop") return CommandType::C_POP;
    return CommandType::NOTHING;
}

/* =========== PUBLIC ============= */

Parser::Parser(std::string path) {
    if (!isVMFile(path)) throw file_exception(path);
    input_.open(path);
    if (input_.fail()) throw file_exception(path);
    init();
}

Parser::Parser() {
}

Parser::~Parser() {
    if (input_.is_open()) input_.close();
}

bool Parser::hasMoreCommands() const {
    if (isEmptyCommand(next_command_)) return false;
    else return true;
}

void Parser::advance() {
    current_command_ = next_command_;
    next_command_ = readCommand();
    parseCurrentCommand();
}

CommandType Parser::commandType() const {
    return type_;
}

std::string Parser::arg1() const {
    return arg1_;
}

int Parser::arg2() const {
    return arg2_;
}

void Parser::setNewFile(std::string path) {
    if (input_.is_open()) input_.close();
    if (!isVMFile(path)) throw file_exception(path);
    input_.open(path);
    if (input_.fail()) throw file_exception(path);
    init();
}