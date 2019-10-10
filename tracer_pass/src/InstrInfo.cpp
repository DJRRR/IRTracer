#include "InstrInfo.h"
#include "Debug.h"
#include <assert.h>
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "Tracer.h"

using namespace llvm;
using namespace tracer;

InstrInfo::InstrInfo(Instruction* I){
    this->instr = I;
    this->opcode = I->getOpcode();
    this->lineNumber = LINE_NUMBER_UNSET;
    this->bb = I->getParent();
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
    this->instrBuf = new char[res.size()+1];
   	memcpy(this->instrBuf, res.c_str(), res.size());
    this->instrBuf[res.size()] = 0;
}

void InstrInfo::setFuncName(){
	if(!this->instr){
		tracer_error("InstrInfo Constructor(setFuncName) Error: instr == NULL");	
	}
    /* set func name */
    llvm::Function* func = this->instr->getFunction();
    this->funcName = new char[func->getName().size()+1];
    memcpy(this->funcName, func->getName().data(), func->getName().size());
    this->funcName[func->getName().size()] = 0;
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

llvm::BasicBlock* InstrInfo::getBasicBlock(){
    return this->bb;
}

char* InstrInfo::getBBOpList(){
    std::string res("");
    for(llvm::BasicBlock::iterator iter = this->bb->begin(); iter != this->bb->end(); iter++){
        llvm::Instruction* I = dyn_cast<llvm::Instruction>(iter);
        uint32_t tmpOpcode = I->getOpcode();
		res += std::to_string(tmpOpcode);
        res += "-";
    }
    char* hash = new char[res.size()+1];
    memcpy(hash, res.c_str(), res.size());
    hash[res.size()] = 0;
	return hash;   
}

/*
 * @isFirstBlock : if is the first basic block of a function
 * @isFirstInstr : if is the first instruction of a basic block
 *
 */

bool InstrInfo::basicInstrument(TracerPass* tracer, bool isFirstBlock, bool isFirstInstr){
    if(tracer->getTraceMode() == TracerPass::TraceMode::TRACE_LINE_LEVEL){
        if(!(this->getLineNumber() == LINE_NUMBER_UNSET || 
                                     this->getLineNumber() == LINE_NUMBER_NOEXIST)){
            llvm::IRBuilder<> IRB(this->instr);
            llvm::Value* valueList[1] = {IRB.getInt32(this->lineNumber)};
            IRB.CreateCall(tracer->logLineLevel, ArrayRef<Value*>(valueList,1));
            return true;
        }
    }
    else if(tracer->getTraceMode() == TracerPass::TraceMode::TRACE_FUNC_LEVEL){
        if(isFirstBlock && isFirstInstr){
            llvm::IRBuilder<> IRB(this->instr);
            Value* str = IRB.CreateGlobalStringPtr(this->instr->getFunction()->getName());
            llvm::Value* valueList[1] = {str};
            IRB.CreateCall(tracer->logFuncLevel, ArrayRef<Value*>(valueList,1));
            return true;
        }
    }
    else if(tracer->getTraceMode() == TracerPass::TraceMode::TRACE_BB_LEVEL){
        if(isFirstInstr){
            llvm::IRBuilder<> IRB(this->instr);
            /* calculate the hash of a basic-block */
            llvm::StringRef* strRef = new llvm::StringRef(getBBOpList());
			Value* str = IRB.CreateGlobalStringPtr(*strRef);
            llvm::Value* valueList[1] = {str};
            IRB.CreateCall(tracer->logBBLevel, ArrayRef<Value*>(valueList, 1));
            return true;
        }
    }
    return false;
}

bool InstrInfo::instrument(TracerPass* tracer, bool isFirstBlock, bool isFirstInstr){
    return basicInstrument(tracer, isFirstBlock, isFirstInstr);
}
