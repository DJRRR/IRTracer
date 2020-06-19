#include "DerivedInstrInfo.h"
#include "Tracer.h"
#include "Debug.h"
#include "llvm-c/Core.h"
using namespace tracer;
using namespace llvm;

bool CallInstrInfo::instrument(TracerPass* tracer, bool isFirstBlock, bool isFirstInstr){
    basicInstrument(tracer, isFirstBlock, isFirstInstr);
    /*if(tracer->getTraceMode() == TracerPass::TraceMode::TRACE_FUNC_LEVEL){
        llvm::IRBuilder<> IRB(this->instr);
        IRB.CreateCall(tracer->logCallDepthInc);
    }*/
    if(tracer->getTraceMode() == TracerPass::TraceMode::TRACE_FUNC_LEVEL){
        llvm::IRBuilder<> IRB(this->instr);
        llvm::Value* funcStr = IRB.CreateGlobalStringPtr(this->instr->getFunction()->getName());
        llvm::StringRef* strRef = new llvm::StringRef(this->getFileName());
        llvm::Value* fileStr = IRB.CreateGlobalStringPtr(*strRef);
        llvm::CallInst* callInst = dyn_cast<llvm::CallInst>(this->instr);
        llvm::Function* func = callInst->getCalledFunction();
        //it seems that when func ptr is used, the getCalledFunction would return NULL
        if(func != NULL && func->getName() == "startLog"){
            return false;
        }
        if(func != NULL && func->isIntrinsic()){
            return false;
        }
        /*FIXME@DJR is external function == (getInstrunctionCount= 0)*/
        if(func == NULL || (func != NULL && func->getInstructionCount() != 0)){
            llvm::Value* valueList[4] = {funcStr, fileStr, IRB.getInt32(this->lineNumber), IRB.getInt32(1)};
            IRB.CreateCall(tracer->setCaller, ArrayRef<Value*>(valueList, 4));
        }
        else if(func != NULL && func->getInstructionCount() == 0){
            llvm::Value* valueList[4] = {funcStr, fileStr, IRB.getInt32(this->lineNumber), IRB.getInt32(0)};
            IRB.CreateCall(tracer->setCaller, ArrayRef<Value*>(valueList, 4));
            llvm::IRBuilder<> IRB2(this->instr->getNextNode());
            IRB2.CreateCall(tracer->startLog);
        }
    }
    return true;
    
}

bool RetInstrInfo::instrument(TracerPass* tracer, bool isFirstBlock, bool isFirstInstr){
    basicInstrument(tracer, isFirstBlock, isFirstInstr);
    /*if(tracer->getTraceMode() == TracerPass::TraceMode::TRACE_FUNC_LEVEL){
        llvm::IRBuilder<> IRB(this->instr);
        IRB.CreateCall(tracer->logCallDepthDec);
    }*/
    return true;
}
