/**
 * Implementation of JackTokenizer.h
 */

#include "JackTokenizer.h"

/* =========== PRIVATE ============= */

void JackTokenizer::initialize() {
    token_.clear();
    current_token_index_ = -1;
    current_token_type_ = TokenType::NOTHING;
}

/**
 * Tokenize the file set in the input stream.
 */
void JackTokenizer::tokenizeFile() {
    bool doesQuotationOpen = false;
    bool doesCommentOpen = false;
    while (!input_.eof()) {
        std::string buffer;
        std::getline(input_, buffer);
        tokenizeLine(buffer, doesQuotationOpen, doesCommentOpen);
    }
}

/**
 * @param buffer a single line of input.
 */
void JackTokenizer::tokenizeLine(std::string& buffer, bool& doesQuotationOpen, bool& doesCommentOpen) {
    int startIndex = 0;

    /* If double quotes or annotations are already open, inspect and process for closure. */
    if (doesQuotationOpen) {
        string_index index = buffer.find("\"");
        if (index == string_end) {
            /* If you cannot close double quotes, 
               modify the last token and receive the following input. */
            token_.back().append(buffer);
            return;
        } else {
            /* If you can close double quotes, 
               close them and prepare for the next token processing. */
            startIndex = index+1;
            token_.back().append(buffer.substr(0, startIndex));
            doesQuotationOpen = false;
        }
    } else if (doesCommentOpen) {
        string_index index = buffer.find("*/");
        if (index == string_end) {
            /* If you cannot close the comment, 
               accept the following input. */
            return;
        } else {
            /* If you can close the annotation, 
               close it and prepare for the next token processing. */
            startIndex = index+2;
            doesCommentOpen = false;
        }
    }

    /* Tokenize the input line. */
    while (startIndex < buffer.size()) {
        std::string token = "";
        for (int endIndex = startIndex; endIndex < buffer.size(); ++endIndex) {
            /* The double quotation surface tokenizes the string created so far
               and starts processing the string. */
            if (buffer[endIndex] == '\"') {
                pushToken(token);

                string_index index = buffer.find("\"", endIndex+1);
                if (index == string_end) {
                    /* If you cannot close double quotes, 
                       receive the following input. */
                    pushToken(buffer.substr(endIndex, string_end));
                    doesQuotationOpen = true;
                    return;
                } else {
                    /* If you can close double quotes, 
                       close them and process tokens. */
                    pushToken(buffer.substr(endIndex, index-endIndex+1));
                    startIndex = index+1;
                    break;
                }
            }

            /* Comment, tokenize the string you have created so far 
               and start processing the annotation. */
            if (endIndex >= 1 && buffer.substr(endIndex-1, 2) == "//") {
                token_.pop_back(); // Remove the symbol token that entered the '/' shape.
                pushToken(token);
                return;
            } else if (endIndex >= 1 && buffer.substr(endIndex-1, 2) == "/*") {
                token_.pop_back(); // Remove the symbol token that entered the '/' shape.
                pushToken(token);

                string_index index = buffer.find("*/", endIndex+1);
                if (index == string_end) {
                    /* If you cannot close comment, 
                       receive the following input. */
                    doesCommentOpen = true;
                    return;
                } else {
                    /* If you can close comment, close it. */
                    startIndex = index+2;
                    break;
                }
            }

            /* When a blank text comes, the token that has been created is tokenized. */
            if (buffer[endIndex] == ' ' || buffer[endIndex] == '\t' || buffer[endIndex] == '\n') {
                pushToken(token);
                startIndex = endIndex+1;
                break;
            }

            /* When a symbol comes, it tokenizes the token it has been created
               and tokenizes the symbol. */
            if (isSymbol(buffer[endIndex])) {
                pushToken(token);
                pushToken(std::string(1, buffer[endIndex]));
                startIndex = endIndex+1;
                break;
            }

            /* Configure the token. */
            token.push_back(buffer[endIndex]);

            /* If you come to the end of the input, tokenize the token. */
            if (endIndex+1 == (int)buffer.size()) {
                pushToken(token);
                startIndex = endIndex+1;
                break;
            }
        }
    } // End Tokenization.
}

/**
 * Put the given token into the member variable token_. If it is an empty token, do not insert it.
 * @param token Tokens to save.
 */
void JackTokenizer::pushToken(const std::string& token) {
    if (token == "") return;
    token_.push_back(token);
}

bool JackTokenizer::isKeyword(const std::string& token) const {
    for (const std::string& keyword : KEYWORDS) {
        if (token == keyword) return true;
    }
    return false;
}

bool JackTokenizer::isSymbol(const char& token) const {
    for (const char& symbol : SYMBOLS) {
        if (token == symbol) return true;
    }
    return false;
}

TokenType JackTokenizer::checkTokenType(const std::string& token) const {
    if (isKeyword(token)) return TokenType::KEYWORD;
    if (token.size() == 1 && isSymbol(token[0])) return TokenType::SYMBOL;
    if (token.size() >= 2 && token[0] == '\"' && token[token.size()-1] == '\"') return TokenType::STRING_CONST;
    try {
        std::stoi(token);
        return TokenType::INT_CONST;
    } catch (std::exception& e) { }
    std::regex re("[_a-zA-Z][_a-zA-Z0-9]*");
    if (std::regex_match(token, re)) return TokenType::IDENTIFIER;

    return TokenType::NOTHING;
}

bool JackTokenizer::isJackFile(const std::string& path) const {
    return path.find(".jack") != std::string::npos;
}

/* =========== PUBLIC ============= */

JackTokenizer::JackTokenizer() {
}

JackTokenizer::~JackTokenizer() {
}

/**
 * Set the input stream, initialize the module, and tokenize the file.
 * To use this module, you must call the tokenize() member function to initialize it.
 */
void JackTokenizer::tokenize(const std::string& path) {
    if (input_.is_open()) input_.close();
    if (!isJackFile(path)) throw file_exception(path);
    input_.open(path);
    if (input_.fail()) throw file_exception(path);
    initialize();
    tokenizeFile();
}

bool JackTokenizer::hasMoreTokens() const {
    if (current_token_index_ < (int)token_.size()-1) return true;
    return false;
}

/**
 * Point to the next token.
 * Called only when hasMoreTokens is ture.
 */
void JackTokenizer::advance() {
    if (!hasMoreTokens()) throw analyze_exception("No more tokens, but advance() is called.");
    ++current_token_index_;
    current_token_type_ = checkTokenType(token_[current_token_index_]);
}

/**
 * Point to the before token.
 * Called only when current_token_index_ excesses 0.
 */
void JackTokenizer::retreat() {
    if (current_token_index_ <= 0) throw analyze_exception("No previous tokens available for lookup, but retreat() is called.");
    --current_token_index_;
    current_token_type_ = checkTokenType(token_[current_token_index_]);
}

/**
 * @return Returns the type of the current token.
 */
TokenType JackTokenizer::tokenType() const {
    return current_token_type_;
}

/**
 * It should only be called when the current token is a keyword.
 * @return Returns the keyword of the current token.
 */
std::string JackTokenizer::keyword() const {
    if (current_token_type_ != TokenType::KEYWORD) throw analyze_exception(token_[current_token_index_]);
    return token_[current_token_index_];
}

/**
 * It should only be called when the current token is a symbol.
 * @return Returns the symbol of the current token.
 */
char JackTokenizer::symbol() const {
    if (current_token_type_ != TokenType::SYMBOL) throw analyze_exception(token_[current_token_index_]);
    return token_[current_token_index_][0];
}

/**
 * It should only be called when the current token is a identifier.
 * @return Returns the identifier of the current token.
 */
std::string JackTokenizer::identifier() const {
    if (current_token_type_ != TokenType::IDENTIFIER) throw analyze_exception(token_[current_token_index_]);
    return token_[current_token_index_];
}

/**
 * It should only be called when the current token is a int constant.
 * @return Returns the int value of the current token.
 */
int JackTokenizer::intVal() const {
    if (current_token_type_ != TokenType::INT_CONST) throw analyze_exception(token_[current_token_index_]);
    return std::stoi(token_[current_token_index_]);
}

/**
 * It should only be called when the current token is a string constant.
 * Double quotation marks are removed.
 * @return Returns the string value of the current token.
 */
std::string JackTokenizer::stringVal() const {
    if (current_token_type_ != TokenType::STRING_CONST) throw analyze_exception(token_[current_token_index_]);
    return token_[current_token_index_].substr(1, token_[current_token_index_].size()-2);
}