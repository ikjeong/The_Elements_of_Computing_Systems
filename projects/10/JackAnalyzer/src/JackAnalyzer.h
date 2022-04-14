/**
    JackAnalyzer

    Interface: 
    - translate:
        translate .jack to .xml file.

*/

#ifndef __JACK_ANALYZER_H__
#define __JACK_ANALYZER_H__

#include "Global.h"

class JackAnalyzer {
private:
    std::vector<std::string> paths_;

    void loadFilePaths(const std::string& path);
    bool isJackFile(const std::string& path) const;

public:
    JackAnalyzer(const std::string& path);
    ~JackAnalyzer();
    void translate();
};

#endif