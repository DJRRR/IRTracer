#ifndef __TRACER_H__
#define __TRACER_H__

#include <fstream>
#include <map>
#include <string>

#include "InstrInfoFactory.h"
#include "InstrInfo.h"

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/IRBuilder.h"

class TracerPass : public llvm::FunctionPass {
    public:
		static char ID;
        TracerPass() : llvm::FunctionPass(ID){ initFactory(); };
        virtual bool runOnFunction(llvm::Function &f);
        virtual bool doInitialization(llvm::Module &M);
        virtual bool runOnBasicBlock(llvm::BasicBlock &BB);
        
        InstrInfoFactory<InstrInfo> instrInfoFactory;

        void initFactory();

    private:
        llvm::Value* logFunc;

};

#endif /* __TRACER_H__ */
