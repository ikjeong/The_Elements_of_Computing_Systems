/**
    Implementation of JackTokenizer.h
*/

#include "JackTokenizer.h"

/* =========== PRIVATE ============= */

void JackTokenizer::initialize() {
    current_token_ = "";
    // next_token_ = readToken();
    // current_token_type_ = checkTokenType(current_token_);
}

bool JackTokenizer::isJackFile(const std::string& path) const {
    return path.find(".jack") != std::string::npos;
}

/* =========== PUBLIC ============= */

JackTokenizer::JackTokenizer() {

}

JackTokenizer::~JackTokenizer() {

}

void JackTokenizer::setFile(const std::string& path) {
    // input stream을 설정하고, 모듈을 초기화한다.
    if (input_.is_open()) input_.close();
    if (!isJackFile(path)) throw file_exception(path);
    input_.open(path);
    if (input_.fail()) throw file_exception(path);
    initialize();

    // while (!input_.eof()) {
    //     std::string buffer;
    //     std::getline(input_, buffer);
    //     std::cout << buffer << std::endl;
    // }
}

bool JackTokenizer::hasMoreTokens() const {

}

void JackTokenizer::advance() {

}

TokenType JackTokenizer::tokenType() const {

}

std::string JackTokenizer::keyword() const {

}

char JackTokenizer::symbol() const {

}

std::string JackTokenizer::identifier() const {

}

int JackTokenizer::intVal() const {

}

std::string JackTokenizer::stringVal() const {
    
}