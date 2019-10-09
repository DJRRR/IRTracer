#ifndef __DERIVEDINSTRINFO_H__
#define __DERIVEDINSTRINFO_H__

#include "InstrInfo.h"
#include "llvm/IR/Instruction.h"

namespace tracer {

class InstrInfo;

class CallInstrInfo : public InstrInfo {
    public:
        CallInstrInfo(llvm::Instruction* I) : InstrInfo(I) {};
    private:
        /* TODO: extra info need to be recorded */
        bool instrument(TracerPass* tracer, bool isFirstBlock, bool isFirstInstr);
};

class StoreInstrInfo : public InstrInfo {
    public:
        StoreInstrInfo(llvm::Instruction* I) : InstrInfo(I) {};
    //private:

};

class LoadInstrInfo : public InstrInfo {
    public:
        LoadInstrInfo(llvm::Instruction* I) : InstrInfo(I) {};
    //private:
};


class DefaultInstrInfo : public InstrInfo {
    public:
        DefaultInstrInfo(llvm::Instruction* I) : InstrInfo(I) {};
};

class RetInstrInfo : public InstrInfo {
    public:
        RetInstrInfo(llvm::Instruction* I) : InstrInfo(I) {};
    private:
        bool instrument(TracerPass* tracer, bool isFirstBlock, bool isFirstInstr);
};

} // end namespace tracer
#endif /* __DERIVEDINSTRINFO_H__ */
