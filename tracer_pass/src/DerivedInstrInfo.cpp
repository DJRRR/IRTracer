#include "DerivedInstrInfo.h"
#include "InstrInfo.h"
#include "Tracer.h"
#include "Debug.h"
#include "llvm-c/Core.h"
#include <map>
#include "Cache.h"

using namespace tracer;
using namespace llvm;


bool CallInstrInfo::instrument(TracerPass* tracer, bool isFirstBlock, bool isFirstInstr){
    basicInstrument(tracer, isFirstBlock, isFirstInstr);
    /*if(tracer->getTraceMode() == TracerPass::TraceMode::TRACE_FUNC_LEVEL){
        llvm::IRBuilder<> IRB(this->instr);
        IRB.CreateCall(tracer->logCallDepthInc);
    }*/
    //ignore the externel call
    if(tracer->getTraceMode() == TracerPass::TraceMode::TRACE_FUNC_LEVEL || tracer->getTraceMode() == TracerPass::TraceMode::TRACE_LINE_LEVEL){
        llvm::IRBuilder<> IRB(this->instr);
        llvm::Value* funcStr = getGlobalPtr(this->instr->getFunction()->getName(), IRB);//IRB.CreateGlobalStringPtr(this->instr->getFunction()->getName());
        llvm::StringRef* strRef = new llvm::StringRef(this->getFileName());
        llvm::Value* fileStr = getGlobalPtr(*strRef, IRB);//IRB.CreateGlobalStringPtr(*strRef);
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
    if(tracer->getTraceMode() == TracerPass::TraceMode::TRACE_LINE_LEVEL){
        //mark the boundary
        llvm::IRBuilder<> IRB(this->instr);
        Value* str = getGlobalPtr(this->instr->getFunction()->getName(), IRB);//IRB.CreateGlobalStringPtr(this->instr->getFunction()->getName());
        llvm::Value* valueList[1] = {str};
        IRB.CreateCall(tracer->logLineFuncEnd, ArrayRef<Value*>(valueList,1));
    }
    return true;
}
