/**
 * Implementation of CompilationEngine.h
 * Implement the member functions associated with the output.
 */

#include "CompilationEngine.h"

/* =========== PRIVATE ============= */

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



/**
 * subroutineCall: subroutineName '(' expressionList ')' |
 *                 (className | varName) '.' subroutineName '(' expressionList ')'
 */
void CompilationEngine::checkAndCompileSubroutineCall() {
    bool hasDot = false;
    VarKind varKind;
    std::string identifierName = "";
    std::string subroutineName = "";

    /* If token is '.', compile method or function call. If token is '(', just compile own method call. */
    advance("symbol('.') or symbol('(')");
    if (checkSymbol('.')) {
        hasDot = true;
        
        /* Check identifier for className or varName. */
        jack_tokenizer_->retreat();
        std::vector<VarKind> expectedIdentifier = {VarKind::CLASS, VarKind::VARNAME};
        checkIdentifier(expectedIdentifier);

        identifierName = jack_tokenizer_->getToken();
        varKind = symbol_table_->kindOf(identifierName);
        if (varKind == VarKind::NONE) varKind = VarKind::CLASS;

        /* Check '.'. */
        advance("symbol('.')");
        checkSymbol('.');

        /* Check subroutineName. */
        advance("identifier for subroutineName");
        checkIdentifier(VarKind::SUBROUTINE);
        subroutineName = jack_tokenizer_->getToken();
    } else {
        /* Check identifier for subroutineName. */
        jack_tokenizer_->retreat();
        checkIdentifier(VarKind::SUBROUTINE);
        subroutineName = jack_tokenizer_->getToken();
    }

    /* If method call, push pointer. */
    if (hasDot && varKind != VarKind::CLASS) {
        int index = symbol_table_->indexOf(identifierName);
        pushVariable(varKind, index);
    } else if (!hasDot) {
        vm_writer_->writePush(Segment::POINTER, 0);
    }

    /* Check '('. */
    advance("symbol('(')");
    checkSymbol('(');

    /* Compile expressionList. It can be empty expressionList. */
    advance("expressionList or symbol(')')");
    int nArgs = compileExpressionList();

    /* Check ')'. */
    advance("symbol(')')");
    checkSymbol(')');

    /* Write vm code. */
    if (hasDot) {
        if (varKind == VarKind::CLASS)
            vm_writer_->writeCall(identifierName + "." + subroutineName, nArgs);
        else
            vm_writer_->writeCall(symbol_table_->typeOf(identifierName) + "." + subroutineName, nArgs+1);
    } else {
        vm_writer_->writeCall(file_name_ + "." + subroutineName, nArgs+1);
    }
}
