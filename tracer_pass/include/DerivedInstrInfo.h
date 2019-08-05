#ifndef __DERIVEDINSTRINFO_H__
#define __DERIVEDINSTRINFO_H__

#include "InstrInfo.h"
#include "llvm/IR/Instruction.h"


class CallInstrInfo : public InstrInfo {
    public:
        CallInstrInfo(llvm::Instruction* I) : InstrInfo(I) {};
    //private:
        /* TODO: extra info need to be recorded */
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



#endif /* __DERIVEDINSTRINFO_H__ */
