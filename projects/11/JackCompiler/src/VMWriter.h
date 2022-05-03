/**
 * VM Writier Module(Class)
 * Output using VM command syntax.
 *
 * Interface
 * - setOutputFile
 * - writePush
 * - writePop
 * - writeArithmetic
 * - writeLabel
 * - writeGoto
 * - writeIf
 * - writeCall
 * - writeFunction
 * - writeReturn
 * - close
 *
 */

#ifndef __VM_WRITER_H__
#define __VM_WRITER_H__

#include "Global.h"

class VMWriter {
private:
    std::ofstream output_;

    bool isJackFile(const std::string& path) const;

public:
    VMWriter();
    ~VMWriter();

    void setOutputFile(const std::string& path);
    void close();

    void writePush(const Segment segment, const int index);
    void writePop(const Segment segment, const int index);
    void writeArithmetic(const Command command);
    void writeLabel(const std::string& label);
    void writeGoto(const std::string& label);
    void writeIf(const std::string& label);
    void writeCall(const std::string& name, const int nArgs);
    void writeFunction(const std::string& name, const int nLocals);
    void writeReturn();
};

#endif