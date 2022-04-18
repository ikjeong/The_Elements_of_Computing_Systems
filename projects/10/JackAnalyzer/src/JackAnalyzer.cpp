/**
    Implementation of JackAnalyzer.h
*/

#include "JackAnalyzer.h"

/* =========== PRIVATE ============= */

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

bool JackAnalyzer::isJackFile(const std::string& path) const {
    return path.find(".jack") != std::string::npos;
}

void JackAnalyzer::translateFile(const std::string& path) {
    // 입력 데이터를 JackTokenizer 모듈로 토큰화한다.
    std::ofstream output_;
    std::string outputPath = path;
    outputPath.erase(outputPath.find(".jack"), std::string::npos);
    outputPath.append("MT.xml");
    output_.open(outputPath);
    if (output_.fail()) throw file_exception(outputPath);

    jackTokenizer->setFile(path);
    output_ << "<tokens>" << std::endl;
    while (jackTokenizer->hasMoreTokens()) {
        jackTokenizer->advance();
        if (jackTokenizer->tokenType() == TokenType::KEYWORD) output_ << "<keyword> " << jackTokenizer->keyword() << " </keyword>" << std::endl;
        else if (jackTokenizer->tokenType() == TokenType::SYMBOL) output_ << "<symbol> " << changeSymboltoXml(jackTokenizer->symbol()) << " </symbol>" << std::endl;
        else if (jackTokenizer->tokenType() == TokenType::IDENTIFIER) output_ << "<identifier> " << jackTokenizer->identifier() << " </identifier>" << std::endl;
        else if (jackTokenizer->tokenType() == TokenType::INT_CONST) output_ << "<integerConstant> " << jackTokenizer->intVal() << " </integerConstant>" << std::endl;
        else if (jackTokenizer->tokenType() == TokenType::STRING_CONST) output_ << "<stringConstant> " << jackTokenizer->stringVal() << " </stringConstant>" << std::endl;
        else if (jackTokenizer->tokenType() == TokenType::NOTHING) throw translate_exception("Incorrect Token Type");
    }
    output_ << "</tokens>" << std::endl;
    // 토큰들을 CompilationEngine 모듈에 전달해 컴파일한 후 출력 메시지를 전달한다.
}

// XML관례에 따른 기호 출력 멤버함수. compilationEngine 모듈에 포함되어야 함
std::string JackAnalyzer::changeSymboltoXml(const char& symbol) const {
    if (symbol == '<') return "&lt;";
    if (symbol == '>') return "&gt;";
    if (symbol == '\"') return "&quot;";
    if (symbol == '&') return "&amp;";
    return std::string(1, symbol);
}

/* =========== PUBLIC ============= */

JackAnalyzer::JackAnalyzer(const std::string& path)
: jackTokenizer(new JackTokenizer()) {
    // path에 포함된 jack파일들 경로를 불러와 저장한다.
    loadFilePaths(path);
    // for (auto p : paths_) {
    //     std::cout << p << std::endl;
    // }
}

JackAnalyzer::~JackAnalyzer() {
}

void JackAnalyzer::translate() {
    if (paths_.empty()) throw file_exception("No .jack files");
    for (auto path : paths_) {
        translateFile(path);
    }
}