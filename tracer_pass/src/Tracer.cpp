#include "Tracer.h"
#include "Debug.h"
#include "InstrInfo.h"
#include "DerivedInstrInfo.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"

using namespace llvm;
using namespace tracer;

cl::opt<std::string> traceModeOpt("tm",
                                cl::desc("Trace Mode"),
                                cl::value_desc("line/func/bb"),
                                cl::init("line"));


#define QUICK_REGISTER(x, y) instrInfoFactory.registerType<x>(y)  
#define DEFAULT_REGISTER(x) \
    for(int i = 1; i <= 64; i++){ \
        if(!instrInfoFactory.ifRegistered(i)){ \
            instrInfoFactory.registerType<DefaultInstrInfo>(i); \
        } \
    } \


uint8_t TracerPass::getTraceMode(){
    return traceMode;
}

void TracerPass::initOpt(){
    std::string tmpOpt = traceModeOpt;
    if(!std::strcmp(tmpOpt.c_str(), "line")){
        traceMode = TraceMode::TRACE_LINE_LEVEL;
    }
    else if(!std::strcmp(tmpOpt.c_str(), "func")){
        traceMode = TraceMode::TRACE_FUNC_LEVEL;
    }
    else if(!std::strcmp(tmpOpt.c_str(), "bb")){
        traceMode = TraceMode::TRACE_BB_LEVEL;
    }
    else{
        tracer_error("Unsupported Trace Mode. Check it.");
    }
}


void TracerPass::initFactory(){
    /* Opcode range: [1,64], see llvm/IR/Instruction.def */
    QUICK_REGISTER(CallInstrInfo, llvm::Instruction::Call);
    QUICK_REGISTER(StoreInstrInfo, llvm::Instruction::Store);
    QUICK_REGISTER(LoadInstrInfo, llvm::Instruction::Load);
    QUICK_REGISTER(RetInstrInfo, llvm::Instruction::Ret);
    /* unsupported opcodes */
    DEFAULT_REGISTER(DefaultInstrInfo);
}


bool TracerPass::runOnFunction(llvm::Function &f){
    bool modified = false;
	/* TODO: filter some functions maybe */
    /* TODO: set cmd option to specify the task */
    /* TODO: write a fiter-dict rather than dozens of if-else*/
    if(f.getName() == "log_line_level" || f.getName() == "log_func_level" || 
            f.getName() == "call_depth_inc" || f.getName() == "call_depth_dec"){
		return false;
	}
    for(auto iter = f.begin(); iter != f.end(); iter++){
        llvm::BasicBlock& bb = *iter;
        modified = runOnBasicBlock(bb, iter == f.begin());
    }
    return false;
    
}


bool TracerPass::runOnBasicBlock(llvm::BasicBlock &BB, bool isFirstBlock){
    /* instrumentation */
    llvm::BasicBlock::iterator insertp = BB.getFirstInsertionPt();
    for(llvm::BasicBlock::iterator iter = insertp; iter != BB.end(); iter++){
        llvm::Instruction* I = dyn_cast<llvm::Instruction>(iter);
        InstrInfo* instrInfo = instrInfoFactory.create(I);
        /* instrument each instruction */ 
        instrInfo->instrument(this, isFirstBlock, iter == BB.getFirstInsertionPt());
    }
    return true;
}

bool TracerPass::doInitialization(llvm::Module &M){
    /* init all trace functions to be instrumented */
    auto VoidTy = llvm::Type::getVoidTy(M.getContext());
    llvm::Type* typeListLine[1] = {llvm::Type::getInt32Ty(M.getContext())};
    logLineLevel = M.getOrInsertFunction("log_line_level", llvm::FunctionType::get(VoidTy, ArrayRef<Type*>(typeListLine,1), false));
    
    llvm::Type* typeListFunc[1] = {llvm::Type::getInt8PtrTy(M.getContext())};
    logFuncLevel = M.getOrInsertFunction("log_func_level", llvm::FunctionType::get(VoidTy, ArrayRef<Type*>(typeListFunc,1), false));
    
    logCallDepthInc = M.getOrInsertFunction("call_depth_inc", llvm::FunctionType::get(VoidTy, false));
    logCallDepthDec = M.getOrInsertFunction("call_depth_dec", llvm::FunctionType::get(VoidTy, false));

    return false;
}


char TracerPass::ID = 0;

static llvm::RegisterPass<TracerPass> X(
  "tracer",                    // Command line argument
  "tracer on IR level",        // Command line desciption
  false,                       // Only looks at CFG
  false                        // Analysis Pass
);
