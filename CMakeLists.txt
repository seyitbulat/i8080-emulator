cmake_minimum_required(VERSION 3.16)

project(i8080_emulator LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(i8080_emulator
  src/main.c
  src/cpu/State8080.c
  src/cpu/arithmetic_group.c
  src/cpu/branch_group.c
  src/cpu/datatransfer_group.c
  src/cpu/logical_group.c
  src/cpu/machine_control_group.c
)

target_include_directories(i8080_emulator
  PRIVATE
    src
    src/cpu
)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(i8080_emulator PRIVATE -Wall -Wextra -Wpedantic)
endif()
