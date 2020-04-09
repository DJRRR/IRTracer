#ifndef __INSTRINFO_H__
#define __INSTRINFO_H__

#define LINE_NUMBER_UNSET -1
#define LINE_NUMBER_NOEXIST -2

#include "llvm/IR/Instruction.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"

namespace tracer{

class TracerPass;

class InstrInfo {
    public:
        void setLineNumber();
        void setInstrBuf();
        void setFuncName();
        void setFileName();
        llvm::Instruction* getInstruction();
        int32_t getLineNumber();
        uint32_t getOpcode();
        char* getFuncName();
        char* getFileName();
        char* getInstrBuf();
        llvm::BasicBlock* getBasicBlock();
        bool basicInstrument(TracerPass* tracer, bool isFirstBlock, bool isFirstInstr);
        char* getBBOpList();

        InstrInfo(llvm::Instruction* I);
        
        /* return false: escaped condition, return true: instrument success*/
        virtual bool instrument(TracerPass* tracer, bool isFirstBlock, bool isFirstInstr);

    protected:
        char *funcName;
        char* fileName;
        int32_t lineNumber;
        char *instrBuf;
        llvm::Instruction* instr;
        uint32_t opcode;
        llvm::BasicBlock* bb;
};

} // end namespace tracer

#endif /* __INSTRINFO_H__ */

