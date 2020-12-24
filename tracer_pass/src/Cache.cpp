#include "Cache.h"
#include <map>

std::map<std::string, llvm::Value*> globalPtrMap;

llvm::Value* getGlobalPtr(std::string strRef, llvm::IRBuilder<> IRB){
      if(globalPtrMap.find(strRef) != globalPtrMap.end()){
          return globalPtrMap[strRef];
      }
      else{
          llvm::Value* str = IRB.CreateGlobalStringPtr(llvm::StringRef(strRef));
          globalPtrMap.insert(std::map<std::string, llvm::Value*>::value_type (strRef,str));
          return str;
      }
  }

