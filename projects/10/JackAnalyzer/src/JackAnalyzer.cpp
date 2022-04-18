/**
 * Implementation of JackAnalyzer.h
 */

#include "JackAnalyzer.h"

/* =========== PRIVATE ============= */

/**
 * All jack files in the program to be analyzed are stored in the member variable path_.
 * @param path The path of the program to be analyzed.
 */
void JackAnalyzer::loadFilePaths(const std::string& path) {
    if (std::filesystem::is_directory(path)){
        for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path)) {
            if (std::filesystem::is_directory(entry.path())) continue;
            if (!isJackFile(entry.path())) continue;
            paths_.push_back(entry.path());
        }
    } else {
        if (!isJackFile(path)) return;
        paths_.push_back(path);
    }
}

/**
 * @param path Path to the file to determine if it is a jack file.
 * @return Returns whether the file is a jack file or not.
 */
bool JackAnalyzer::isJackFile(const std::string& path) const {
    return path.find(".jack") != std::string::npos;
}

/**
 * Analyze the file given by param.
 * v1: After tokenization, the token is saved as a .xml file by attaching a keyword to the token. The file name is xxxMT.xml.
 *
 * @param path Set the path to the file to be analyzed(Must be a .jack file).
 */ 
void JackAnalyzer::analyzeFile(const std::string& path) {
    // Specify the file to print the contents. This shall be included in the Complication Engine module.
    std::ofstream output_;
    std::string outputPath = path;
    outputPath.erase(outputPath.find(".jack"), std::string::npos);
    outputPath.append("MT.xml");
    output_.open(outputPath);
    if (output_.fail()) throw file_exception(outputPath);

    // Tokenize the input data using the JackTokenizer module.
    jackTokenizer->setFile(path);
    output_ << "<tokens>" << std::endl;
    while (jackTokenizer->hasMoreTokens()) {
        jackTokenizer->advance();
        if (jackTokenizer->tokenType() == TokenType::KEYWORD) output_ << "<keyword> " << jackTokenizer->keyword() << " </keyword>" << std::endl;
        else if (jackTokenizer->tokenType() == TokenType::SYMBOL) output_ << "<symbol> " << changeSymboltoXml(jackTokenizer->symbol()) << " </symbol>" << std::endl;
        else if (jackTokenizer->tokenType() == TokenType::IDENTIFIER) output_ << "<identifier> " << jackTokenizer->identifier() << " </identifier>" << std::endl;
        else if (jackTokenizer->tokenType() == TokenType::INT_CONST) output_ << "<integerConstant> " << jackTokenizer->intVal() << " </integerConstant>" << std::endl;
        else if (jackTokenizer->tokenType() == TokenType::STRING_CONST) output_ << "<stringConstant> " << jackTokenizer->stringVal() << " </stringConstant>" << std::endl;
        else if (jackTokenizer->tokenType() == TokenType::NOTHING) throw analyze_exception("Incorrect Token Type");
    }
    output_ << "</tokens>" << std::endl;
    output_.close();

    // TODO: The tokens are delivered to the Compilation Engine module. It is then compiled and printed.
}

/**
 * Symbol output member function according to XML convention. This shall be included in the Compilation Engine module.
 * @param symbol Token of type symbol.
 * @return Return symbols according to xml convention.
 */
std::string JackAnalyzer::changeSymboltoXml(const char& symbol) const {
    if (symbol == '<') return "&lt;";
    if (symbol == '>') return "&gt;";
    if (symbol == '\"') return "&quot;";
    if (symbol == '&') return "&amp;";
    return std::string(1, symbol);
}

/* =========== PUBLIC ============= */

/**
 * Creates a jack parser module. Prepare to parse all .jack files in a given program with param.
 * @param path Path to the program you want to parse.
 */
JackAnalyzer::JackAnalyzer(const std::string& path)
: jackTokenizer(new JackTokenizer()) {
    loadFilePaths(path);
}

JackAnalyzer::~JackAnalyzer() {
}

/**
 * Parses all .jack files in the program and saves them as .xml files.
 */
void JackAnalyzer::analyze() {
    if (paths_.empty()) throw file_exception("No .jack files");
    for (auto path : paths_) {
        analyzeFile(path);
    }
}