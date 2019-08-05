#ifndef __INSTRINFOFACTORY_H__
#define __INSTRINFOFACTORY_H__

#include "InstrInfo.h"
#include "Debug.h"

#include "llvm/IR/Instruction.h"
#include <vector>
#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include <assert.h>

using namespace std;

template<typename T>
class InstrInfoFactory{
    public:
        template <typename TDerived>
        void registerType(uint32_t opcode){
            static_assert(std::is_base_of<T, TDerived>::value, "Illegal Opcode!");
            _createFuncs[opcode] = &createFunc<TDerived>;
        }

        T* create(uint32_t opcode, llvm::Instruction* I){
            auto it = _createFuncs.find(opcode);
            if(it != _createFuncs.end()){
                return it -> second(I);
            }
            tracer_error("Encounter Unregistered Opcode : %u\n", opcode);
            return NULL;
        }

    private:
        template <typename TDerived>
        static T* createFunc(llvm::Instruction* I){
           return new TDerived(I); 
        }

        typedef T* (*PCreateFunc)(llvm::Instruction*);
        map<uint32_t, PCreateFunc> _createFuncs;

};


#endif /* __INSTRINFOFACTORY_H__ */
