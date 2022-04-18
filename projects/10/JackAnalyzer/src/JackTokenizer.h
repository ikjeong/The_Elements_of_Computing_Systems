/**
    Jack Tokenizer Module(Class)
    This module decomposes the program into tokens.

    Interface
    - setFile: set the program to tokenize.
    - hasMoreCommands
    - advance: read next token
    - tokenType: return current token's type
    - keyword: return current token's keyword (tokenType == KEYWORD)
    - symbol: return token's character (tokenType == SYMBOL)
    - identifier: return token's identifier (tokenType == IDENTIFIER)
    - intVal: return token's int valute (tokenType == INT_CONST)
    - stringVal: return token's string value (tokenType == STRING_CONST)

    Caution
    - When generated, current token is initialized to ""(Empty string).
    - Therefore, you need to use advance() before using another function.
*/

#ifndef __JACK_TOKENIZER_H__
#define __JACK_TOKENIZER_H__

#define string_index std::string::size_type
#define string_end std::string::npos

#include "Global.h"

const std::vector<std::string> KEYWORDS = {
    "class", "cunstructor", "function", "method",
    "field", "static", "var", "int", "char",
    "boolean", "void", "true", "false", "null",
    "this", "let", "do", "if", "else", "while", "return"
};

const std::vector<char> SYMBOLS = {
    '{', '}', '(', ')', '[', ']', '.',
    ',', ';', '+', '-', '*', '/', '&',
    '|', '<', '>', '=', '~'
};

class JackTokenizer {
private:
    std::ifstream input_;
    std::vector<std::string> token_;
    int current_token_index_;
    TokenType current_token_type_;

    void initialize();
    void parseProgram();
    void parseLine(std::string& buffer, bool& doesQuotationOpen, bool& doesCommentOpen);
    void pushToken(const std::string& token);
    bool isKeyword(const std::string& token) const;
    bool isSymbol(const char& token) const;
    TokenType checkTokenType(const std::string& token) const;
    bool isJackFile(const std::string& path) const;

public:
    JackTokenizer();
    ~JackTokenizer();

    void setFile(const std::string& path);
    bool hasMoreTokens() const;
    void advance();
    TokenType tokenType() const;
    std::string keyword() const;
    char symbol() const;
    std::string identifier() const;
    int intVal() const;
    std::string stringVal() const;
};

#endif