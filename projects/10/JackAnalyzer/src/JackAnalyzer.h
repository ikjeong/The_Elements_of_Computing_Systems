/**
    JackAnalyzer

    Interface: 
    - translate:
        translate .jack to .xml file.

*/

#ifndef __JACK_ANALYZER_H__
#define __JACK_ANALYZER_H__

#include "Global.h"
#include "JackTokenizer.h"

class JackAnalyzer {
private:
    std::unique_ptr<JackTokenizer> jackTokenizer;
    std::vector<std::string> paths_;

    void loadFilePaths(const std::string& path);
    bool isJackFile(const std::string& path) const;
    void translateFile(const std::string& path);
    std::string changeSymboltoXml(const char& symbol) const;

public:
    JackAnalyzer(const std::string& path);
    ~JackAnalyzer();
    void translate();
};

#endif