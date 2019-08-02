cmake_minimum_required(VERSION 2.8)

project(LLVM-TRACER)

#### cmake scripts ####
set(SCRIPT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake-scripts")
include("${SCRIPT_DIR}/TracerConfig.cmake")
include("${SCRIPT_DIR}/findAndSetLLVM.cmake")
#### Register LLVM Pass (do Analysis & Instrumentation) ####
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tracer_pass")
#### Implement Log Functions to be instrumented ####
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/logutils")

