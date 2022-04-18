/**
    Implementation of JackTokenizer.h
*/

#include "JackTokenizer.h"

/* =========== PRIVATE ============= */

void JackTokenizer::initialize() {
    token_.clear();
    current_token_index_ = -1;
    current_token_type_ = TokenType::NOTHING;
    parseProgram();
}

void JackTokenizer::parseProgram() {
    // 프로그램을 토큰화해 vector에 담는다.

    // 입력이 끝날때까지
        // 한 줄을 입력받고,
            // 라인이 마무리될때까지
            // 앞에서부터 입력받은 라인을 처리 
            // symbol을 제외한 모든 토큰들은 공백과 symbol로 구분됨을 이용 

            // 쌍따음표가 나올 경우 그 뒤부터는
            // 다시 쌍따음표가 나올때까지 하나의 토큰으로 취급.

            // "//" 주석의 경우 substr로 뒤를 없애기
            // "/*" 주석의 경우 그 뒤부터 "*/"나올때까지 continue
            // "/**" 도 동일 

    bool doesQuotationOpen = false;
    bool doesCommentOpen = false;
    while (!input_.eof()) {
        std::string buffer;
        std::getline(input_, buffer);
        parseLine(buffer, doesQuotationOpen, doesCommentOpen);
    }

    // std::cout << "Print tokens" << std::endl;
    // for (auto& token : token_) {
    //     std::cout << token << std::endl;
    // }
}

void JackTokenizer::parseLine(std::string& buffer, bool& doesQuotationOpen, bool& doesCommentOpen) {

    // 쌍따옴표 열림과 코멘트 열림은 동시에 일어날 수 없음.
    // 열려있으면 처리하는 과정을 먼저 수행
    // std::cout << buffer << std::endl;

    int startIndex = 0;
    if (doesQuotationOpen) {
        string_index index = buffer.find("\"");
        if (index == string_end) {
            token_.back().append(buffer);
            return;
        } else {
            startIndex = index+1;
            token_.back().append(buffer.substr(0, startIndex));
            doesQuotationOpen = false;
        }
    } else if (doesCommentOpen) {
        string_index index = buffer.find("*/");
        if (index == string_end) {
            return;
        } else {
            startIndex = index+2;
            doesCommentOpen = false;
        }
    }

    // 토큰화
    while (startIndex < buffer.size()) {
        std::string token = "";
        for (int endIndex = startIndex; endIndex < buffer.size(); ++endIndex) {
            // 쌍따옴표면 만든 문자열 토큰화하고 문자열 처리 시작
            if (buffer[endIndex] == '\"') {
                pushToken(token);
                string_index index = buffer.find("\"", endIndex+1);
                if (index == string_end) {
                    pushToken(buffer.substr(endIndex, string_end));
                    doesQuotationOpen = true;
                    return;
                } else {
                    pushToken(buffer.substr(endIndex, index-endIndex+1));
                    startIndex = index+1;
                    break;
                }
            }

            // 주석이면 만든 문자열 토큰화하고 주석 열고 처리시작
            if (endIndex >= 1 && buffer.substr(endIndex-1, 2) == "//") {
                // '/'로 들어간 symbol을 제거해줌.
                token_.pop_back();
                pushToken(token);
                return;
            } else if (endIndex >= 1 && buffer.substr(endIndex-1, 2) == "/*") {
                // '/'로 들어간 symbol을 제거해줌.
                token_.pop_back();
                pushToken(token);
                string_index index = buffer.find("*/", endIndex+1);
                if (index == string_end) {
                    doesCommentOpen = true;
                    return;
                } else {
                    startIndex = index+2;
                    break;
                }
            }

            // 공백 문자면 만든 문자열 토큰화하고 넘기기
            if (buffer[endIndex] == ' ' || buffer[endIndex] == '\t' || buffer[endIndex] == '\n') {
                pushToken(token);
                startIndex = endIndex+1;
                break;
            }

            // symbol이면 만든 문자열 토큰화하고 심볼 토큰화
            if (isSymbol(buffer[endIndex])) {
                pushToken(token);
                pushToken(std::string(1, buffer[endIndex]));
                startIndex = endIndex+1;
                break;
            }

            // 그 외면 토큰으로 구성
            token.push_back(buffer[endIndex]);

            // 라인 끝까지 왔다면 토큰구성
            if (endIndex+1 == (int)buffer.size()) {
                pushToken(token);
                startIndex = endIndex+1;
                break;
            }
        }
    }

}

void JackTokenizer::pushToken(const std::string& token) {
    if (token == "") return;
    token_.push_back(token);
}

bool JackTokenizer::isKeyword(const std::string& token) const {
    for (const std::string& keyword : KEYWORDS) {
        if (token == keyword) return true;
    }
    return false;
}

bool JackTokenizer::isSymbol(const char& token) const {
    for (const char& symbol : SYMBOLS) {
        if (token == symbol) return true;
    }
    return false;
}

TokenType JackTokenizer::checkTokenType(const std::string& token) const {
    // token의 type을 반환
    if (isKeyword(token)) return TokenType::KEYWORD;
    if (token.size() == 1 && isSymbol(token[0])) return TokenType::SYMBOL;
    if (token.size() >= 2 && token[0] == '\"' && token[token.size()-1] == '\"') return TokenType::STRING_CONST;
    try {
        std::stoi(token);
        return TokenType::INT_CONST;
    } catch (std::exception& e) { }
    std::regex re("[_a-zA-Z][_a-zA-Z0-9]*");
    if (std::regex_match(token, re)) return TokenType::IDENTIFIER;

    return TokenType::NOTHING;
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
    if (current_token_index_ < (int)token_.size()-1) return true;
    return false;
}

void JackTokenizer::advance() {
    // current_token_index를 다음으로 하고, current_token_type을 설정
    ++current_token_index_;
    current_token_type_ = checkTokenType(token_[current_token_index_]);
}

TokenType JackTokenizer::tokenType() const {
    // current_token_type을 반환
    return current_token_type_;
}

std::string JackTokenizer::keyword() const {
    // current_token_type이 keyword가 아니면 예외
    // 그 외는 토큰값 반환
    if (current_token_type_ != TokenType::KEYWORD) throw translate_exception(token_[current_token_index_]);
    return token_[current_token_index_];
}

char JackTokenizer::symbol() const {
    // current_token_type이 symbol이 아니면 예외
    // 그 외는 토큰값 반환
    if (current_token_type_ != TokenType::SYMBOL) throw translate_exception(token_[current_token_index_]);
    return token_[current_token_index_][0];
}

std::string JackTokenizer::identifier() const {
    // current_token_type이 identifier가 아니면 예외
    // 그 외는 토큰값 반환
    if (current_token_type_ != TokenType::IDENTIFIER) throw translate_exception(token_[current_token_index_]);
    return token_[current_token_index_];
}

int JackTokenizer::intVal() const {
    // current_token_type이 intVal가 아니면 예외
    // 그 외는 토큰값 반환  
    if (current_token_type_ != TokenType::INT_CONST) throw translate_exception(token_[current_token_index_]);
    return std::stoi(token_[current_token_index_]);
}

std::string JackTokenizer::stringVal() const {
    // current_token_type이 stringVal이 아니면 예외
    // 그 외는 토큰값 반환  
    if (current_token_type_ != TokenType::STRING_CONST) throw translate_exception(token_[current_token_index_]);
    return token_[current_token_index_].substr(1, token_[current_token_index_].size()-2);
}