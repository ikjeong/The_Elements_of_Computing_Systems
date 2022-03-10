/**
    VM Translator

    Function: 
    - constructor:
        Argument is .asm file path or directory.
        
    - translate:
        tranlaste .vm to .asm file.

*/

#ifndef __VM_TRANSLATOR_H__
#define __VM_TRANSLATOR_H__

#include "Global.h"
#include "Parser.h"
#include "CodeWriter.h"

class VMtranslator {
private:
    Parser* parser_;
    CodeWriter* code_writer_;
    std::vector<std::string> paths_;

    void loadFilePaths(const std::string& path);
    void translateFile(const std::string& path);
    bool isVMFile(const std::string& path) const;

public:
    VMtranslator(const std::string& path);
    ~VMtranslator();
    void translate();
};

#endif