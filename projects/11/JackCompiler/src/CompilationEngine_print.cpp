/**
 * Implementation of CompilationEngine.h
 * Implement the member functions associated with the output.
 */

#include "CompilationEngine.h"

/* =========== PRIVATE ============= */

/**
 * printXXX() functions need to be checked before being called.
 * checkAndPrintXXX() don't need to be checked before being called.
 */

void CompilationEngine::printIndent() {
    for (int i = 0; i < indent_depth_ * 2; ++i)
        *output_ << ' ';
}

void CompilationEngine::printStartTag(const std::string& tag) {
    printIndent();
    *output_ << "<" << tag << ">" << std::endl;
    ++indent_depth_;
}

void CompilationEngine::printEndTag(const std::string& tag) {
    --indent_depth_;
    printIndent();
    *output_ << "</" << tag << ">" << std::endl;
}

void CompilationEngine::printTerminalNode(const TokenType tokenType, const std::string& token) {
    printIndent();
    *output_ << "<" << tokenTypeToString(tokenType) << "> ";
    if (tokenType == TokenType::SYMBOL) *output_ << changeSymboltoXmlSymbol(token[0]);
    else                                *output_ << token;
    *output_ << " </" << tokenTypeToString(tokenType) << ">" << std::endl;
}

std::string CompilationEngine::changeSymboltoXmlSymbol(const char& symbol) const {
    if (symbol == '<') return "&lt;";
    else if (symbol == '>') return "&gt;";
    else if (symbol == '\"') return "&quot;";
    else if (symbol == '&') return "&amp;";
    else return std::string(1, symbol);
}



void CompilationEngine::printKeyword() {
    printTerminalNode(TokenType::KEYWORD, jack_tokenizer_->getToken());
}

void CompilationEngine::printSymbol() {
    printTerminalNode(TokenType::SYMBOL, jack_tokenizer_->getToken());
}

void CompilationEngine::printIdentifier(const VarKind varKind, const std::string& type, const int index) {
    printTerminalNode(TokenType::IDENTIFIER, jack_tokenizer_->getToken());
    printIndent();
    *output_ << "<Attribute> ";
    *output_ << "VarKind: " << varKindToString(varKind);
    if (varKind != VarKind::CLASS && varKind != VarKind::SUBROUTINE) *output_ << ", type: " << type;
    if (varKind != VarKind::CLASS && varKind != VarKind::SUBROUTINE) *output_ << ", index: " << index;
    *output_ << " </Attribute>" << std::endl;
}

void CompilationEngine::printIntegerConstant() {
    printTerminalNode(TokenType::INT_CONST, jack_tokenizer_->getToken());
}

void CompilationEngine::printStringConstant() {
    printTerminalNode(TokenType::STRING_CONST, jack_tokenizer_->getToken());
}



bool CompilationEngine::checkKeyword(const std::string& keyword) const {
    if (jack_tokenizer_->tokenType() == TokenType::KEYWORD &&
        jack_tokenizer_->getToken() == keyword) return true;
    else return false;
}

bool CompilationEngine::checkKeyword(const std::vector<std::string>& keyword) const {
    if (jack_tokenizer_->tokenType() != TokenType::KEYWORD) return false;
    for (const std::string& k : keyword) {
        if (jack_tokenizer_->getToken() == k)
            return true;
    }
    return false;
}

bool CompilationEngine::checkSymbol(const char symbol) const {
    if (jack_tokenizer_->tokenType() == TokenType::SYMBOL &&
        jack_tokenizer_->getToken()[0] == symbol) return true;
    else return false;
}

bool CompilationEngine::checkSymbol(const std::vector<char>& symbol) const {
    if (jack_tokenizer_->tokenType() != TokenType::SYMBOL) return false;
    for (const char s : symbol) {
        if (jack_tokenizer_->getToken()[0] == s)
            return true;
    }
    return false;
}

/**
 * Check that the identifier you want to verify is same varKind.
 * VARNAME: Use for identifiers that must have been declared. This is one of var, argument, field, static.
 * VAR: The identifier must be declared as a regional variable.
 * ARGUMENT: The identifier must be declared as a parameter.
 * STATIC: Must be a variable declared static.
 * FIELD: Must be a variable declared field.
 * CLASS: As the class name, symbolTable returns NONE.
 * SUBROUTINE: As a subroutine name, symbolTable returns NONE.
 * 
 * Caution: It is judged by the symbol table, so it is impossible to confirm that it is both CLASS and SUBROUTINE.
 */
bool CompilationEngine::checkIdentifier(const VarKind varKind) const {
    if (jack_tokenizer_->tokenType() != TokenType::IDENTIFIER) return false;
    
    VarKind varKindInSymTable = symbol_table_->kindOf(jack_tokenizer_->getToken());
    if (varKindInSymTable == VarKind::NONE) {
        if (varKind == VarKind::CLASS || varKind == VarKind::SUBROUTINE) return true;
        else return false;
    } else {
        if (varKind == varKindInSymTable || varKind == VarKind::VARNAME) return true;
        else return false;
    }

    return false;
}

/**
 * Check that the identifier you want to verify is same varKind.
 * VARNAME: Use for identifiers that must have been declared. This is one of var, argument, field, static.
 * VAR: The identifier must be declared as a regional variable.
 * ARGUMENT: The identifier must be declared as a parameter.
 * STATIC: Must be a variable declared static.
 * FIELD: Must be a variable declared field.
 * CLASS: As the class name, symbolTable returns NONE.
 * SUBROUTINE: As a subroutine name, symbolTable returns NONE.
 * 
 * Caution: It is judged by the symbol table, so it is impossible to confirm that it is both CLASS and SUBROUTINE.
 */
bool CompilationEngine::checkIdentifier(const std::vector<VarKind>& varKind) const {
    if (jack_tokenizer_->tokenType() != TokenType::IDENTIFIER) return false;

    VarKind varKindInSymTable = symbol_table_->kindOf(jack_tokenizer_->getToken());
    for (const VarKind k : varKind) {
        if (varKindInSymTable == VarKind::NONE) {
            if (k == VarKind::CLASS || k == VarKind::SUBROUTINE) {
                return true;
            }
        } else {
            if (k == varKindInSymTable || k == VarKind::VARNAME) {
                return true;
            }
        }
    }
    return false;
}


bool CompilationEngine::checkIntegerConstant() const {
    if (jack_tokenizer_->tokenType() == TokenType::INT_CONST) return true;
    else return false;
}

bool CompilationEngine::checkStringConstant() const {
    if (jack_tokenizer_->tokenType() == TokenType::STRING_CONST) return true;
    else return false;
}



bool CompilationEngine::checkVarDecKeyword() const {
    if (checkKeyword("static"))     return true;
    else if (checkKeyword("field")) return true;
    else return false;
}

bool CompilationEngine::checkSubroutineDecKeyword() const {
    if (checkKeyword("constructor"))   return true;
    else if (checkKeyword("function")) return true;
    else if (checkKeyword("method"))   return true;
    else return false;
}

/**
 * KeywordConstant: 'true' | 'false' | 'null' | 'this'
 */
bool CompilationEngine::checkKeywordConstant() const {
    if (checkKeyword("true")) return true;
    else if (checkKeyword("false")) return true;
    else if (checkKeyword("null")) return true;
    else if (checkKeyword("this")) return true;
    else return false;
}

bool CompilationEngine::checkPrimitiveType() const {
    if (checkKeyword("int"))          return true;
    else if (checkKeyword("char"))    return true;
    else if (checkKeyword("boolean")) return true;
    else return false;
}

/**
 * type: 'int' | 'char' | 'boolean' | className
 */
bool CompilationEngine::checkType() const {
    if (checkPrimitiveType()) return true;
    else if (checkIdentifier(VarKind::CLASS)) return true;
    else return false;
}

/**
 * term: integerConstant | stringConstant | keywordConstant |
 *       varName | varName '[' expression ']' | subroutineCall |
 *       '(' expression ')' unaryOp term
 */
bool CompilationEngine::checkTerm() const {
    if (jack_tokenizer_->tokenType() == TokenType::INT_CONST) return true;
    else if (jack_tokenizer_->tokenType() == TokenType::STRING_CONST) return true;
    else if (checkKeywordConstant()) return true;
    else if (jack_tokenizer_->tokenType() == TokenType::IDENTIFIER) return true;
    else if (checkSymbol('(')) return true;
    else if (checkUnaryOp()) return true;
    else return false;
}

/**
 * op: '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
 */
bool CompilationEngine::checkOp() const {
    if (checkSymbol('+')) return true;
    else if (checkSymbol('-')) return true;
    else if (checkSymbol('*')) return true;
    else if (checkSymbol('/')) return true;
    else if (checkSymbol('&')) return true;
    else if (checkSymbol('|')) return true;
    else if (checkSymbol('<')) return true;
    else if (checkSymbol('>')) return true;
    else if (checkSymbol('=')) return true;
    else return false;
}

bool CompilationEngine::checkUnaryOp() const {
    if (checkSymbol('-')) return true;
    else if (checkSymbol('~')) return true;
    else return false;
}



void CompilationEngine::checkAndDefineIdentifier(const std::string& type, const VarKind varKind) {
    if (jack_tokenizer_->tokenType() != TokenType::IDENTIFIER) throw compile_exception("Expected Identifier for VarDec.", jack_tokenizer_->getCurrentTokenLineNumber());

    VarKind varKindInSymTable = symbol_table_->kindOf(jack_tokenizer_->getToken());
    if (varKindInSymTable != VarKind::NONE) throw compile_exception("This variable is already defined.", jack_tokenizer_->getCurrentTokenLineNumber());

    symbol_table_->define(jack_tokenizer_->getToken(), type, varKind);
}



void CompilationEngine::checkAndPrintKeyword(const std::string& keyword) {
    if (checkKeyword(keyword)) printKeyword();
    else throw compile_exception("Expected keyword('" + keyword + "')", jack_tokenizer_->getCurrentTokenLineNumber());
}

void CompilationEngine::checkAndPrintKeyword(const std::vector<std::string>& keyword) {
    if (checkKeyword(keyword)) printKeyword();
    else {
        std::string exceptionMessage = "Expected keyword('";
        for (const std::string& k : keyword) {
            exceptionMessage.append(k);
            exceptionMessage.append("or ");
        }
        for (int i = 0; i < std::string("or ").size(); ++i)
            exceptionMessage.pop_back();
        exceptionMessage.append("')");
        throw compile_exception(exceptionMessage, jack_tokenizer_->getCurrentTokenLineNumber());
    }
}

void CompilationEngine::checkAndPrintSymbol(const char symbol) {
    if (checkSymbol(symbol)) printSymbol();
    else throw compile_exception("Expected symbol('" + std::string(1, symbol) + std::string("')"), jack_tokenizer_->getCurrentTokenLineNumber());
}

void CompilationEngine::checkAndPrintSymbol(const std::vector<char>& symbol) {
    if (checkSymbol(symbol)) printSymbol();
    else {
        std::string exceptionMessage = "Expected symbol('";
        for (const char s : symbol) {
            exceptionMessage.push_back(s);
            exceptionMessage.append(" or ");
        }
        for (int i = 0; i < std::string(" or ").size(); ++i)
            exceptionMessage.pop_back();
        exceptionMessage.append("')");
        throw compile_exception(exceptionMessage, jack_tokenizer_->getCurrentTokenLineNumber());
    }
}

/**
 * Check that the identifier you want to verify is same varKind.
 * VARNAME: Use for identifiers that must have been declared. This is one of var, argument, field, static.
 * VAR: The identifier must be declared as a regional variable.
 * ARGUMENT: The identifier must be declared as a parameter.
 * STATIC: Must be a variable declared static.
 * FIELD: Must be a variable declared field.
 * CLASS: As the class name, symbolTable returns NONE.
 * SUBROUTINE: As a subroutine name, symbolTable returns NONE.
 * 
 * Caution: It is judged by the symbol table, so it is impossible to confirm that it is both CLASS and SUBROUTINE.
 */
void CompilationEngine::checkAndPrintIdentifier(const VarKind varKind) {
    if (checkIdentifier(varKind)) {
        VarKind varKindInSymTable = symbol_table_->kindOf(jack_tokenizer_->getToken());
        if (varKindInSymTable == VarKind::NONE) printIdentifier(varKind);
        else printIdentifier(varKindInSymTable, symbol_table_->typeOf(jack_tokenizer_->getToken()), symbol_table_->indexOf(jack_tokenizer_->getToken()));
    } else throw compile_exception("Expected Identifier('" + varKindToString(varKind) + "')", jack_tokenizer_->getCurrentTokenLineNumber());
}

/**
 * Check that the identifier you want to verify is same varKind.
 * VARNAME: Use for identifiers that must have been declared. This is one of var, argument, field, static.
 * VAR: The identifier must be declared as a regional variable.
 * ARGUMENT: The identifier must be declared as a parameter.
 * STATIC: Must be a variable declared static.
 * FIELD: Must be a variable declared field.
 * CLASS: As the class name, symbolTable returns NONE.
 * SUBROUTINE: As a subroutine name, symbolTable returns NONE.
 * 
 * Caution: It is judged by the symbol table, so it is impossible to confirm that it is both CLASS and SUBROUTINE.
 */
void CompilationEngine::checkAndPrintIdentifier(const std::vector<VarKind>& varKind) {
    if (checkIdentifier(varKind)) {
        VarKind varKindInSymTable = symbol_table_->kindOf(jack_tokenizer_->getToken());
        if (varKindInSymTable == VarKind::NONE && std::find(varKind.begin(), varKind.end(), VarKind::CLASS) != varKind.end()) printIdentifier(VarKind::CLASS);
        else if (varKindInSymTable == VarKind::NONE && std::find(varKind.begin(), varKind.end(), VarKind::SUBROUTINE) != varKind.end()) printIdentifier(VarKind::SUBROUTINE);
        else printIdentifier(varKindInSymTable, symbol_table_->typeOf(jack_tokenizer_->getToken()), symbol_table_->indexOf(jack_tokenizer_->getToken()));
    } else {
        std::string exceptionMessage = "Expected Identifier('";
        for (const VarKind k : varKind) {
            exceptionMessage.append(varKindToString(k));
            exceptionMessage.append(" or ");
        }
        for (int i = 0; i < std::string(" or ").size(); ++i)
            exceptionMessage.pop_back();
        exceptionMessage.append("')");
        throw compile_exception(exceptionMessage, jack_tokenizer_->getCurrentTokenLineNumber());
    }
}

void CompilationEngine::checkAndPrintIntegerConstant() {
    if (checkIntegerConstant()) printIntegerConstant();
    else throw compile_exception("Expected Integer Constant", jack_tokenizer_->getCurrentTokenLineNumber());
}

void CompilationEngine::checkAndPrintStringConstant() {
    if (checkStringConstant()) printStringConstant();
    else throw compile_exception("Expected String Constant", jack_tokenizer_->getCurrentTokenLineNumber());

}

/**
 * type: 'int' | 'char' | 'boolean' | className
 */
void CompilationEngine::checkAndPrintType() {
    if (checkIdentifier(VarKind::CLASS)) printIdentifier(VarKind::CLASS);
    else if (checkPrimitiveType()) printKeyword();
    else throw compile_exception("Expected type(primitive type or className)", jack_tokenizer_->getCurrentTokenLineNumber());
}

/**
 * subroutineCall: subroutineName '(' expressionList ')' |
 *                 (className | varName) '.' subroutineName '(' expressionList ')'
 */
void CompilationEngine::checkAndPrintSubroutineCall() {
    /* If token is '.', print '.' and subroutineName. If token is '(', just print it. */
    advance("symbol('.') or symbol('(')");
    if (checkSymbol('.')) {
        /* Print identifier for className or varName. */
        jack_tokenizer_->retreat();
        std::vector<VarKind> expectedIdentifier = {VarKind::CLASS, VarKind::VARNAME};
        checkAndPrintIdentifier(expectedIdentifier);

        /* Print '.'. */
        advance("symbol('.')");
        checkAndPrintSymbol('.');

        /* Print subroutineName. */
        advance("identifier for subroutineName");
        checkAndPrintIdentifier(VarKind::SUBROUTINE);
    } else {
        /* Print identifier for subroutineName. */
        jack_tokenizer_->retreat();
        checkAndPrintIdentifier(VarKind::SUBROUTINE);
    }

    /* Print '('. */
    advance("symbol('(')");
    checkAndPrintSymbol('(');

    /* Print expressionList. It can be empty expressionList. */
    advance("expressionList or symbol(')')");
    compileExpressionList();

    /* Print ')'. */
    advance("symbol(')')");
    checkAndPrintSymbol(')');
}
