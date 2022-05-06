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

    auto it  = TokenTypeStrings.find(tokenType);
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
    auto it  = VarKindStrings.find(varKind);
    if (it == VarKindStrings.end()) throw compile_exception("Can't change VarKind to string.");
    else return it->second;
}

/**
 * @return String to VarKind.
 */
VarKind Var::stringToVarKind(const std::string& varKind) {
    const std::map<std::string, VarKind> VarKindStrings {
        { "static", VarKind::STATIC },
        { "field", VarKind::FIELD  },
        { "argument", VarKind::ARGUMENT },
        { "var", VarKind::VAR }
    };

    auto it  = VarKindStrings.find(varKind);
    if (it == VarKindStrings.end()) throw compile_exception("Can't change string to VarKind.");
    else return it->second;
}

/**
 * @return Segment to string.
 */
std::string VM::segmentToString(Segment segment) {
    const std::map<Segment, std::string> SegmentStrings {
        { Segment::CONST, "constant" },
        { Segment::ARG, "argument" },
        { Segment::LOCAL, "local" },
        { Segment::STATIC, "static" },
        { Segment::THIS, "this" },
        { Segment::THAT, "that" },
        { Segment::POINTER, "pointer" },
        { Segment::TEMP, "temp" },
        { Segment::NONE, "" }
    };
    auto it  = SegmentStrings.find(segment);
    if (it == SegmentStrings.end()) throw compile_exception("Can't change Segment to string.");
    else return it->second;
}

/**
 * @return Command to string.
 */
std::string VM::commandToString(Command command) {
    const std::map<Command, std::string> CommandStrings {
        { Command::ADD, "add" },
        { Command::SUB, "sub" },
        { Command::NEG, "neg" },
        { Command::EQ, "eq" },
        { Command::GT, "gt" },
        { Command::LT, "lt" },
        { Command::AND, "and" },
        { Command::OR, "or" },
        { Command::NOT, "not" }
    };
    auto it  = CommandStrings.find(command);
    if (it == CommandStrings.end()) throw compile_exception("Can't change Command to string.");
    else return it->second;
}

/**
 * @return string to binray operation command.
 */
Command VM::stringToBinaryOperation(const std::string& command) {
    const std::map<std::string, Command> StringCommands {
        { "+", Command::ADD },
        { "-", Command::SUB },
        { "=", Command::EQ },
        { ">", Command::GT},
        { "<", Command::LT},
        { "&", Command::AND },
        { "|", Command::OR }
    };
    auto it  = StringCommands.find(command);
    if (it == StringCommands.end()) throw compile_exception("Can't change string to Command.");
    else return it->second;
}

/**
 * @return string to unary operation command.
 */
Command VM::stringToUnaryOperation(const std::string& command) {
    const std::map<std::string, Command> StringCommands {
        { "~", Command::NOT },
        { "-", Command::NEG }
    };
    auto it  = StringCommands.find(command);
    if (it == StringCommands.end()) throw compile_exception("Can't change string to Command.");
    else return it->second;
}