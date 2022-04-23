/**
 * Implementation of CompilationEngine.h
 */

#include "CompilationEngine.h"

/* =========== PRIVATE ============= */

/**
 * Set up the tokenization module and output file.
 * @param jackTokenizer The module that has completed tokenization must be delivered.
 * @param output Specifies the file to be compiled output. It must be in the .xml file format.
 */
void CompilationEngine::initialize(JackTokenizer* jackTokenizer, std::ofstream* output) {
    jack_tokenizer_ = jackTokenizer;
    output_ = output;
    indent_depth_ = 0;
}

/**
 * Advance Tokenizer.
 * @param expectedToken when fail to advance, throw exception with message includes expected token.
 */
void CompilationEngine::advance(const std::string& expectedToken) {
    if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
    else throw analyze_exception("Next token does not exist. Compiler expects " + expectedToken + std::string("."));
}

/**
 * printXXX() functions need to be checked that can be called.
 * checkAndPrintXXX() don't need to be checked that can be called.
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
    *output_ << "<symbol> " << jack_tokenizer_->symbol() << " </symbol>" << std::endl;
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
 * It need to be called when token is expected type.
 */
void CompilationEngine::checkAndPrintSymbol(const char& symbol) {
    if (checkSymbol(symbol)) printSymbol();
    else throw analyze_exception("Expected symbol('" + std::string(1, symbol) + std::string("')"));
}

/**
 * It need to be called when token is expected type.
 * @param expectedIdentifier Use only when outputting exception messages.
 */
void CompilationEngine::checkAndPrintIdentifier(const std::string& expectedIdentifier) {
    if (jack_tokenizer_->tokenType() == TokenType::IDENTIFIER) printIdentifier();
    else throw analyze_exception("Expected Identifier for " + expectedIdentifier);
}

/**
 * It need to be called when token is expected type.
 */
void CompilationEngine::checkAndPrintType() {
    if (jack_tokenizer_->tokenType() == TokenType::IDENTIFIER) printIdentifier();
    else if (checkPrimitiveType()) printKeyword();
    else throw analyze_exception("Expected type(primitive type or className)");
}

/**
 * It need to be called when token is ','.
 * When return, token is varName.
 * 
 * @param endSymbol Check and print comman & varName until token is endSymbol.
 */
void CompilationEngine::checkAndPrintCommaAndVarName() {
    /* If symbol(',') exists, token(identifier(varName)) must exist. */
    checkAndPrintSymbol(',');

    advance("identifier for varName");
    checkAndPrintIdentifier("varName");
}

/**
 * It need to be called when token is expected type.
 * When return, token is varName.
 */
void CompilationEngine::checkAndPrintTypeAndVarName() {
    /* Print type. */
    checkAndPrintType();

    /* Print varName. */
    advance("identifier for varName");
    checkAndPrintIdentifier("varName or type(primitive type or className)");
}

/**
 * All compileXXX() member functions must have
 * the tokenization module pointing to the token
 * that fits XXX before being called.
 */

/**
 * Compile the entire class.
 * 
 * Class: 'class' className '{' classVarDec* subroutineDec* '}'
 * classVarDec: ('static' | 'field') type varName (',' varName)* ';'
 * subroutineDec: ('constructor' | 'function' | 'method') ('void' | type) subroutineName
 *                '(' parameterList ')' subroutineBody
 */
void CompilationEngine::compileClass() {
    printStartTag("class");

    /* Print 'class'. */
    printKeyword(); // It must be 'class'.

    /* Print className. */
    advance("identifier for className");
    checkAndPrintIdentifier("className");

    /* Print '{'. */
    advance("symbol('{')");
    checkAndPrintSymbol('{');

    /* Print classVarDec, subroutineDec. */
    while (true) {
        advance("symbol('}'), or keyword for classVarDec or subroutineDec.");

        if (checkSymbol('}')) break;
        
        if (checkKeyword("static")) compileClassVarDec();
        else if (checkKeyword("field")) compileClassVarDec();
        else if (checkKeyword("constructor")) compileSubroutine();
        else if (checkKeyword("function")) compileSubroutine();
        else if (checkKeyword("method")) compileSubroutine();
        else throw analyze_exception("Expected symbol('}') or keyword(classVarDec or subroutineDec)");
    }

    /* Print '}'. */
    printSymbol(); // It must be '}'.

    printEndTag("class");
}

/**
 * Compile static or field declarations.
 * 
 * VarDec: ('static' | 'field') type varName (',' varName)* ';'
 * type: 'int' | 'char' | 'boolean' | className
 */
void CompilationEngine::compileClassVarDec() {
    printStartTag("classVarDec");

    /* Print 'static' or 'field'. */
    printKeyword(); // It must be 'static' or 'field'.

    /* Print type and varName. */
    advance("type");
    checkAndPrintTypeAndVarName();

    /* Print ',' varName until ';'. */
    while (true) {
        advance("symbol(';') or symbol(',')");

        if (checkSymbol(';')) break;

        try { checkAndPrintCommaAndVarName(); }
        catch(analyze_exception& e) { throw analyze_exception("Expected symbol(';') or symbol(',')"); }
    }

    /* Print ';'. */
    printSymbol(); // It must be ';'.

    printEndTag("classVarDec");
}

/**
 * Compiles an entire method, function, or constructor.
 * 
 * compileSubroutine: ('constructor' | 'function' | 'method') ('void' | type) subroutineName
 *                    '(' parameterList ')' subroutineBody
 * parameterList: ((type varName) (',' type varName)*)?
 * subroutineBody: '{' varDec* statements '}'
 */
void CompilationEngine::compileSubroutine() {
    printStartTag("subroutine");

    /* Print 'constructor' or 'function', or 'method'. */
    printKeyword(); // It must be 'static' or 'field', or 'method'.

    /* Print type or 'void'. */
    try {
        advance("type");
        checkAndPrintType();
    } catch (analyze_exception& e) {
        if (checkKeyword("void")) printKeyword();
        else throw analyze_exception("Expected 'void' or type(primitive type or className)");
    }
    
    /* Print subroutineName. */
    advance("identifier for subroutineName");
    checkAndPrintIdentifier("subroutineName or return type('void' or type(primitive type or className)");

    /* Print '('. */
    advance("symbol('(')");
    checkAndPrintSymbol('(');

    /* Print parameterList. */
    advance("type for parameter or symbol ')'");
    compileParameterList();
    
    /* Print ')'. */
    advance("symbol(')')");
    checkAndPrintSymbol(')');

    /* Print subroutineBody. */
    advance("symbol('{')");
    if (checkSymbol('{')) compileSubroutineBody();
    else throw analyze_exception("Expected symbol('{')");

    printEndTag("subroutine");
}

/**
 * Compile the parameter list (may be an empty list). It does not include '()'.
 * Caution: Token that tokenizer moudle points to at first need to be checked.
 *          Because it can be empty parameterList.
 * 
 * parameterList: ((type varName) (',' type varName)*)?
 */
void CompilationEngine::compileParameterList() {
    printStartTag("parameterList");

    /* At first, check token.
       If token is ')', empty parameterList. */
    if (checkSymbol(')')) {
        --indent_depth_;
        printIndent();
        *output_ << "</parameterList>" << std::endl;
        jack_tokenizer_->retreat();
        return;
    }
    
    /* Print type and varName. */
    checkAndPrintTypeAndVarName();  // It is expected that token points to type.

    /* Print ',' type, varName. */
    while (true) {
        advance("symbol(')'), or symbol(',') for more parameter");
        
        if (checkSymbol(')')) break;

        /* If symbol(',') exists, type and token(identifier(varName)) must exist. */
        try { checkAndPrintSymbol(','); }
        catch(analyze_exception& e) { throw analyze_exception("Expected symbol(')') or symbol(',')"); }

        /* Print type and varName. */
        advance("type and varName for more parameter");
        checkAndPrintTypeAndVarName();
    }

    printEndTag("parameterList");
    jack_tokenizer_->retreat();
}

/**
 * Compile the subroutine body.
 * 
 * subroutineBody: '{' varDec* statements '}'
 * varDec: 'var' type varName (',' varName)* ';'
 * statements: statement*
 */
void CompilationEngine::compileSubroutineBody() {
    printStartTag("subroutineBody");

    /* Print '{'. */
    printSymbol(); // It must be '{'.

    /* When keyword 'var' exists, Print varDec. */
    advance("'var' or keyword for statement, or symbol('}')");
    while (checkKeyword("var")) {
        compileVarDec();
        advance("'var' or keyword for statement, or symbol('}').");
    }

    /* Print statements. It can be empty statements. */
    compileStatements();

    /* Print '}'. */
    advance("symbol('}')");
    checkAndPrintSymbol('}');

    printEndTag("subroutineBody");
}

/**
 * Compile the var declaration.
 */
void CompilationEngine::compileVarDec() {
    printStartTag("varDec");

    /* Print 'var'. */
    printKeyword(); // It must be 'var'.

    /* Print type and varName. */
    advance("type and identifier for varName");
    checkAndPrintTypeAndVarName();

    /* Print ',' varName until ';'. */
    while (true) {
        advance("symbol(';') or symbol(',')");

        if (checkSymbol(';')) break;

        try { checkAndPrintCommaAndVarName(); }
        catch(analyze_exception& e) { throw analyze_exception("Expected symbol(';') or symbol(',')"); }
    }

    /* Print ';'. */
    printSymbol(); // It must be ';'.

    printEndTag("varDec");
}

/**
 * Compiles a series of statements. Does not include '{}'.
 * Caution: Token that tokenizer moudle points to at first need to be checked.
 *          Because it can be empty statements.
 * 
 * statements: statement*
 * statement: letStatement | ifStatement | whileStatement |
 *            doStatement | returnStatement
 * letStatement: 'let' varName ('[' expression ']')? '=' expression ';'
 * ifStatement: 'if' '(' expression ')' '{' statements '}'
 *              ('else' '{' statements '}')?
 * whileStatement: 'while' '(' expression ')' '{' statements '}'
 * doStatement: 'do' subroutineCall ';'
 * returnStatement: 'return' expression? ';'
 */
void CompilationEngine::compileStatements() {
    printStartTag("statements");

    /* Check statement and print. */
    while (true) {
        /* At first, check token. */
        if (checkSymbol('}')) break;

        if (checkKeyword("let")) compileLet();
        else if (checkKeyword("if")) compileIf();
        else if (checkKeyword("while")) compileWhile();
        else if (checkKeyword("do")) compileDo();
        else if (checkKeyword("return")) compileReturn();
        else throw analyze_exception("Expected symbol('}') or keyword for statement.");
        
        advance("symbol('}') or keyword for statement");
    }

    printEndTag("statements");
    jack_tokenizer_->retreat();
}

/**
 * Compile the do statement.
 * 
 * doStatement: 'do' subroutineCall ';'
 * subroutineCall: subroutineName '(' expressionList ')' |
 *                 (className | varName) '.' subroutineName '(' expressionList ')'
 */
void CompilationEngine::compileDo() {
    printStartTag("doStatement");

    /* Print 'do'. */
    printKeyword(); // It must be 'do'.

    /* Print identifier(it may be className or varName, or subroutineName). */
    advance("identifier for className or varName, or subroutineName");
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
    advance("expression or symbol(')')");
    compileExpressionList();

    /* Print ')'. */
    advance("symbol(')')");
    checkAndPrintSymbol(')');

    /* Print ';'. */
    advance("symbol(';')");
    checkAndPrintSymbol(';');

    printEndTag("doStatement");
}

/**
 * Compile the let statement.
 * 
 * letStatement: 'let' varName ('[' expression ']')? '=' expression ';'
 */
void CompilationEngine::compileLet() {
    printStartTag("letStatement");

    /* Print 'let'. */
    printKeyword(); // It must be 'let'.

    /* Print varName. */
    advance("identifier for varName");
    checkAndPrintIdentifier("varName.");

    /* If token is '[', print '[' expression ']'. If token is '=', print it. */
    advance("symbol('[') or symbol('=')");

    if (checkSymbol('[')) {
        /* Print '['. */
        printSymbol();

        /* Print expression. */
        advance("expression");
        /* Need to check token */
        compileExpression();

        /* Print ']'. */
        advance("symbol(']')");
        checkAndPrintSymbol(']');
        
        /* Get '='. */
        advance("symbol('=')");
    }

    /* Print '='. */
    checkAndPrintSymbol('=');

    /* Print expression. */
    advance("expression");
    /* Need to check token */
    compileExpression();

    /* Print ';'. */
    advance("symbol(';')");
    checkAndPrintSymbol(';');

    printEndTag("letStatement");
}

/**
 * compile the while statement
 */
void CompilationEngine::compileWhile() {

}

/**
 * Compile the return statement.
 */
void CompilationEngine::compileReturn() {

}

/**
 * Compile the if statement and the following else clause.
 */
void CompilationEngine::compileIf() {

}

/**
 * Compile an expression.
 */
void CompilationEngine::compileExpression() {
    printStartTag("expression");

    /* Need Implement. Now on, just compile one term(identifier). */
    compileTerm();

    printEndTag("expression");
}

/**
 * Compile an term.
 */
void CompilationEngine::compileTerm() {
    printStartTag("term");

    /* Need Implement. Now on, just compile one term(identifier). */
    checkAndPrintIdentifier("term");

    printEndTag("term");
}

/**
 * Compiles a comma-separated list of expressions (which can be an empty list)
 * Caution: Token that tokenizer moudle points to at first need to be checked.
 *          Because it can be empty statements.
 */
void CompilationEngine::compileExpressionList() {
    printStartTag("expressionList");


    /* Need Implement. */
    

    printEndTag("expressionList");
    jack_tokenizer_->retreat();
}

/* =========== PUBLIC ============= */

CompilationEngine::CompilationEngine() {
}

CompilationEngine::~CompilationEngine() {
}

/**
 * Set up the tokenization module and output file, and start the compilation output.
 * @param jackTokenizer The module that has completed tokenization must be delivered.
 * @param output Specifies the file to be compiled output. It must be in the .xml file format.
 */
void CompilationEngine::compile(JackTokenizer* jackTokenizer, std::ofstream* output) {
    initialize(jackTokenizer, output);

    if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
    else throw analyze_exception("The token does not exist. Is it jack file?");
    
    if (checkKeyword("class")) compileClass();
    else throw analyze_exception("The first syntax must be class.");

    if (jack_tokenizer_->hasMoreTokens()) throw analyze_exception("Only one class must exist in one file.");
}