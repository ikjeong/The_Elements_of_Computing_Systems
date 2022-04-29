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

void CompilationEngine::printIndent() {
    for (int i = 0; i < indent_depth_ * 2; ++i)
        *output_ << ' ';
}

void CompilationEngine::printKeyword() {
    printIndent();
    *output_ << "<keyword> " << jack_tokenizer_->keyword() << " </keyword>" << std::endl;
}

void CompilationEngine::printSymbol() {
    printIndent();
    *output_ << "<symbol> " << changeSymboltoXmlSymbol(jack_tokenizer_->symbol()) << " </symbol>" << std::endl;
}

void CompilationEngine::printIntegerConstant() {
    printIndent();
    *output_ << "<integerConstant> " << jack_tokenizer_->intVal() << " </integerConstant>" << std::endl;
}

void CompilationEngine::printStringConstant() {
    printIndent();
    *output_ << "<stringConstant> " << jack_tokenizer_->stringVal() << " </stringConstant>" << std::endl;
}

void CompilationEngine::printIdentifier() {
    printIndent();
    *output_ << "<identifier> " << jack_tokenizer_->identifier() << " </identifier>" << std::endl;
}

std::string CompilationEngine::changeSymboltoXmlSymbol(const char& symbol) const {
    if (symbol == '<') return "&lt;";
    else if (symbol == '>') return "&gt;";
    else if (symbol == '\"') return "&quot;";
    else if (symbol == '&') return "&amp;";
    else return std::string(1, symbol);
}

bool CompilationEngine::checkKeyword(const std::string& keyword) const {
    if (jack_tokenizer_->tokenType() == TokenType::KEYWORD &&
        jack_tokenizer_->keyword() == keyword) return true;
    else return false;
}

bool CompilationEngine::checkSymbol(const char& symbol) const {
    if (jack_tokenizer_->tokenType() == TokenType::SYMBOL &&
        jack_tokenizer_->symbol() == symbol) return true;
    else return false;
}

bool CompilationEngine::checkPrimitiveType() const {
    if (checkKeyword("int") || checkKeyword("char") || checkKeyword("boolean")) return true;
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
    else if (checkSymbol('(') || checkSymbol('-') || checkSymbol('~')) return true;
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

void CompilationEngine::checkAndPrintSymbol(const char& symbol) {
    if (checkSymbol(symbol)) printSymbol();
    else throw compile_exception("Expected symbol('" + std::string(1, symbol) + std::string("')"), jack_tokenizer_->getCurrentTokenLineNumber());
}

/**
 * @param expectedIdentifier Use only when outputting exception messages.
 */
void CompilationEngine::checkAndPrintIdentifier(const std::string& expectedIdentifier) {
    if (jack_tokenizer_->tokenType() == TokenType::IDENTIFIER) printIdentifier();
    else throw compile_exception("Expected Identifier for " + expectedIdentifier, jack_tokenizer_->getCurrentTokenLineNumber());
}

/**
 * type: 'int' | 'char' | 'boolean' | className
 */
void CompilationEngine::checkAndPrintType() {
    if (jack_tokenizer_->tokenType() == TokenType::IDENTIFIER) printIdentifier();
    else if (checkPrimitiveType()) printKeyword();
    else throw compile_exception("Expected type(primitive type or className)", jack_tokenizer_->getCurrentTokenLineNumber());
}

void CompilationEngine::checkAndPrintCommaAndVarName() {
    checkAndPrintSymbol(',');

    advance("identifier for varName");
    checkAndPrintIdentifier("varName");
}

void CompilationEngine::checkAndPrintTypeAndVarName() {
    checkAndPrintType();

    advance("identifier for varName");
    checkAndPrintIdentifier("varName or type(primitive type or className)");
}

/**
 * subroutineCall: subroutineName '(' expressionList ')' |
 *                 (className | varName) '.' subroutineName '(' expressionList ')'
 */
void CompilationEngine::checkAndPrintSubroutineCall() {
    /* Print identifier(it may be className or varName, or subroutineName). */
    checkAndPrintIdentifier("className or varName, or subroutineName.");

    /* If token is '.', print '.' and subroutineName. If token is '(', just print it. */
    advance("symbol('.') or symbol('(')");

    if (checkSymbol('.')) {
        /* Print '.'. */
        printSymbol();

        /* Print subroutineName. */
        advance("identifier for subroutineName");
        checkAndPrintIdentifier("subroutineName.");

        /* Get '(' token. */
        advance("symbol('(')");
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
