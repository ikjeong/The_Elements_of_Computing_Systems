/**
 * Implementation of Global.h
 */

#include "Global.h"

/**
 * @return TokenType to string.
 */
std::string Token::tokenTypeToString(TokenType tokenType) {
    const std::map<TokenType, std::string> TokenTypeStrings {
        { TokenType::KEYWORD, "keyword" },
        { TokenType::SYMBOL, "symbol" },
        { TokenType::IDENTIFIER, "identifier" },
        { TokenType::INT_CONST, "integerConstant" },
        { TokenType::STRING_CONST, "stringConstant" }
    };

    auto   it  = TokenTypeStrings.find(tokenType);
    if (it == TokenTypeStrings.end()) throw compile_exception("Can't change TokenType to string.");
    else return it->second;
}

/**
 * @return TokenType to string.
 */
std::string Var::varKindToString(VarKind varKind) {
    const std::map<VarKind, std::string> VarKindStrings {
        { VarKind::STATIC, "static" },
        { VarKind::FIELD, "field" },
        { VarKind::ARGUMENT, "argument" },
        { VarKind::VAR, "var" },
        { VarKind::CLASS, "class" },
        { VarKind::SUBROUTINE, "subroutine" }
    };

    auto   it  = VarKindStrings.find(varKind);
    if (it == VarKindStrings.end()) throw compile_exception("Can't change VarKind to string.");
    else return it->second;
}