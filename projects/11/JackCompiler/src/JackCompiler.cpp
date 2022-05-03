/**
 * Implementation of JackCompiler.h
 */

#include "JackCompiler.h"

/* =========== PRIVATE ============= */

/**
 * All jack files in the program to be compiled are stored in the member variable path_.
 * @param path The path of the program to be compiled.
 */
void JackCompiler::loadFilePaths(const std::string& path) {
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
bool JackCompiler::isJackFile(const std::string& path) const {
    return path.find(".jack") != std::string::npos;
}

/**
 * Compile the file given by param.
 * v1: Use the SymbolTable module to classify identifiers.
 *
 * @param path Set the path to the file to be compiled(Must be a .jack file).
 */ 
void JackCompiler::compileFile(const std::string& path) {
    /* Tokenize the input data using the JackTokenizer module. */
    jack_tokenizer_->tokenize(path);

    /* Initalize Symbol Table. */
    symbol_table_->resetTable();

    /* Set Output File. */
    vm_writer_->setOutputFile(path);

    /* Compiled and printed using the CompilationEngine module. */
    compilation_engine_->compile(jack_tokenizer_.get(), symbol_table_.get(), vm_writer_.get());
}

/* =========== PUBLIC ============= */

/**
 * Creates a jack parser module. Prepare to parse all .jack files in a given program with param.
 * @param path Path to the program you want to parse.
 */
JackCompiler::JackCompiler(const std::string& path)
: jack_tokenizer_(new JackTokenizer()), symbol_table_(new SymbolTable()),
  compilation_engine_(new CompilationEngine()), vm_writer_(new VMWriter()) {
    root_path_= path;
    loadFilePaths(path);
}

JackCompiler::~JackCompiler() {
}

/**
 * Compile all .jack files in the program and saves them as .vm files.
 */
void JackCompiler::compile() {
    if (paths_.empty()) throw file_exception(root_path_ + ", Error: There is no .jack file.");
    for (auto path : paths_) {
        compileFile(path);
    }
}