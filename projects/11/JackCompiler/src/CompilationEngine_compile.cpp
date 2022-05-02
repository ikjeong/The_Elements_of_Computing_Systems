/**
 * Implementation of CompilationEngine.h
 * Implement the member functions associated with the compilation.
 */

#include "CompilationEngine.h"

/* =========== PRIVATE ============= */

/**
 * Advance Tokenizer.
 * @param expectedToken when fail to advance, throw exception with message includes expected token.
 */
void CompilationEngine::advance(const std::string& expectedToken) {
    if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
    else throw compile_exception("Next token does not exist. Compiler expects " + expectedToken + std::string("."));
}

/**
 * All compileXXX() member functions must have
 * the tokenization module pointing to the token
 * that fits XXX before being called.
 * 
 * It may be empty, but if it must be called at least once, 
 * it may not be pointing to the correct token.
 *
 * However, even at this time, it must point to a token that has not yet been processed.
 */

/**
 * Compile the entire class.
 * 
 * Class: 'class' className '{' classVarDec* subroutineDec* '}'
 */
void CompilationEngine::compileClass() {
    printStartTag("class");

    /* Print 'class'. */
    printKeyword(); // It must be 'class'.

    /* Print className. */
    advance("identifier for className");
    checkAndPrintIdentifier(VarKind::CLASS);

    /* Print '{'. */
    advance("symbol('{')");
    checkAndPrintSymbol('{');

    /* Print classVarDec, subroutineDec. */
    while (true) {
        advance("symbol('}'), or keyword for classVarDec or subroutineDec.");

        if (checkSymbol('}')) break;
        
        if (checkVarDecKeyword()) compileClassVarDec();
        else if (checkSubroutineDecKeyword()) compileSubroutine();
        else throw compile_exception("Expected symbol('}') or keyword(classVarDec or subroutineDec)", jack_tokenizer_->getCurrentTokenLineNumber());
    }

    /* Print '}'. */
    printSymbol(); // It must be '}'.

    printEndTag("class");
}

/**
 * Compile static or field declarations.
 * 
 * VarDec: ('static' | 'field') type varName (',' varName)* ';'
 */
void CompilationEngine::compileClassVarDec() {
    printStartTag("classVarDec");

    /* Print 'static' or 'field'. */
    printKeyword(); // It must be 'static' or 'field'.
    VarKind varKind = stringToVarKind(jack_tokenizer_->getToken());

    /* Print type and varName. */
    advance("type");
    checkAndPrintType();
    std::string type = jack_tokenizer_->getToken();

    /* Define new var. */
    advance("identifier for varName");
    checkAndDefineIdentifier(type, varKind);
    checkAndPrintIdentifier(varKind);

    /* Print ',' varName until ';'. */
    while (true) {
        advance("symbol(';') or symbol(',')");

        if (checkSymbol(';')) break;

        try { checkAndPrintSymbol(','); }
        catch(compile_exception& e) { throw compile_exception("Expected symbol(';') or symbol(',')", jack_tokenizer_->getCurrentTokenLineNumber()); }
                
        advance("identifier for varName");
        checkAndDefineIdentifier(type, varKind);
        checkAndPrintIdentifier(varKind);
    }

    /* Print ';'. */
    printSymbol(); // It must be ';'.

    printEndTag("classVarDec");
}

/**
 * Compiles an entire method, function, or constructor.
 * 
 * subroutineDec: ('constructor' | 'function' | 'method') ('void' | type) subroutineName
 *                '(' parameterList ')' subroutineBody
 */
void CompilationEngine::compileSubroutine() {
    printStartTag("subroutineDec");

    /* Print 'constructor' or 'function', or 'method'. */
    printKeyword(); // It must be 'constructor' or 'function', or 'method'.

    /* Print type or 'void'. */
    try {
        advance("type");
        checkAndPrintType();
    } catch (compile_exception& e) {
        if (checkKeyword("void")) printKeyword();
        else throw compile_exception("Expected 'void' or type(primitive type or className)", jack_tokenizer_->getCurrentTokenLineNumber());
    }
    
    /* Print subroutineName. */
    advance("identifier for subroutineName");
    checkAndPrintIdentifier(VarKind::SUBROUTINE);

    /* Print '('. */
    advance("symbol('(')");
    checkAndPrintSymbol('(');

    /* Start Symbol Table for subroutine. */
    symbol_table_->startSubroutine();

    /* Print parameterList. */
    advance("type for parameter or symbol ')'");
    compileParameterList();
    
    /* Print ')'. */
    advance("symbol(')')");
    checkAndPrintSymbol(')');

    /* Print subroutineBody. */
    advance("symbol('{')");
    if (checkSymbol('{')) compileSubroutineBody();
    else throw compile_exception("Expected symbol('{')", jack_tokenizer_->getCurrentTokenLineNumber());

    printEndTag("subroutineDec");
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
       If first token isn't type, empty parameterList. */
    if (!checkType()) {
        jack_tokenizer_->retreat();
        printEndTag("parameterList");
        return;
    }
    
    /* Print type and varName. */
    std::string type = jack_tokenizer_->getToken();
    checkAndPrintType();

    advance("identifier for varName");
    checkAndDefineIdentifier(type, VarKind::ARGUMENT);
    checkAndPrintIdentifier(VarKind::ARGUMENT);

    /* Print ',' type, varName. */
    advance("symbol for closing parameterList or symbol(',') for more parameter");
    while (checkSymbol(',')) {
        /* If symbol(',') exists, type and token(identifier(varName)) must exist. */
        checkAndPrintSymbol(',');

        /* Print type and varName. */
        advance("type and varName for more parameter");
        std::string type = jack_tokenizer_->getToken();
        checkAndPrintType();

        advance("identifier for varName");
        checkAndDefineIdentifier(type, VarKind::ARGUMENT);
        checkAndPrintIdentifier(VarKind::ARGUMENT);

        advance("symbol for closing parameterList or symbol(',') for more parameter");
    }
    jack_tokenizer_->retreat();

    printEndTag("parameterList");
}

/**
 * Compile the subroutine body.
 * 
 * subroutineBody: '{' varDec* statements '}'
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
 * 
 * varDec: 'var' type varName (',' varName)* ';'
 */
void CompilationEngine::compileVarDec() {
    printStartTag("varDec");

    /* Print 'var'. */
    printKeyword(); // It must be 'var'.

    /* Print type and varName. */
    advance("type and identifier for varName");
    std::string type = jack_tokenizer_->getToken();
    checkAndPrintType();

    advance("identifier for varName");
    checkAndDefineIdentifier(type, VarKind::VAR);
    checkAndPrintIdentifier(VarKind::VAR);

    /* Print ',' varName until ';'. */
    while (true) {
        advance("symbol(';') or symbol(',')");

        if (checkSymbol(';')) break;

        try { checkAndPrintSymbol(','); }
        catch(compile_exception& e) { throw compile_exception("Expected symbol(';') or symbol(',')", jack_tokenizer_->getCurrentTokenLineNumber()); }
                
        advance("identifier for varName");
        checkAndDefineIdentifier(type, VarKind::VAR);
        checkAndPrintIdentifier(VarKind::VAR);
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
 */
void CompilationEngine::compileStatements() {
    printStartTag("statements");

    /* Check statement and print. */
    while (true) {
        if (checkKeyword("let")) compileLet();
        else if (checkKeyword("if")) compileIf();
        else if (checkKeyword("while")) compileWhile();
        else if (checkKeyword("do")) compileDo();
        else if (checkKeyword("return")) compileReturn();
        else break;
        
        advance("symbol for closing statements or keyword for statement");
    }
    jack_tokenizer_->retreat();

    printEndTag("statements");
}

/**
 * Compile the do statement.
 * 
 * doStatement: 'do' subroutineCall ';'
 */
void CompilationEngine::compileDo() {
    printStartTag("doStatement");

    /* Print 'do'. */
    printKeyword(); // It must be 'do'.

    /* Print subroutineCall. */
    advance("identifier for className or varName, or subroutineName");
    checkAndPrintSubroutineCall();

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
    checkAndPrintIdentifier(VarKind::VARNAME);

    /* If token is '[', print '[' expression ']'. If token is '=', print it. */
    advance("symbol('[') or symbol('=')");

    if (checkSymbol('[')) {
        /* Print '['. */
        printSymbol();

        /* Print expression. */
        advance("expression");
        if (checkTerm()) compileExpression();
        else throw compile_exception("Expected expression.", jack_tokenizer_->getCurrentTokenLineNumber());

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
    if (checkTerm()) compileExpression();
    else throw compile_exception("Expected expression.", jack_tokenizer_->getCurrentTokenLineNumber());

    /* Print ';'. */
    advance("symbol(';')");
    checkAndPrintSymbol(';');

    printEndTag("letStatement");
}

/**
 * compile the while statement
 * 
 * whileStatement: 'while' '(' expression ')' '{' statements '}'
 */
void CompilationEngine::compileWhile() {
    printStartTag("whileStatement");

    /* Print 'while'. */
    printKeyword(); // It must be 'while'.

    /* Print '('. */
    advance("symbol('(')");
    checkAndPrintSymbol('(');

    /* Print expression. */
    advance("expression");
    if (checkTerm()) compileExpression();
    else throw compile_exception("Expected expression.", jack_tokenizer_->getCurrentTokenLineNumber());

    /* Print ')'. */
    advance("symbol(')')");
    checkAndPrintSymbol(')');

    /* Print '{'. */
    advance("symbol('{')");
    checkAndPrintSymbol('{');

    /* Print statements. It can be empty statements. */
    advance("keyword for statement, or symbol('}')");
    compileStatements();

    /* Print '}'. */
    advance("symbol('}')");
    checkAndPrintSymbol('}');

    printEndTag("whileStatement");
}

/**
 * Compile the return statement.
 * 
 * returnStatement: 'return' expression? ';'
 */
void CompilationEngine::compileReturn() {
    printStartTag("returnStatement");

    /* Print 'return'. */
    printKeyword(); // It must be 'return'.

    /* Print expression and ';' when expression exist, or only print ';'. */
    advance("expression or symbol(';')");
    if (checkTerm()) {
        compileExpression();
        advance("symbol(';')");
    }
    try { checkAndPrintSymbol(';'); }
    catch (compile_exception& e) { throw compile_exception("Expected expression or symbol(';')", jack_tokenizer_->getCurrentTokenLineNumber()); }

    printEndTag("returnStatement");
}

/**
 * Compile the if statement and the following else clause.
 * 
 * ifStatement: 'if' '(' expression ')' '{' statements '}'
 *              ('else' '{' statements '}')?
 */
void CompilationEngine::compileIf() {
    printStartTag("ifStatement");

    /* Print 'if'. */
    printKeyword(); // It must be 'if'.

    /* Print '('. */
    advance("symbol('(')");
    checkAndPrintSymbol('(');

    /* Print expression. */
    advance("expression");
    if (checkTerm()) compileExpression();
    else throw compile_exception("Expected expression.", jack_tokenizer_->getCurrentTokenLineNumber());

    /* Print ')'. */
    advance("symbol(')')");
    checkAndPrintSymbol(')');

    /* Print '{'. */
    advance("symbol('{')");
    checkAndPrintSymbol('{');

    /* Print statements. It can be empty statements. */
    advance("keyword for statement, or symbol('}')");
    compileStatements();

    /* Print '}'. */
    advance("symbol('}')");
    checkAndPrintSymbol('}');

    /* If keyword 'else' exists, print else statement. */
    advance("keyword('else')");
    if (!checkKeyword("else")) {
        jack_tokenizer_->retreat();
        printEndTag("ifStatement");
        return;
    }
    printKeyword();

    /* Print '{'. */
    advance("symbol('{')");
    checkAndPrintSymbol('{');

    /* Print statements. It can be empty statements. */
    advance("keyword for statement, or symbol('}')");
    compileStatements();

    /* Print '}'. */
    advance("symbol('}')");
    checkAndPrintSymbol('}');

    printEndTag("ifStatement");
}

/**
 * Compile an expression.
 * 
 * expression: term (op term)*
 */
void CompilationEngine::compileExpression() {
    printStartTag("expression");
    
    /* Check term and print. */
    compileTerm();

    /* If op exists, print op and term. */
    advance("symbol for op or symbol for closing expression or expressionList.");
    while (checkOp()) {
        printSymbol();

        advance("term");
        if (checkTerm()) compileTerm();
        else throw compile_exception("Expected term.", jack_tokenizer_->getCurrentTokenLineNumber());

        advance("symbol for op or symbol for closing expression or expressionList.");
    }
    jack_tokenizer_->retreat();

    printEndTag("expression");
}

/**
 * Compile an term.
 * 
 * term: integerConstant | stringConstant | keywordConstant |
 *       varName | varName '[' expression ']' | subroutineCall |
 *       '(' expression ')' unaryOp term
 */
void CompilationEngine::compileTerm() {
    printStartTag("term");

    /* Print term. It must be term. */
    if (checkIntegerConstant()) {
        /* Print integerConstant. */
        printIntegerConstant();
    } else if (checkStringConstant()) {
        /* Print stringConstant. */
        printStringConstant();
    } else if (checkKeywordConstant()) {
        /* Print keywordConstant. */
        printKeyword();
    } else if (checkUnaryOp()) {
        /* Print unaryOp. */
        printSymbol();

        /* Print term. */
        advance("term");
        if (checkTerm()) compileTerm();
        else throw compile_exception("Expected term.", jack_tokenizer_->getCurrentTokenLineNumber());
    } else if (checkSymbol('(')) {
        /* Print '('. */
        printSymbol();

        /* Print expression. */
        advance("expression");
        if (checkTerm()) compileExpression();
        else throw compile_exception("Expected expression.", jack_tokenizer_->getCurrentTokenLineNumber());

        /* Print ')'. */
        advance("symbol(')')");
        checkAndPrintSymbol(')');
    } else if (jack_tokenizer_->tokenType() == TokenType::IDENTIFIER) {
        advance("symbol for closing term or symbol('['), or symbol('(' or '.') for subroutineCall");
        if (checkSymbol('[')) {
            /* Print varName. */
            jack_tokenizer_->retreat();
            checkAndPrintIdentifier(VarKind::VARNAME);

            /* Print '['. */
            advance("symbol('[')");
            checkAndPrintSymbol('[');

            /* Print expression. */
            advance("expression");
            if (checkTerm()) compileExpression();
            else throw compile_exception("Expected expression.", jack_tokenizer_->getCurrentTokenLineNumber());

            /* Print ']'. */
            advance("symbol(']')");
            checkAndPrintSymbol(']');
        } else if (checkSymbol('(') || checkSymbol('.')) {
            /* Print subroutineCall. */
            jack_tokenizer_->retreat();
            checkAndPrintSubroutineCall();
        } else {
            /* Print varName. */
            jack_tokenizer_->retreat();
            checkAndPrintIdentifier(VarKind::VARNAME);
        }
    } else {
        throw compile_exception("Invalid function call. Debug whether the token element is term before calling compileTerm().");
    }

    printEndTag("term");
}

/**
 * Compiles a comma-separated list of expressions (which can be an empty list)
 * Caution: Token that tokenizer moudle points to at first need to be checked.
 *          Because it can be empty statements.
 */
void CompilationEngine::compileExpressionList() {
    printStartTag("expressionList");

    /* If empty expressionList, return. */
    if (!checkTerm()) {
        jack_tokenizer_->retreat();
        printEndTag("expressionList");
        return;
    }

    /* Print expression. */
    compileExpression();

    /* If symbol(',') exists, print ',' and expression. */
    advance("symbol(',') or symbol for closing expressionList.");
    while (checkSymbol(',')) {
        printSymbol();

        advance("expression");
        if (checkTerm()) compileExpression();
        else throw compile_exception("Expected expression.", jack_tokenizer_->getCurrentTokenLineNumber());

        advance("symbol(',') or symbol for closing expressionList.");
    }
    jack_tokenizer_->retreat();
    
    printEndTag("expressionList");
}

/* =========== PUBLIC ============= */

/**
 * Set up the tokenization module and output file, and start the compilation output.
 * @param jackTokenizer The module that has completed tokenization must be delivered.
 * @param output Specifies the file to be compiled output. It must be in the .xml file format.
 */
void CompilationEngine::compile(JackTokenizer* jackTokenizer, SymbolTable* symbolTable, std::ofstream* output) {
    initialize(jackTokenizer, symbolTable, output);

    if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
    else throw compile_exception("The token does not exist. Is it jack file?");
    
    if (checkKeyword("class")) compileClass();
    else throw compile_exception("The first syntax must be class.", jack_tokenizer_->getCurrentTokenLineNumber());

    if (jack_tokenizer_->hasMoreTokens()) throw compile_exception("Only one class must exist in one file.");
}