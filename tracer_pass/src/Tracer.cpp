#include "Tracer.h"
#include "Debug.h"
#include "InstrInfo.h"
#include "DerivedInstrInfo.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;


void TracerPass::initFactory(){
    instrInfoFactory.registerType<CallInstrInfo>(llvm::Instruction::Call);
    instrInfoFactory.registerType<StoreInstrInfo>(llvm::Instruction::Store);
    instrInfoFactory.registerType<LoadInstrInfo>(llvm::Instruction::Load);
}


bool TracerPass::runOnFunction(llvm::Function &f){
    bool modified = false;
	/* TODO: filter some functions maybe */
    /* TODO: set cmd option to specify the task */
    if(f.getName() != "main"){
		return false;
	}
    for(auto iter = f.begin(); iter != f.end(); iter++){
        llvm::BasicBlock& bb = *iter;
        modified |= runOnBasicBlock(bb);
    }
    return false;
}

bool TracerPass::runOnBasicBlock(llvm::BasicBlock &BB){
    /* instrumentation */
    llvm::BasicBlock::iterator insertp = BB.getFirstInsertionPt();
    llvm::Instruction* I = llvm::dyn_cast<llvm::Instruction>(insertp);
	llvm::Function* testLogger = llvm::dyn_cast<llvm::Function>(logFunc);
    llvm::Instruction* testI = (llvm::Instruction*)(I->getNextNode()->getNextNode());
    InstrInfo* instrInfo = instrInfoFactory.create(testI->getOpcode(), testI);
    instrInfo->instrument(testLogger);
    tracer_message("line number : %d\n", instrInfo->getLineNumber());
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
