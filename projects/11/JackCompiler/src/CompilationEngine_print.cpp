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

void CompilationEngine::printIdentifier() {
    printTerminalNode(TokenType::IDENTIFIER, jack_tokenizer_->getToken());
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

bool CompilationEngine::checkIdentifier(const std::string& expectedIdentifier) const {
    if (jack_tokenizer_->tokenType() != TokenType::IDENTIFIER) return false;
    
    /* 만약 예상되는 종류의 identifier면 참을 반환 */
        return true;

    return false;
}
bool CompilationEngine::checkIdentifier(const std::vector<std::string>& expectedIdentifier) const {
    if (jack_tokenizer_->tokenType() != TokenType::IDENTIFIER) return false;
    for (const std::string& i : expectedIdentifier) {
        /* 만약 예상되는 종류의 identifier면 참을 반환 */
            return true;
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
    else if (checkIdentifier("className")) return true;
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
            exceptionMessage.append("or ");
        }
        for (int i = 0; i < std::string("or ").size(); ++i)
            exceptionMessage.pop_back();
        exceptionMessage.append("')");
        throw compile_exception(exceptionMessage, jack_tokenizer_->getCurrentTokenLineNumber());
    }
}

void CompilationEngine::checkAndPrintIdentifier(const std::string& expectedIdentifier) {
    if (checkIdentifier(expectedIdentifier)) printIdentifier();
    else throw compile_exception("Expected Identifier('" + expectedIdentifier + "')", jack_tokenizer_->getCurrentTokenLineNumber());
}

void CompilationEngine::checkAndPrintIdentifier(const std::vector<std::string>& expectedIdentifier) {
    if (checkIdentifier(expectedIdentifier)) printIdentifier();
    else {
        std::string exceptionMessage = "Expected Identifier('";
        for (const std::string& i : expectedIdentifier) {
            exceptionMessage.append(i);
            exceptionMessage.append("or ");
        }
        for (int i = 0; i < std::string("or ").size(); ++i)
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
    if (checkIdentifier("className")) printIdentifier();
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
        std::vector<std::string> expectedIdentifier = {"className", "varName"};
        checkAndPrintIdentifier(expectedIdentifier);

        /* Print '.'. */
        advance("symbol('.')");
        checkAndPrintSymbol('.');

        /* Print subroutineName. */
        advance("identifier for subroutineName");
        checkAndPrintIdentifier("subroutineName");

        /* Get '(' token. */
        advance("symbol('(')");
    } else {
        /* Print identifier for subroutineName. */
        jack_tokenizer_->retreat();
        checkAndPrintIdentifier("subroutineName");
    }

    /* Print '('. */
    checkAndPrintSymbol('(');

    /* Print expressionList. It can be empty expressionList. */
    advance("expressionList or symbol(')')");
    compileExpressionList();

    /* Print ')'. */
    advance("symbol(')')");
    checkAndPrintSymbol(')');
}
