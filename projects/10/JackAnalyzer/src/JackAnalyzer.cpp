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
    /* Tokenize the input data using the JackTokenizer module. */
    jack_tokenizer_->tokenize(path);

    /* Make XXXM.xml output file and ready to write. */
    setOutputFile(path);

    /* Compiled and printed using the CompilationEngine module. */
    compilation_engine_->compile(jack_tokenizer_.get(), &output_);
}

/**
 * Set XXXM.xml output file. Input file(XXX.jack) must be .jack file.
 */
void JackAnalyzer::setOutputFile(std::string path) {
    if (output_.is_open()) output_.close();
    if (!isJackFile(path)) throw file_exception(path);
    path.erase(path.find(".jack"), std::string::npos);
    path.append("M.xml");
    output_.open(path);
    if (output_.fail()) throw file_exception(path);
}

/**
 * Symbol output member function according to XML convention. This shall be included in the Compilation Engine module.
 * @param symbol Token of type symbol.
 * @return Return symbols according to xml convention.
 */
// std::string JackAnalyzer::changeSymboltoXml(const char& symbol) const {
//     if (symbol == '<') return "&lt;";
//     if (symbol == '>') return "&gt;";
//     if (symbol == '\"') return "&quot;";
//     if (symbol == '&') return "&amp;";
//     return std::string(1, symbol);
// }

/* =========== PUBLIC ============= */

/**
 * Creates a jack parser module. Prepare to parse all .jack files in a given program with param.
 * @param path Path to the program you want to parse.
 */
JackAnalyzer::JackAnalyzer(const std::string& path)
: jack_tokenizer_(new JackTokenizer()), compilation_engine_(new CompilationEngine()) {
    loadFilePaths(path);
}

JackAnalyzer::~JackAnalyzer() {
    if (output_.is_open()) output_.close();
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