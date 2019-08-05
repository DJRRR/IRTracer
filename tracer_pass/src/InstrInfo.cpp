#include "InstrInfo.h"
#include "Debug.h"
#include <assert.h>
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

InstrInfo::InstrInfo(Instruction* I){
    this->instr = I;
    this->opcode = I->getOpcode();
    this->lineNumber = LINE_NUMBER_UNSET;
    setLineNumber();
    setInstrBuf();
    setFuncName();
}

void InstrInfo::setLineNumber(){
	if(!this->instr){
		tracer_error("InstrInfo Constructor(setLineNumber) Error: instr == NULL");	
	}	
    /* set debug meta data */
	MDNode* md = instr->getMetadata("dbg");
    if(md){
        DILocation* Loc = dyn_cast<DILocation>(md);
        this->lineNumber = Loc->getLine();
    }
    else{
        this->lineNumber = LINE_NUMBER_NOEXIST;
    }
}

void InstrInfo::setInstrBuf(){
    if(!this->instr){
		tracer_error("InstrInfo Constructor(setInstrInfo) Error: instr == NULL");	
	}	
    /* set instr buf */
    std::string res("");
    llvm::raw_string_ostream ss(res);
    ss << *this->instr;
    this->instrBuf = new char[res.size()];
   	memcpy(this->instrBuf, res.c_str(), res.size());
}

void InstrInfo::setFuncName(){
	if(!this->instr){
		tracer_error("InstrInfo Constructor(setFuncName) Error: instr == NULL");	
	}
    /* set func name */
    llvm::Function* func = this->instr->getFunction();
    this->funcName = new char[func->getName().size()];
    memcpy(this->funcName, func->getName().data(), func->getName().size());
}

int32_t InstrInfo::getLineNumber(){
    return this->lineNumber;
}

llvm::Instruction* InstrInfo::getInstruction(){
    return this->instr;
}

uint32_t InstrInfo::getOpcode(){
    return this->opcode;
}

char* InstrInfo::getFuncName(){
    return this->funcName;
}

char* InstrInfo::getInstrBuf(){
    return this->instrBuf;
}

bool InstrInfo::instrument(llvm::Function* tracer){
    llvm::IRBuilder<> IRB(this->instr);
    /* TODO: add params to tracer function */
    IRB.CreateCall(tracer);
}
