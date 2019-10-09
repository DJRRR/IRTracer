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

namespace tracer {

class InstrInfo;
template<typename T>class InstrInfoFactory;

class TracerPass : public llvm::FunctionPass {
    public:

        enum TraceMode : uint8_t {
            TRACE_LINE_LEVEL = 1,
            TRACE_FUNC_LEVEL,
            TRACE_BB_LEVEL
        };

		static char ID;
        TracerPass() : llvm::FunctionPass(ID){ initFactory(); initOpt(); };
        virtual bool runOnFunction(llvm::Function &f);
        virtual bool doInitialization(llvm::Module &M);
        virtual bool runOnBasicBlock(llvm::BasicBlock &BB, bool isFirstBlock);
        llvm::Value* logLineLevel; // trace the execution at source-line level
        llvm::Value* logFuncLevel; // trace the execution at function level
        llvm::Value* logBBLevel; // trace the execution at Basic-block level
        llvm::Value* logCallDepthInc; // record the depth of call path(+)
        llvm::Value* logCallDepthDec; // record the depth of call path(-)

        uint8_t getTraceMode();

    private:
        InstrInfoFactory<InstrInfo> instrInfoFactory;
        void initFactory();
        void initOpt();
        uint8_t traceMode;

};

} // end namespace tracer
#endif /* __TRACER_H__ */
