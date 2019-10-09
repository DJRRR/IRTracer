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

namespace tracer {

template<typename T>
class InstrInfoFactory{
    public:
        template <typename TDerived>
        void registerType(uint32_t opcode){
            static_assert(std::is_base_of<T, TDerived>::value, "Illegal Opcode!");
            _createFuncs[opcode] = &createFunc<TDerived>;
        }

        T* create(llvm::Instruction* I){
            auto it = _createFuncs.find(I->getOpcode());
            if(it != _createFuncs.end()){
                return it -> second(I);
            }
            tracer_error("Encounter Unregistered Opcode : %u\n", I->getOpcode());
            return NULL;
        }

        bool ifRegistered(uint32_t opcode){
            auto it = _createFuncs.find(opcode);
            if(it != _createFuncs.end()){
                return true;
            }
            return false;
        }

    private:
        template <typename TDerived>
        static T* createFunc(llvm::Instruction* I){
           return new TDerived(I); 
        }

        typedef T* (*PCreateFunc)(llvm::Instruction*);
        std::map<uint32_t, PCreateFunc> _createFuncs;

};

} // end namespace tracer
#endif /* __INSTRINFOFACTORY_H__ */
