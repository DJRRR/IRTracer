#include "DerivedInstrInfo.h"
#include "Tracer.h"

using namespace tracer;

bool CallInstrInfo::instrument(TracerPass* tracer, bool isFirstBlock, bool isFirstInstr){
    basicInstrument(tracer, isFirstBlock, isFirstInstr);
    if(tracer->getTraceMode() == TracerPass::TraceMode::TRACE_FUNC_LEVEL){
        llvm::IRBuilder<> IRB(this->instr);
        IRB.CreateCall(tracer->logCallDepthInc);
    }
    return true;
    
}

bool RetInstrInfo::instrument(TracerPass* tracer, bool isFirstBlock, bool isFirstInstr){
    basicInstrument(tracer, isFirstBlock, isFirstInstr);
    if(tracer->getTraceMode() == TracerPass::TraceMode::TRACE_FUNC_LEVEL){
        llvm::IRBuilder<> IRB(this->instr);
        IRB.CreateCall(tracer->logCallDepthDec);
    }
    return true;
}
