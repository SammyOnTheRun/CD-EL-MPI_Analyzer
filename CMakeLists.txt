cmake_minimum_required(VERSION 3.10)
project(MPIAnalyzer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM 14 REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_definitions(${LLVM_DEFINITIONS})


add_library(MPIAnalyzer SHARED
  src/main.cpp
  src/MPIAnalyzer.cpp
)
target_link_libraries(MPIAnalyzer 
    clangTooling
    clangBasic
    clangASTMatchers
)
target_include_directories(MPIAnalyzer PRIVATE include)
