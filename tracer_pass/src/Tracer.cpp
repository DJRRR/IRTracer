#include "Tracer.h"
#include "Debug.h"
#include "InstrInfo.h"
#include "DerivedInstrInfo.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;


#define QUICK_REGISTER(x, y) instrInfoFactory.registerType<x>(y)  
#define DEFAULT_REGISTER(x) \
    for(int i = 1; i <= 64; i++){ \
        if(!instrInfoFactory.ifRegistered(i)){ \
            instrInfoFactory.registerType<DefaultInstrInfo>(i); \
        } \
    } \

void TracerPass::initFactory(){
    /* Opcode range: [1,64], see llvm/IR/Instruction.def */
    QUICK_REGISTER(CallInstrInfo, llvm::Instruction::Call);
    QUICK_REGISTER(StoreInstrInfo, llvm::Instruction::Store);
    QUICK_REGISTER(LoadInstrInfo, llvm::Instruction::Load);
    /* unsupported opcodes */
    DEFAULT_REGISTER(DefaultInstrInfo);
}


bool TracerPass::runOnFunction(llvm::Function &f){
    bool modified = false;
	/* TODO: filter some functions maybe */
    /* TODO: set cmd option to specify the task */
    if(f.getName() == "log_func"){
		return false;
	}
    for(auto iter = f.begin(); iter != f.end(); iter++){
        llvm::BasicBlock& bb = *iter;
        modified = runOnBasicBlock(bb);
    }
    return false;
}

bool TracerPass::runOnBasicBlock(llvm::BasicBlock &BB){
    /* instrumentation */
    llvm::BasicBlock::iterator insertp = BB.getFirstInsertionPt();
    for(llvm::BasicBlock::iterator iter = insertp; iter != BB.end(); iter++){
        llvm::Instruction* I = llvm::dyn_cast<llvm::Instruction>(iter);
        llvm::Function* logger = llvm::dyn_cast<llvm::Function>(logFunc);
        InstrInfo* instrInfo = instrInfoFactory.create(I);
        instrInfo->instrument(logger);
        tracer_message("line number: %d\n", instrInfo->getLineNumber());
    }
    return true;
}

bool TracerPass::doInitialization(llvm::Module &M){
    /* init all trace functions to be instrumented */
    auto VoidTy = llvm::Type::getVoidTy(M.getContext());
    logFunc = M.getOrInsertFunction("log_func", llvm::FunctionType::get(VoidTy, false));
    return false;
}


char TracerPass::ID = 0;

static llvm::RegisterPass<TracerPass> X(
  "tracer",                    // Command line argument
  "tracer on IR level",        // Command line desciption
  false,                       // Only looks at CFG
  false                        // Analysis Pass
);
