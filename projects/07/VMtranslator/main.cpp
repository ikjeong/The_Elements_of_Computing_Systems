/**
    Main Virtual Machine
    v1: Implement arithmetic command and memory access command.
    todo: Implement program flow command and function calling command.

    Command structure
    command, command arg or command arg1 arg2

    Arithmetic command
    - add
    - sub
    - neg
    - eq
    - gt
    - lt
    - and
    - or
    - not

    Memory access command
    - push segment index
    - pop segment index

    Virtual Memory Segment
    Virtual Machine manipulate virtual memory segments.
    The left side is a memory segment, and the right side is a memory corresponding to the segment.
    '*' is the value indicated by the corresponding memory.
    - argument  *ARG(RAM[2])
    - local     *LCL(RAM[1])
    - static    RAM[16-255]
    - constant  (This segment is emulated.)
    - this      *THIS(RAM[3])
    - that      *THAT(RAM[4])
    - pointer   0: THIS(RAM[3]), 1: THAT(RAM[4])
    - temp      RAM[5-12]
    + Virtualized stack     *SP(RAM[0]) -> RAM[256-2047]

    Modules
    - Parser: After parsing, access to each field is provided.
    - CodeWriter: Returns the binary code for the association symbol.

    How to use
    prompt> VMtranslator source
    source is .vm file or directory which contains .vm files.
    return .asm file.
*/

#include "Global.h"
#include "Parser.h"
#include "CodeWriter.h"



// path분석 
// .vm 파일이면 -> parser 설정 후 codewrite에 작성 명령
// 다른 파일이면 -> 그냥 넘기기
// 디렉터리면 -> 순회

bool isVMFile(const std::string& path) {
    return path.find(".vm") != std::string::npos;
}

void translate(Parser* p, CodeWriter* c, const std::string& path) {
    p->setNewFile(path);
    c->setFileName(path);
    while (p->hasMoreCommands()) {
        p->advance();
        if (p->commandType() == CommandType::C_ARITHMETIC) c->writeArithmetic(p->arg1());
        else if (p->commandType() == CommandType::C_PUSH) c->writePushPop(p->commandType(), p->arg1(), p->arg2());
        else if (p->commandType() == CommandType::C_POP) c->writePushPop(p->commandType(), p->arg1(), p->arg2());
    }
}

int main(int argc, char* argv[]) {
    // std::filesystem::path tt(argv[1]);
    std::vector<std::string> paths;
    if (std::filesystem::is_directory(argv[1])){
        for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(argv[1])) {
            paths.push_back(entry.path());
        }
    } else {
        paths.push_back(argv[1]);
    }

    Parser* p = new Parser();
    CodeWriter* c = new CodeWriter(argv[1]);

    for (auto path : paths) {
        if (std::filesystem::is_directory(path)) continue;
        if (!isVMFile(path)) continue;
        translate(p, c, path);
    }
    
    delete p;
    delete c;

    return 0;
}


