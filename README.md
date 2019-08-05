# LLVM-Tracer

## Whitzard

## Quick Start
```
mkdir ./build
cd build
cmake ..
make -j8
llvm-link ./test.bc ./logutils/logutils.llvm -o ./x.bc (test.bc compiled with -g option)
opt -load=./tracer_pass/tracer_pass.so -tracer ./x.bc -o ./k.bc (Instrument & Analysis)
```

## Project Status: In Development

## Reference
https://github.com/ysshao/LLVM-Tracer

## Requirement
LLVM 7.0.0
