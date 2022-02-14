/**
    Implementation of Parser.h
*/

#include "Parser.h"

/* =========== PRIVATE ============= */

void Parser::initializer() {
    current_command_file_line_ = 0;
    file_line_ = 0;
    command_address_ = -1;
    current_command_ = "";
    next_command_ = readCommand();
    checkCommandType(current_command_);
}

std::string Parser::readCommand() {
    std::string buffer = "";
    while (!input_.eof()) {
        ++file_line_;
        std::getline(input_, buffer);
        deleteComment(buffer);
        deleteWhiteSpace(buffer);
        if (!isEmptyCommand(buffer)) return buffer;
    }
    return buffer;
}

void Parser::deleteComment(std::string& command) {
    string_iter pos = command.find("//");
    if (pos != string_end) command.erase(pos, string_end);
}

void Parser::deleteWhiteSpace(std::string& command) {
    int idx = 0;
    while (idx < static_cast<int>(command.size())) {
        if (command[idx] == ' ' || command[idx] == '\t') {
            command.erase(idx--, 1);
        }
        ++idx;
    }
}

void Parser::checkCommandType(const std::string& command) {
    if (isEmptyCommand(command)) {
        type_ = CommandType::nothing;
    } else if (command[0] == '@') {
        type_ = CommandType::address;    // @{1}[a-zA-Z_.$:]+[a-zA-Z_.&:0-9]*
    } else if (command[0] == '(' && command[command.size()-1] == ')') {
        type_ = CommandType::label;      // [(]{1}[a-zA-Z_.$:]+[a-zA-Z_.&:0-9]*[)]{1}
    } else {
        type_ = CommandType::compute;
    }
}

bool Parser::isEmptyCommand(const std::string& command) const {
    return (static_cast<int>(command.size()) == 0);
}

/* =========== PUBLIC ============= */

Parser::Parser(std::string path) {
    input_.open(path);
    if (input_.fail()) throw fileException(path);
    initializer();
}

bool Parser::hasMoreCommands() const {
    if (isEmptyCommand(next_command_)) return false;
    else return true;
}

Parser::~Parser() {
    input_.close();
}

void Parser::advance() {
    current_command_file_line_ = file_line_;
    current_command_ = next_command_;
    next_command_ = readCommand();
    checkCommandType(current_command_);

    if (type_ == CommandType::address || type_ == CommandType::compute)
        ++command_address_;
}

CommandType Parser::commandType() const {
    return type_;
}

std::string Parser::symbol() const {
    if (type_ != CommandType::address && type_ != CommandType::label)
        throw functionCallException("This command isn't A-COMMAND or label(Parse line: " + std::to_string(current_command_file_line_) + ")");
    std::string result = current_command_.substr(1, string_end);
    if (result.back() == ')') result.pop_back();
    return result;
}

std::string Parser::dest() const {
    if (type_ != CommandType::compute)
        throw functionCallException("This command isn't C-COMMAND(Parse line: " + std::to_string(current_command_file_line_) + ")");
    string_iter destPos = current_command_.find("=");
    std::string result = "";
    if (destPos == string_end) return result;
    result = current_command_.substr(0, destPos);
    return result;
}

std::string Parser::comp() const {
    if (type_ != CommandType::compute)
        throw functionCallException("This command isn't C-COMMAND(Parse line: " + std::to_string(current_command_file_line_) + ")");
    string_iter destPos = current_command_.find("=");
    string_iter jumpPos = current_command_.find(";");
    std::string result = current_command_;
    if (jumpPos != string_end) result.erase(jumpPos, string_end);
    if (destPos != string_end) result.erase(0, destPos+1);
    return result;
}

std::string Parser::jump() const {
    if (type_ != CommandType::compute)
        throw functionCallException("This command isn't C-COMMAND(Parse line: " + std::to_string(current_command_file_line_) + ")");
    string_iter jumpPos = current_command_.find(";");
    std::string result = "";
    if (jumpPos == string_end) return result;
    result = current_command_.substr(jumpPos+1, string_end);
    return result;
}

void Parser::resetCursor() {
    input_.clear();
    input_.seekg(0, std::ios::beg);
    initializer();
}

int Parser::getCurrentCommandFileLine() const {
    return current_command_file_line_;
}

int Parser::getFileLine() const {
    return file_line_;
}

int Parser::getCommandAddress() const {
    return command_address_;
}