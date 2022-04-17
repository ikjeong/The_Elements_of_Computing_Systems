/**
    Implementation of JackTokenizer.h
*/

#include "JackTokenizer.h"

/* =========== PRIVATE ============= */

void JackTokenizer::initialize() {
    current_token_index_ = -1;
    current_token_type_ = TokenType::NOTHING;
    parseToken();
}

void JackTokenizer::parseToken() {
    // 프로그램을 토큰화해 vector에 담는다.

    // 입력이 끝날때까지
        // 한 줄을 입력받고,
            // 라인이 마무리될때까지
            // 앞에서부터 입력받은 라인을 처리 
            // symbol을 제외한 모든 토큰들은 공백과 symbol로 구분됨을 이용 
            
}

TokenType JackTokenizer::checkTokenType(const std::string& token) const {
    // token의 type을 반환
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
    // token size 와 current_token_index를 비교해 반환 
}

void JackTokenizer::advance() {
    // current_token_index를 다음으로 하고, current_token_type을 설정
}

TokenType JackTokenizer::tokenType() const {
    // current_token_type을 반환 
}

std::string JackTokenizer::keyword() const {
    // current_token_type이 keyword가 아니면 예외
    // 그 외는 토큰값 반환
}

char JackTokenizer::symbol() const {
    // current_token_type이 symbol이 아니면 예외
    // 그 외는 토큰값 반환  
}

std::string JackTokenizer::identifier() const {
    // current_token_type이 identifier가 아니면 예외
    // 그 외는 토큰값 반환  
}

int JackTokenizer::intVal() const {
    // current_token_type이 intVal가 아니면 예외
    // 그 외는 토큰값 반환  
}

std::string JackTokenizer::stringVal() const {
        // current_token_type이 stringVal이 아니면 예외
    // 그 외는 토큰값 반환  
}