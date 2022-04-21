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
 * printXXX() functions need to be checked that can be called.
 * checkAndPrintXXX() don't need to be checked that can be called.
 */

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

bool CompilationEngine::checkPrimitiveType() {
    if (jack_tokenizer_->tokenType() == TokenType::KEYWORD &&
       (jack_tokenizer_->keyword() == "int" ||
        jack_tokenizer_->keyword() == "char" ||
        jack_tokenizer_->keyword() == "boolean")) return true;
    else return false;
}

/**
 * When return, token is type.
 */
void CompilationEngine::checkAndPrintType() {
    if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
    else throw analyze_exception("The token does not exist. It needs type.");

    if (jack_tokenizer_->tokenType() == TokenType::IDENTIFIER) printIdentifier();
    else if (checkPrimitiveType()) printKeyword();
    else throw analyze_exception("Required type(primitive type or className)");
}

/**
 * When return, token is endSymbol.
 * 
 * @param endSymbol Check and print comman & varName until token is endSymbol.
 */
void CompilationEngine::checkAndPrintCommaAndVarName(const char& endSymbol) {
    while (true) {
        if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
        else throw analyze_exception("The token does not exist. It needs symbol '" + std::string(1, endSymbol) + std::string("', or symbol ',' for more VarDec."));
        
        if (jack_tokenizer_->tokenType() == TokenType::SYMBOL && 
           jack_tokenizer_->symbol() == endSymbol) break;

        /* If symbol(',') exists, token(identifier(varName)) must exist. */
        if (jack_tokenizer_->tokenType() == TokenType::SYMBOL &&
            jack_tokenizer_->symbol() == ',') printSymbol();
        else throw analyze_exception("Required symbol('" + std::string(1, endSymbol) + std::string("') or symbol(',')"));

        if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
        else throw analyze_exception("The token does not exist. It needs identifier for varName");

        if (jack_tokenizer_->tokenType() == TokenType::IDENTIFIER) printIdentifier();
        else throw analyze_exception("Required identifier(varName)");
    }
}

/**
 * When return, token is VarName.
 */
void CompilationEngine::checkAndPrintTypeAndVarName() {
    /* Print type. */
    checkAndPrintType();

    /* Print varName. */
    if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
    else throw analyze_exception("The token does not exist. It needs identifier for varName.");

    if (jack_tokenizer_->tokenType() == TokenType::IDENTIFIER) printIdentifier();
    else throw analyze_exception("Required identifier(varName) or type(primitive type or className)");
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
    printIndent();
    *output_ << "<class>" << std::endl;
    ++indent_depth_;

    /* Print 'class'. */
    printKeyword(); // It must be 'class'.

    /* Print className. */
    if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
    else throw analyze_exception("The token does not exist. It needs identifier for class.");

    if (jack_tokenizer_->tokenType() == TokenType::IDENTIFIER) printIdentifier();
    else throw analyze_exception("Required identifier(className)");

    /* Print '{'. */
    if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
    else throw analyze_exception("The token does not exist. It needs symbol '{'");

    if (jack_tokenizer_->tokenType() == TokenType::SYMBOL && 
        jack_tokenizer_->symbol() == '{') printSymbol();
    else throw analyze_exception("Required symbol('{')");

    /* Print classVarDec, subroutineDec. */
    while (true) {
        if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
        else throw analyze_exception("The token does not exist. It needs symbol '}', or keyword for classVarDec or subroutineDec.");
        
        if (jack_tokenizer_->tokenType() == TokenType::SYMBOL && 
           jack_tokenizer_->symbol() == '}') break;
        
        if (jack_tokenizer_->tokenType() != TokenType::KEYWORD)
            throw analyze_exception("Required symbol('}') or keyword(classVarDec or subroutineDec)");

        if (jack_tokenizer_->keyword() == "static") compileClassVarDec();
        else if (jack_tokenizer_->keyword() == "field") compileClassVarDec();
        else if (jack_tokenizer_->keyword() == "constructor") compileSubroutine();
        else if (jack_tokenizer_->keyword() == "function") compileSubroutine();
        else if (jack_tokenizer_->keyword() == "method") compileSubroutine();
        else throw analyze_exception("Required symbol('}') or keyword(classVarDec or subroutineDec)");
    }

    /* Print '}'. */
    printSymbol(); // It must be '}'.

    --indent_depth_;
    printIndent();
    *output_ << "</class>" << std::endl;
}

/**
 * Compile static or field declarations.
 * 
 * VarDec: ('static' | 'field') type varName (',' varName)* ';'
 * type: 'int' | 'char' | 'boolean' | className
 */
void CompilationEngine::compileClassVarDec() {
    printIndent();
    *output_ << "<classVarDec>" << std::endl;
    ++indent_depth_;

    /* Print 'static' or 'field'. */
    printKeyword(); // It must be 'static' or 'field'.

    /* Print type and varName. */
    checkAndPrintTypeAndVarName();

    /* Print ',' varName until ';'. */
    checkAndPrintCommaAndVarName(';');

    /* Print ';'. */
    printSymbol(); // It must be ';'.

    --indent_depth_;
    printIndent();
    *output_ << "</classVarDec>" << std::endl;
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
    printIndent();
    *output_ << "<subroutine>" << std::endl;
    ++indent_depth_;

    /* Print 'constructor' or 'function', or 'method'. */
    printKeyword(); // It must be 'static' or 'field', or 'method'.

    /* Print 'void' or type. */
    try {
        checkAndPrintType();
    } catch (analyze_exception& e) {
        if (jack_tokenizer_->tokenType() == TokenType::KEYWORD &&
            jack_tokenizer_->keyword() == "void") printKeyword();
        else throw analyze_exception("Required 'void' or type(primitive type or className)");
    }
    
    /* Print subroutineName. */
    if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
    else throw analyze_exception("The token does not exist. It needs identifier for subroutineName.");

    if (jack_tokenizer_->tokenType() == TokenType::IDENTIFIER) printIdentifier();
    else throw analyze_exception("Required identifier(subroutineName) or return type('void' or type(primitive type or className))");

    /* Print '('. */
    if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
    else throw analyze_exception("The token does not exist. It needs symbol '('");

    if (jack_tokenizer_->tokenType() == TokenType::SYMBOL && 
        jack_tokenizer_->symbol() == '(') printSymbol();
    else throw analyze_exception("Required symbol('(')");

    /* Print parameterList and ')'. */
    if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
    else throw analyze_exception("The token does not exist. It needs type for parameter or symbol ')'");

    compileParameterList();
    
    if (jack_tokenizer_->tokenType() == TokenType::SYMBOL && 
        jack_tokenizer_->symbol() == ')') printSymbol();
    else throw analyze_exception("Required symbol(')')");

    /* Print subroutineBody. */
    if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
    else throw analyze_exception("The token does not exist. It needs symbol '{'");

    if (jack_tokenizer_->tokenType() == TokenType::SYMBOL && 
        jack_tokenizer_->symbol() == '{') compileSubroutineBody();
    else throw analyze_exception("Required symbol('{')");

    --indent_depth_;
    printIndent();
    *output_ << "</subroutine>" << std::endl;
}

/**
 * Compile the parameter list (may be an empty list). It does not include '()'.
 * Caution: Token that tokenizer moudle points to at first need to be checked.
 *          When this function returns, jack tokenizer module points to next token.
 *          Therefore, token must be used before advance().
 * 
 * parameterList: ((type varName) (',' type varName)*)?
 */
void CompilationEngine::compileParameterList() {
    printIndent();
    *output_ << "<parameterList>" << std::endl;
    ++indent_depth_;

    /* If token is ')', empty parameterList. */
    if (jack_tokenizer_->tokenType() == TokenType::SYMBOL &&
        jack_tokenizer_->symbol() == ')') {
        --indent_depth_;
        printIndent();
        *output_ << "</parameterList>" << std::endl;
        return;
    }
    
    /* Print type. token already points to type, so cannot use checkAndPrintTypeAndVarName(). */
    if (jack_tokenizer_->tokenType() == TokenType::IDENTIFIER) printIdentifier();
    else if (checkPrimitiveType()) printKeyword();
    else throw analyze_exception("Required type(primitive type or className)");

    /* Print varName. */
    if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
    else throw analyze_exception("The token does not exist. It needs identifier for varName.");

    if (jack_tokenizer_->tokenType() == TokenType::IDENTIFIER) printIdentifier();
    else throw analyze_exception("Required identifier(varName) or type(primitive type or className)");

    /* Print ',' type, varName. */
    while (true) {
        if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
        else throw analyze_exception("The token does not exist. It needs symbol ')', or symbol ',' for more parameter.");
        
        if (jack_tokenizer_->tokenType() == TokenType::SYMBOL && 
           jack_tokenizer_->symbol() == ')') break;

        /* If symbol(',') exists, type and token(identifier(varName)) must exist. */
        if (jack_tokenizer_->tokenType() == TokenType::SYMBOL &&
            jack_tokenizer_->symbol() == ',') printSymbol();
        else throw analyze_exception("Required symbol(')') or symbol(',')");

        /* Print type and varName. */
        checkAndPrintTypeAndVarName();
    }

    --indent_depth_;
    printIndent();
    *output_ << "</parameterList>" << std::endl;
}

/**
 * Compile the subroutine body.
 * 
 * subroutineBody: '{' varDec* statements '}'
 * varDec: 'var' type varName (',' varName)* ';'
 * statements: statement*
 */
void CompilationEngine::compileSubroutineBody() {
    printIndent();
    *output_ << "<subroutineBody>" << std::endl;
    ++indent_depth_;

    /* Print '{'. */
    printSymbol(); // It must be '{'.

    /* When keyword 'var' exists, Print varDec. */
    if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
    else throw analyze_exception("The token does not exist. It needs 'var' or keyword for statement, or '}'.");

    while (jack_tokenizer_->tokenType() == TokenType::KEYWORD &&
        jack_tokenizer_->keyword() == "var") {
        compileVarDec();

        if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
        else throw analyze_exception("The token does not exist. It needs 'var' or keyword for statement, or '}'.");
    }

    /* Print statements. It can be empty statements in this implementation step.
       (Jack Program need 'return' statement, so it cannot be empty statements). */
    compileStatements();

    /* Print '}'. */
    printSymbol(); // It must be '}'.

    --indent_depth_;
    printIndent();
    *output_ << "</subroutineBody>" << std::endl;
}

/**
 * Compile the var declaration.
 */
void CompilationEngine::compileVarDec() {
    printIndent();
    *output_ << "<varDec>" << std::endl;
    ++indent_depth_;

    /* Print 'var'. */
    printKeyword(); // It must be 'var'.

    /* Print type and varName. */
    checkAndPrintTypeAndVarName();

    /* Print ',' varName until ';'. */
    checkAndPrintCommaAndVarName(';');

    /* Print ';'. */
    printSymbol(); // It must be ';'.

    --indent_depth_;
    printIndent();
    *output_ << "</varDec>" << std::endl;
}

/**
 * Compiles a series of statements. Does not include '{}'.
 * Caution: Token that tokenizer moudle points to at first need to be checked.
 *          When this function returns, jack tokenizer module points to next token.
 *          Therefore, token must be used before advance().
 *          It can be empty statements in this implementation step.
 *          (Jack Program need 'return' statement, so it cannot be empty statements).
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
    printIndent();
    *output_ << "<statements>" << std::endl;
    ++indent_depth_;

    /* Check statement and print. */
    while (true) {
        if (jack_tokenizer_->tokenType() == TokenType::SYMBOL &&
            jack_tokenizer_->symbol() == '}') break;
        
        if (jack_tokenizer_->tokenType() != TokenType::KEYWORD)
            throw analyze_exception("Required symbol('}') or keyword for statement.");

        if (jack_tokenizer_->keyword() == "let") compileLet();
        else if (jack_tokenizer_->keyword() == "if") compileIf();
        else if (jack_tokenizer_->keyword() == "while") compileWhile();
        else if (jack_tokenizer_->keyword() == "do") compileDo();
        else if (jack_tokenizer_->keyword() == "return") compileReturn();
        else throw analyze_exception("Required symbol('}') or keyword for statement.");
        
        if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
        else throw analyze_exception("The token does not exist. It needs symbol('}') or keyword for statement.");
    }

    --indent_depth_;
    printIndent();
    *output_ << "</statements>" << std::endl;
}

/**
 * Compile the do statement.
 */
void CompilationEngine::compileDo() {

}

/**
 * Compile the let statement.
 */
void CompilationEngine::compileLet() {

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

}

/**
 * Compile an term.
 */
void CompilationEngine::compileTerm() {

}

/**
 * Compiles a comma-separated list of expressions (which can be an empty list)
 */
void CompilationEngine::compileExpressionList() {

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
    
    if (jack_tokenizer_->tokenType() == TokenType::KEYWORD && 
        jack_tokenizer_->keyword() == "class") compileClass();
    else throw analyze_exception("The first syntax must be class.");

    if (jack_tokenizer_->hasMoreTokens()) throw analyze_exception("Only one class must exist in one file.");
}