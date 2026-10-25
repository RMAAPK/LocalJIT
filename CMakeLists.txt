cmake_minimum_required(VERSION 3.20)
project(PolyWarp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optimization Flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-O3 -march=native -Wall -Wextra -pthread)
endif()

include_directories(include)

# Core Library Sources
set(POLYWARP_SOURCES
    src/cpu/arm64_decoder.cpp
    src/cpu/x86_emitter.cpp
    src/cpu/jit_memory.cpp
    src/gpu/shader_hook.cpp
    src/ipfs/vault_client.cpp
)

add_library(polywarp_core STATIC ${POLYWARP_SOURCES})

# CLI Executable
add_executable(polywarp src/main.cpp)
target_link_libraries(polywarp PRIVATE polywarp_core pthread)

# Unit Tests
enable_testing()
add_executable(test_cpu tests/test_cpu_translation.cpp)
target_link_libraries(test_cpu PRIVATE polywarp_core pthread)
add_test(NAME TestCPU COMMAND test_cpu)
