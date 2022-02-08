/**
    Implementation of Parser.h
*/

#include "Global.h"
#include "Parser.h"

// =========== PRIVATE ============= //

void Parser::Initializer() {
    currentLineNumber = 0;
    currentCommand = readCommand();
    checkCommandType(currentCommand);
}

std::string Parser::readCommand() {
    std::string buffer = "";
    while (!input.eof()) {
        std::getline(input, buffer);
        ++currentLineNumber;
        deleteCommentAndWhiteSpace(buffer);
        if (int(buffer.size()) > 0) return buffer;
    }
    return buffer;
}

void Parser::deleteCommentAndWhiteSpace(std::string& command) {
    string_iter pos = command.find("//");
    if (pos != string_end) command.erase(pos, string_end);

    int idx = 0;
    while (idx < command.size()) {
        if (command[idx] == ' ' || command[idx] == '\t') {
            command.erase(idx--, 1);
        }
        ++idx;
    }
}

void Parser::checkCommandType(const std::string& command) {
    if (command.size() == 0) {
        type = CommandType::nothing;
    } else if (command[0] == '@') {
        type = CommandType::address;    // @{1}[a-zA-Z_.$:]+[a-zA-Z_.&:0-9]*
    } else if (command[0] == '(' && command[command.size()-1] == ')') {
        type = CommandType::label;      // [(]{1}[a-zA-Z_.$:]+[a-zA-Z_.&:0-9]*[)]{1}
    } else {
        type = CommandType::compute;
    }
}

// =========== PUBLIC ============= //

Parser::Parser(std::string path) : input(path) {
    if (input.fail()) {
        std::cout << "Can't find file" << std::endl;
        std::exit(int(Error::fileOpen));
    }
    Initializer();
}

bool Parser::hasMoreCommands() const {
    if (int(currentCommand.size()) == 0) return false;
    else return true;
}

Parser::~Parser() {
    input.close();
}

void Parser::advance() {
    if (!hasMoreCommands()) return;
    currentCommand = readCommand();
    checkCommandType(currentCommand);
}

CommandType Parser::commandType() const {
    return type;
}

std::string Parser::symbol() const {
    if (type != CommandType::address && type != CommandType::label) {
        std::cout << "Incorrect function call." << std::endl;
        std::exit(int(Error::functionCall));
    }
    std::string result = currentCommand.substr(1, string_end);
    if (result.back() == ')') result.pop_back();
    return result;
}

std::string Parser::dest() const {
    if (type != CommandType::compute) {
        std::cout << "Incorrect function call." << std::endl;
        std::exit(int(Error::functionCall));
    }
    string_iter destPos = currentCommand.find("=");
    std::string result = "";
    if (destPos == string_end) return result;
    result = currentCommand.substr(0, destPos);
    return result;
}

std::string Parser::comp() const {
    if (type != CommandType::compute) {
        std::cout << "Incorrect function call." << std::endl;
        std::exit(int(Error::functionCall));
    }
    string_iter destPos = currentCommand.find("=");
    string_iter jumpPos = currentCommand.find(";");
    std::string result = currentCommand;
    if (destPos != string_end) result.erase(0, destPos+1);
    if (jumpPos != string_end) result.erase(jumpPos, string_end);
    return result;
}

std::string Parser::jump() const {
    if (type != CommandType::compute) {
        std::cout << "Incorrect function call." << std::endl;
        std::exit(int(Error::functionCall));
    }
    string_iter jumpPos = currentCommand.find(";");
    std::string result = "";
    if (jumpPos == string_end) return result;
    result = currentCommand.substr(jumpPos+1, string_end);
    return result;
}

void Parser::resetCursor() {
    input.clear();
    input.seekg(0, std::ios::beg);
    Initializer();
}