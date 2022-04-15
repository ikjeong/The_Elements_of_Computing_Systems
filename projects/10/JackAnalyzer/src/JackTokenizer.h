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

#include "Global.h"

class JackTokenizer {
private:

public:
    JackTokenizer();
    ~JackTokenizer();

    void setFile(const string& path);
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