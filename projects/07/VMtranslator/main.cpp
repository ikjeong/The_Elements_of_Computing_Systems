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

#define string_end std::string::npos
typedef std::string::size_type string_iter;


const std::vector<std::string> ARITHMETIC_COMMAND = {
    /* need to sort */
    "add",
    "sub",
    "neg",
    "eq",
    "gt",
    "lt",
    "and",
    "or",
    "not"
};

class Parser {
private:
    std::ifstream input_;
    std::string current_command_;
    std::string next_command_;
    CommandType type_;
    std::string arg1_;
    std::string arg2_;

    bool isEmptyCommand(const std::string& command) const {
        return (static_cast<int>(command.size()) == 0);
    }

    bool isArithmeticCommand(const std::string& command) const {
        auto iter = std::find(ARITHMETIC_COMMAND.begin(), ARITHMETIC_COMMAND.end(), command);
        return iter != ARITHMETIC_COMMAND.end();
    }

    CommandType checkCommandType(const std::string& command) const {
        if (isArithmeticCommand(command)) return CommandType::C_ARITHMETIC;
        if (command == "push") return CommandType::C_PUSH;
        if (command == "pop") return CommandType::C_POP;
        return CommandType::NOTHING;
    }

    void clearTypeAndArgs() {
        type_ = CommandType::NOTHING;
        arg1_ = "";
        arg2_ = "";
    }

    void parseCurrentCommand() {
        clearTypeAndArgs();

        std::stringstream ss(current_command_);
        ss.str(current_command_);
        
        std::string word;
        if (ss >> word) type_ = checkCommandType(word);
        if (type_ == CommandType::C_ARITHMETIC) {
            arg1_ = word;
            arg2_ = "";
        } else {
            if (ss >> word) arg1_ = word;
            if (ss >> word) arg2_ = word;
        }

        if (ss >> word) makeError("Too many arguments");
    }

    void deleteComment(std::string& command) {
        string_iter pos = command.find("//");
        if (pos != string_end) command.erase(pos, string_end);
    }

    std::string readCommand() {
        std::string buffer = "";
        while (!input_.eof()) {
            std::getline(input_, buffer);
            deleteComment(buffer);
            if (!isEmptyCommand(buffer)) return buffer;
        }
        return buffer;
    }

    void init() {
        current_command_ = "";
        next_command_ = readCommand();
        parseCurrentCommand();
    }

public:
    Parser(std::string path) {
        input_.open(path);
        if (input_.fail()) makeError("File OPEN ERROR: " + path);
        init();
    }
    
    ~Parser() {
        input_.close();
    }

    bool hasMoreCommands() const {
        if (isEmptyCommand(next_command_)) return false;
        else return true;
    }

    void advance() {
        current_command_ = next_command_;
        next_command_ = readCommand();
        parseCurrentCommand();
    }

    CommandType commandType() const {
        return type_;
    }

    std::string arg1() const {
        return arg1_;
    }

    std::string arg2() const {
        return arg2_;
    }
};

int main(int argc, char* argv[]) {
    
    Parser* p = new Parser(argv[1]);


    while (p->hasMoreCommands()) {
        p->advance();
        if (p->commandType() == CommandType::C_ARITHMETIC) std::cout << "ARITHMETIC";
        else if (p->commandType() == CommandType::C_PUSH) std::cout << "PUSH";
        else if (p->commandType() == CommandType::C_POP) std::cout << "POP"; 
        std::cout << " ";
        std::cout << p->arg1() << " ";
        std::cout << p->arg2() << std::endl;
    }
    
    delete p;

    return 0;
}