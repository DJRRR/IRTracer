#include "Tracer.h"
#include "Debug.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

bool TracerPass::runOnFunction(llvm::Function &f){
    bool modified = false;
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
    llvm::BasicBlock::iterator insertp = BB.getFirstInsertionPt();
    llvm::Instruction* I = llvm::dyn_cast<llvm::Instruction>(insertp);
	llvm::IRBuilder<> IRB(I);
	llvm::Function* testLogger = llvm::dyn_cast<llvm::Function>(logFunc);
	IRB.CreateCall(testLogger);
	return true;
}

bool TracerPass::doInitialization(llvm::Module &M){
    auto VoidTy = llvm::Type::getVoidTy(M.getContext());
    logFunc = M.getOrInsertFunction("log_func", llvm::FunctionType::get(VoidTy, false));
    return false;
}


char TracerPass::ID = 0;

static llvm::RegisterPass<TracerPass> X(
  "mytracer",               // Command line argument
  "Dummy Pass",             // Command line desciption
  false,                    // Only looks at CFG
  false                     // Analysis Pass
);
