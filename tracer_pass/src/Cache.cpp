#include "Cache.h"
#include <map>

std::map<llvm::StringRef, llvm::Value*> globalPtrMap;

llvm::Value* getGlobalPtr(llvm::StringRef strRef, llvm::IRBuilder<> IRB){
      if(globalPtrMap.find(strRef) != globalPtrMap.end()){
          return globalPtrMap[strRef];
      }
      else{
          llvm::Value* str = IRB.CreateGlobalStringPtr(strRef);
          globalPtrMap.insert(std::map<llvm::StringRef, llvm::Value*>::value_type (strRef,str));
          return str;
      }
  }

