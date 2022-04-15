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

/* =========== PUBLIC ============= */

JackAnalyzer::JackAnalyzer(const std::string& path) {
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
    // 입력 데이터를 JackTokenizer 모듈로 토큰화한다.
    // 토큰들을 CompilationEngine 모듈에 전달해 컴파일한 후 출력 메시지를 전달한다.
}