/**
 * Implementation of CompilationEngine.h
 * Implement the member functions associated with the compilation.
 */

#include "CompilationEngine.h"

/* =========== PRIVATE ============= */

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
    /* Check 'class'. */
    // It must be 'class'.

    /* Check className. */
    advance("identifier for className");
    checkIdentifier(VarKind::CLASS);
    if (file_name_ != jack_tokenizer_->getToken()) throw compile_exception("File name and class name need to be same.");

    /* Check '{'. */
    advance("symbol('{')");
    checkSymbol('{');

    /* Compile classVarDec, subroutineDec. */
    while (true) {
        advance("symbol('}'), or keyword for classVarDec or subroutineDec.");

        if (checkSymbol('}')) break;
        
        if (checkVarDecKeyword()) compileClassVarDec();
        else if (checkSubroutineDecKeyword()) compileSubroutine();
        else throw compile_exception("Expected symbol('}') or keyword(classVarDec or subroutineDec)", jack_tokenizer_->getCurrentTokenLineNumber());
    }

    /* Check '}'. */
    // It must be '}'.
}

/**
 * Compile static or field declarations.
 * 
 * VarDec: ('static' | 'field') type varName (',' varName)* ';'
 */
void CompilationEngine::compileClassVarDec() {
    /* Check 'static' or 'field'. */
    // It must be 'static' or 'field'.
    VarKind varKind = stringToVarKind(jack_tokenizer_->getToken());
    std::string type;

    /* Check type and varName. */
    advance("type");
    checkType();
    type = jack_tokenizer_->getToken();

    /* Define new var. */
    advance("identifier for varName");
    checkAndDefineIdentifier(type, varKind);

    /* Check ',' varName until ';'. */
    while (true) {
        advance("symbol(';') or symbol(',')");

        if (checkSymbol(';')) break;

        try { checkSymbol(','); }
        catch(compile_exception& e) { throw compile_exception("Expected symbol(';') or symbol(',')", jack_tokenizer_->getCurrentTokenLineNumber()); }
                
        advance("identifier for varName");
        checkAndDefineIdentifier(type, varKind);
    }

    /* Check ';'. */
    // It must be ';'.
}

/**
 * Compiles an entire method, function, or constructor.
 * 
 * subroutineDec: ('constructor' | 'function' | 'method') ('void' | type) subroutineName
 *                '(' parameterList ')' subroutineBody
 */
void CompilationEngine::compileSubroutine() {
    /* Check 'constructor' or 'function', or 'method'. */
    // It must be 'constructor' or 'function', or 'method'.
    std::string subroutineType = jack_tokenizer_->getToken();
    std::string subroutineName;

    /* Check return type. */
    try {
        advance("type");
        if (subroutineType == "constructor") {
            checkIdentifier(VarKind::CLASS);
            if (jack_tokenizer_->getToken() != file_name_) throw compile_exception("Constructor return type and class name need to be same.");
        } else {
            checkType();
        }
    } catch (compile_exception& e) {
        if (subroutineType == "constructor") {
            throw compile_exception("Constructor return type and class name need to be same.");
        } else {
            if (!checkKeyword("void")) throw compile_exception("Expected 'void' or type(primitive type or className)", jack_tokenizer_->getCurrentTokenLineNumber());
        }
    }
    
    /* Check subroutineName. */
    advance("identifier for subroutineName");
    checkIdentifier(VarKind::SUBROUTINE);
    subroutineName = jack_tokenizer_->getToken();

    /* Check '('. */
    advance("symbol('(')");
    checkSymbol('(');

    /* Start Symbol Table for subroutine. */
    symbol_table_->startSubroutine();

    /* Compile parameterList. */
    advance("type for parameter or symbol ')'");
    compileParameterList(subroutineType);
    
    /* Check ')'. */
    advance("symbol(')')");
    checkSymbol(')');

    /* Compile subroutineBody. */
    advance("symbol('{')");
    if (checkSymbol('{')) compileSubroutineBody(subroutineType, subroutineName);
    else throw compile_exception("Expected symbol('{')", jack_tokenizer_->getCurrentTokenLineNumber());
}

/**
 * Compile the parameter list (may be an empty list). It does not include '()'.
 * Method's first argument is 'this'.
 * Caution: Token that tokenizer moudle points to at first need to be checked.
 *          Because it can be empty parameterList.
 * 
 * parameterList: ((type varName) (',' type varName)*)?
 */
void CompilationEngine::compileParameterList(const std::string& subroutineType) {
    /* Method's first argument is 'this'. */
    if (subroutineType == "method")
        symbol_table_->define("this", file_name_, VarKind::ARGUMENT);
        // "this" is keyword, so "this" is unique in symbol table.

    /* At first, check token.
       If first token isn't type, empty parameterList. */
    if (!checkType()) {
        jack_tokenizer_->retreat();
        return;
    }
    
    /* Check type and varName. */
    checkType();
    std::string type = jack_tokenizer_->getToken();

    advance("identifier for varName");
    checkAndDefineIdentifier(type, VarKind::ARGUMENT);

    /* Check ',' type, varName. */
    advance("symbol for closing parameterList or symbol(',') for more parameter");
    while (checkSymbol(',')) {
        /* If symbol(',') exists, type and token(identifier(varName)) must exist. */
        checkSymbol(',');

        /* Print type and varName. */
        advance("type and varName for more parameter");
        checkType();
        type = jack_tokenizer_->getToken();

        advance("identifier for varName");
        checkAndDefineIdentifier(type, VarKind::ARGUMENT);

        advance("symbol for closing parameterList or symbol(',') for more parameter");
    }
    jack_tokenizer_->retreat();
}

/**
 * Compile the subroutine body.
 * 
 * subroutineBody: '{' varDec* statements '}'
 */
void CompilationEngine::compileSubroutineBody(const std::string& subroutineType, const std::string& subroutineName) {
    /* Check '{'. */
    // It must be '{'.

    /* When keyword 'var' exists, Compile varDec. */
    advance("'var' or keyword for statement, or symbol('}')");
    while (checkKeyword("var")) {
        compileVarDec();
        advance("'var' or keyword for statement, or symbol('}').");
    }

    /* VM function define. */
    vm_writer_->writeFunction(file_name_ + "." + subroutineName, symbol_table_->varCount(VarKind::VAR));

    if (subroutineType == "method") {
        /* If method, set "this" segment. */
        vm_writer_->writePush(Segment::ARG, 0);
        vm_writer_->writePop(Segment::POINTER, 0);
    } else if (subroutineType == "constructor") {
        /* If constructor, alloc memory. */
        int size = symbol_table_->varCount(VarKind::FIELD);
        vm_writer_->writePush(Segment::CONST, size);
        vm_writer_->writeCall("Memory.alloc", 1);
        /* set "this" segment. */
        vm_writer_->writePop(Segment::POINTER, 0);
    }

    /* Compile statements. It can be empty statements. */
    compileStatements();

    /* Check '}'. */
    advance("symbol('}')");
    checkSymbol('}');
}

/**
 * Compile the var declaration.
 * 
 * varDec: 'var' type varName (',' varName)* ';'
 */
void CompilationEngine::compileVarDec() {
    /* Check 'var'. */
    // It must be 'var'.

    /* Check type and varName. */
    advance("type and identifier for varName");
    checkType();
    std::string type = jack_tokenizer_->getToken();

    advance("identifier for varName");
    checkAndDefineIdentifier(type, VarKind::VAR);

    /* Check ',' varName until ';'. */
    while (true) {
        advance("symbol(';') or symbol(',')");

        if (checkSymbol(';')) break;

        try { checkSymbol(','); }
        catch(compile_exception& e) { throw compile_exception("Expected symbol(';') or symbol(',')", jack_tokenizer_->getCurrentTokenLineNumber()); }
        
        advance("identifier for varName");
        checkAndDefineIdentifier(type, VarKind::VAR);
    }

    /* Check ';'. */
    // It must be ';'.
}

/**
 * Compiles a series of statements. Does not include '{}'.
 * Caution: Token that tokenizer moudle points to at first need to be checked.
 *          Because it can be empty statements.
 * 
 * statements: statement*
 */
void CompilationEngine::compileStatements() {
    /* Compile statement. */
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
}

/**
 * Compile the do statement.
 * 
 * doStatement: 'do' subroutineCall ';'
 */
void CompilationEngine::compileDo() {
    /* Check 'do'. */
    // It must be 'do'.

    /* Compile subroutineCall. */
    advance("identifier for className or varName, or subroutineName");
    checkAndCompileSubroutineCall();

    /* Pop return value. */
    vm_writer_->writeArithmetic(Command::SUB);

    /* Check ';'. */
    advance("symbol(';')");
    checkSymbol(';');
}

/**
 * Compile the let statement.
 * 
 * letStatement: 'let' varName ('[' expression ']')? '=' expression ';'
 */
void CompilationEngine::compileLet() {
    /* Check 'let'. */
    // It must be 'let'.

    std::string varName;
    bool isArray;

    /* Check varName. */
    advance("identifier for varName");
    checkIdentifier(VarKind::VARNAME);
    varName = jack_tokenizer_->getToken();

    /* If token is '[', compile '[' expression ']'. If token is '=', check it. */
    isArray = false;
    advance("symbol('[') or symbol('=')");
    if (checkSymbol('[')) {
        /* Check '['. */
        isArray = true;

        /* Compile expression. */
        advance("expression");
        if (checkTerm()) compileExpression();
        else throw compile_exception("Expected expression.", jack_tokenizer_->getCurrentTokenLineNumber());

        /* Check ']'. */
        advance("symbol(']')");
        checkSymbol(']');
        
        /* Get '='. */
        advance("symbol('=')");
    }

    /* If array, save index to temp 0. */
    if (isArray) vm_writer_->writePop(Segment::TEMP, 0);

    /* Check '='. */
    checkSymbol('=');

    /* Compile expression. */
    advance("expression");
    if (checkTerm()) compileExpression();
    else throw compile_exception("Expected expression.", jack_tokenizer_->getCurrentTokenLineNumber());

    /* Print vm code. */
    VarKind varKind = symbol_table_->kindOf(varName);
    int index = symbol_table_->indexOf(varName);
    if (isArray) {
        pushVariable(varKind, index);               // Push array address.
        vm_writer_->writePush(Segment::TEMP, 0);    // Push index.
        vm_writer_->writeArithmetic(Command::ADD);  // ADD addres
        vm_writer_->writePop(Segment::POINTER, 1);

        vm_writer_->writePop(Segment::THAT, 0);
    } else {
        popToVariable(varKind, index); 
    }

    /* Check ';'. */
    advance("symbol(';')");
    checkSymbol(';');
}

/**
 * compile the while statement
 * 
 * whileStatement: 'while' '(' expression ')' '{' statements '}'
 */
void CompilationEngine::compileWhile() {
    /* Check 'while'. */
    // It must be 'while'.

    /* Set label. */
    int labelIndex = while_label_index_;
    while_label_index_ += 2;
    vm_writer_->writeLabel("whileLabel" + std::to_string(labelIndex));

    /* Check '('. */
    advance("symbol('(')");
    checkSymbol('(');

    /* Check expression. */
    advance("expression");
    if (checkTerm()) compileExpression();
    else throw compile_exception("Expected expression.", jack_tokenizer_->getCurrentTokenLineNumber());

    /* Check ')'. */
    advance("symbol(')')");
    checkSymbol(')');

    /* Set if-goto. */
    vm_writer_->writeArithmetic(Command::NOT);
    vm_writer_->writeIf("whileLabel" + std::to_string(labelIndex+1));

    /* Check '{'. */
    advance("symbol('{')");
    checkSymbol('{');

    /* compile statements. It can be empty statements. */
    advance("keyword for statement, or symbol('}')");
    compileStatements();

    /* Goto first label. */
    vm_writer_->writeGoto("whileLabel" + std::to_string(labelIndex));

    /* Check '}'. */
    advance("symbol('}')");
    checkSymbol('}');

    /* Set label. */
    vm_writer_->writeLabel("whileLabel" + std::to_string(labelIndex+1));
}

/**
 * Compile the return statement.
 * 
 * returnStatement: 'return' expression? ';'
 */
void CompilationEngine::compileReturn() {
    /* Check 'return'. */
    // It must be 'return'.

    /* Compile expression and ';' when expression exist, or only check ';'. */
    advance("expression or symbol(';')");
    if (checkTerm()) {
        compileExpression();
        advance("symbol(';')");
    } else {
        vm_writer_->writePush(Segment::CONST, 0);
    }
    
    /* Write return vm code. */
    vm_writer_->writeReturn();

    try { checkSymbol(';'); }
    catch (compile_exception& e) { throw compile_exception("Expected expression or symbol(';')", jack_tokenizer_->getCurrentTokenLineNumber()); }
}

/**
 * Compile the if statement and the following else clause.
 * 
 * ifStatement: 'if' '(' expression ')' '{' statements '}'
 *              ('else' '{' statements '}')?
 */
void CompilationEngine::compileIf() {
    /* Check 'if'. */
    // It must be 'if'.

    /* Check '('. */
    advance("symbol('(')");
    checkSymbol('(');

    /* compile expression. */
    advance("expression");
    if (checkTerm()) compileExpression();
    else throw compile_exception("Expected expression.", jack_tokenizer_->getCurrentTokenLineNumber());

    /* Check ')'. */
    advance("symbol(')')");
    checkSymbol(')');

    /* Set if-goto. */
    int labelIndex = if_label_index_;
    if_label_index_ += 2;
    vm_writer_->writeArithmetic(Command::NOT);
    vm_writer_->writeIf("ifLabel" + std::to_string(labelIndex));

    /* Check '{'. */
    advance("symbol('{')");
    checkSymbol('{');

    /* Compile statements. It can be empty statements. */
    advance("keyword for statement, or symbol('}')");
    compileStatements();

    /* Set goto. */
    vm_writer_->writeGoto("ifLabel" + std::to_string(labelIndex+1));

    /* Check '}'. */
    advance("symbol('}')");
    checkSymbol('}');

    /* If keyword 'else' exists, check else statement. */
    advance("keyword('else')");
    if (!checkKeyword("else")) {
        /* Set label. */
        vm_writer_->writeLabel("ifLabel" + std::to_string(labelIndex));
        vm_writer_->writeLabel("ifLabel" + std::to_string(labelIndex+1));

        jack_tokenizer_->retreat();
        return;
    }

    /* Check '{'. */
    advance("symbol('{')");
    checkSymbol('{');

    /* Set label. */
    vm_writer_->writeLabel("ifLabel" + std::to_string(labelIndex));

    /* Compile statements. It can be empty statements. */
    advance("keyword for statement, or symbol('}')");
    compileStatements();

    /* Check '}'. */
    advance("symbol('}')");
    checkSymbol('}');

    /* Set label. */
    vm_writer_->writeLabel("ifLabel" + std::to_string(labelIndex+1));
}

/**
 * Compile an expression.
 * 
 * expression: term (op term)*
 */
void CompilationEngine::compileExpression() {
    /* Compile term. */
    compileTerm();

    /* If op exists, compile op and term. */
    advance("symbol for op or symbol for closing expression or expressionList.");
    while (checkOp()) {
        std::string command = jack_tokenizer_->getToken();

        advance("term");
        if (checkTerm()) compileTerm();
        else throw compile_exception("Expected term.", jack_tokenizer_->getCurrentTokenLineNumber());

        if (command == "*") {
            vm_writer_->writeCall("Math.multiply", 2);
        } else if (command == "/") {
            vm_writer_->writeCall("Math.divide", 2);
        } else {
            vm_writer_->writeArithmetic(stringToBinaryOperation(command));
        }

        advance("symbol for op or symbol for closing expression or expressionList.");
    }
    jack_tokenizer_->retreat();
}

/**
 * Compile an term.
 * 
 * term: integerConstant | stringConstant | keywordConstant |
 *       varName | varName '[' expression ']' | subroutineCall |
 *       '(' expression ')' unaryOp term
 */
void CompilationEngine::compileTerm() {
    /* Compile term. It must be term. */
    if (checkIntegerConstant()) {
        /* Push integerConstant. */
        vm_writer_->writePush(Segment::CONST, std::stoi(jack_tokenizer_->getToken()));
    } else if (checkStringConstant()) {
        /* Push stringConstant. */
        std::string stringConstant = jack_tokenizer_->getToken();
        vm_writer_->writePush(Segment::CONST, stringConstant.size());
        vm_writer_->writeCall("String.new", 1);
        for (const auto c : stringConstant) {
            vm_writer_->writePush(Segment::CONST, int(c));
            vm_writer_->writeCall("String.appendChar", 2);
        }
    } else if (checkKeywordConstant()) {
        /* Push keywordConstant. */
        if (checkKeyword("true")) {
            vm_writer_->writePush(Segment::CONST, 1);
            vm_writer_->writeArithmetic(Command::NEG);
        } else if (checkKeyword("false")) {
            vm_writer_->writePush(Segment::CONST, 0);
        } else if (checkKeyword("null")) {
            vm_writer_->writePush(Segment::CONST, 0);
        } else if (checkKeyword("this")) {
            vm_writer_->writePush(Segment::POINTER, 0);
        }
    } else if (checkUnaryOp()) {
        /* Check unaryOp. */
        std::string unaryCommand = jack_tokenizer_->getToken();

        /* Check term. */
        advance("term");
        if (checkTerm()) compileTerm();
        else throw compile_exception("Expected term.", jack_tokenizer_->getCurrentTokenLineNumber());

        /* Write vm code. */
        if (unaryCommand == "~") {
            vm_writer_->writeArithmetic(Command::NOT);
        } else if (unaryCommand == "-") {
            vm_writer_->writeArithmetic(Command::NEG);
        } else {
            throw compile_exception("It isn't unaryOp.", jack_tokenizer_->getCurrentTokenLineNumber());
        }
    } else if (checkSymbol('(')) {
        /* Check '('. */

        /* Compile expression. */
        advance("expression");
        if (checkTerm()) compileExpression();
        else throw compile_exception("Expected expression.", jack_tokenizer_->getCurrentTokenLineNumber());

        /* Check ')'. */
        advance("symbol(')')");
        checkSymbol(')');
    } else if (jack_tokenizer_->tokenType() == TokenType::IDENTIFIER) {
        advance("symbol for closing term or symbol('['), or symbol('(' or '.') for subroutineCall");
        if (checkSymbol('[')) {
            /* Check varName. */
            jack_tokenizer_->retreat();
            checkIdentifier(VarKind::VARNAME);
            std::string varName = jack_tokenizer_->getToken();

            /* Check '['. */
            advance("symbol('[')");
            checkSymbol('[');

            /* Compile expression -> Push Index. */
            advance("expression");
            if (checkTerm()) compileExpression();
            else throw compile_exception("Expected expression.", jack_tokenizer_->getCurrentTokenLineNumber());

            /* Check ']'. */
            advance("symbol(']')");
            checkSymbol(']');

            /* Push Array element. */
            VarKind varKind = symbol_table_->kindOf(varName);
            int index = symbol_table_->indexOf(varName);
            
            pushVariable(varKind, index);               // Push array address.
            vm_writer_->writeArithmetic(Command::ADD);  // ADD addres
            vm_writer_->writePop(Segment::POINTER, 1);

            vm_writer_->writePush(Segment::THAT, 0);
        } else if (checkSymbol('(') || checkSymbol('.')) {
            /* Compile subroutineCall. */
            jack_tokenizer_->retreat();
            checkAndCompileSubroutineCall();
        } else {
            /* Check varName. */
            jack_tokenizer_->retreat();
            checkIdentifier(VarKind::VARNAME);
            std::string varName = jack_tokenizer_->getToken();
            pushVariable(symbol_table_->kindOf(varName), symbol_table_->indexOf(varName));
        }
    } else {
        throw compile_exception("Invalid function call. Debug whether the token element is term before calling compileTerm().");
    }
}

/**
 * Compiles a comma-separated list of expressions (which can be an empty list)
 * Caution: Token that tokenizer moudle points to at first need to be checked.
 *          Because it can be empty statements.
 */
int CompilationEngine::compileExpressionList() {
    int numExpression = 0;

    /* If empty expressionList, return. */
    if (!checkTerm()) {
        jack_tokenizer_->retreat();
        return numExpression;
    }

    /* Compile expression. */
    compileExpression();
    ++numExpression;

    /* If symbol(',') exists, compile ',' and expression. */
    advance("symbol(',') or symbol for closing expressionList.");
    while (checkSymbol(',')) {
        advance("expression");
        if (checkTerm()) compileExpression();
        else throw compile_exception("Expected expression.", jack_tokenizer_->getCurrentTokenLineNumber());
        ++numExpression;

        advance("symbol(',') or symbol for closing expressionList.");
    }
    jack_tokenizer_->retreat();
    
    return numExpression;
}

/* =========== PUBLIC ============= */

/**
 * Set up the tokenization module and output file, and start the compilation output.
 * @param jackTokenizer The module that has completed tokenization must be delivered.
 * @param output Specifies the file to be compiled output. It must be in the .xml file format.
 */
void CompilationEngine::compile(JackTokenizer* jackTokenizer, SymbolTable* symbolTable, VMWriter* vmWriter, const std::string& path) {
    initialize(jackTokenizer, symbolTable, vmWriter, path);

    if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
    else throw compile_exception("The token does not exist. Is it jack file?");
    
    if (checkKeyword("class")) compileClass();
    else throw compile_exception("The first syntax must be class.", jack_tokenizer_->getCurrentTokenLineNumber());

    if (jack_tokenizer_->hasMoreTokens()) throw compile_exception("Only one class must exist in one file.");
}