/**
 * Jack Tokenizer Module(Class)
 * This module decomposes the program into tokens.
 *
 * Interface
 * - tokenize: set the program to tokenize.
 * - hasMoreCommands
 * - advance: read next token
 * - tokenType: return current token's type
 * - getToken: return current token
 * - getCurrentTokenLineNumber: Returns which line the token is on in the program
 *
 * Caution
 * - When generated, current token is initialized to ""(Empty string).
 * - Therefore, you need to use advance() before using another function.
 */

#ifndef __JACK_TOKENIZER_H__
#define __JACK_TOKENIZER_H__

#define string_index std::string::size_type
#define string_end std::string::npos

#include "Global.h"

const std::vector<std::string> KEYWORDS = {
    "class", "constructor", "function", "method",
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
    std::vector<int> token_line_number_;
    int current_token_index_;
    TokenType current_token_type_;

    void initialize();
    void tokenizeFile();
    void tokenizeLine(std::string& buffer, bool& doesQuotationOpen, bool& doesCommentOpen, int lineNumber);
    
    void pushToken(const std::string& token, int lineNumber);
    void popToken();
    
    bool isKeyword(const std::string& token) const;
    bool isSymbol(const char& token) const;
    bool isJackFile(const std::string& path) const;
    
    TokenType getTokenTypeOf(const std::string& token) const;

public:
    JackTokenizer();
    ~JackTokenizer();

    void tokenize(const std::string& path);
    bool hasMoreTokens() const;
    void advance();
    void retreat();
    TokenType tokenType() const;
    std::string getToken() const;
    int getCurrentTokenLineNumber() const;
};

#endif