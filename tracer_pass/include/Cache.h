#ifndef __CACHE_H__
#define __CACHE_H__

#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/ADT/StringRef.h"

llvm::Value* getGlobalPtr(std::string strRef, llvm::IRBuilder<> IRB);


#endif /* __CACHE_H__ */
