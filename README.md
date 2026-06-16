# Intel 8080 CPU Emulator

A C-based emulator for the Intel 8080 microprocessor, implementing the complete instruction set and machine control operations.

## Overview

This project implements a fully functional Intel 8080 CPU emulator capable of executing 8080 machine code. The emulator includes:

- Complete 64KB memory simulation
- Full instruction set implementation (244+ opcodes)
- CPU state management with registers and flags
- I/O port handlers (256 input and 256 output ports)
- Interrupt handling and HALT state support
- CP/M BIOS support for console I/O operations

## The Intel 8080

The Intel 8080 was an 8-bit microprocessor released in 1974, serving as the successor to the 8008. It was widely used in early personal computers and embedded systems, including the Altair 8800. Key characteristics:

- **8-bit data bus** with 16-bit address bus
- **64KB addressable memory**
- **6 general-purpose registers** (A, B, C, D, E, H, L)
- **Flag register** (Sign, Zero, Parity, Carry, Auxiliary Carry)
- **Instructions**: ~244 opcodes covering data transfer, arithmetic, logical, branch, and machine control operations

## Features Implemented

### Core Architecture ✅

- **CPU State Structure** (`State8080`)
  - 8-bit registers: A, B, C, D, E, H, L
  - 16-bit register pairs: BC, DE, HL
  - Program Counter (PC) and Stack Pointer (SP)
  - Flag Register (Sign, Zero, Parity, Carry, Auxiliary Carry)
  - 64KB memory
  - Opcode dispatch table
  - I/O port handlers (256 input, 256 output)
  - HALT and Interrupt Enable states

### Instruction Groups ✅

#### Data Transfer (63+ opcodes)
- Register-to-register moves (`MOV r1, r2`)
- Immediate data loads (`MVI r, d8`)
- Register pair loads (`LXI rp, d16`)
- Memory operations (`STAX`, `LDAX`, `SHLD`, `LHLD`, `STA`, `LDA`)
- Register exchange (`XCHG`)

#### Arithmetic & Logical (80+ opcodes)
- Addition: `ADD`, `ADC`, `ADI`, `ACI`
- Subtraction: `SUB`, `SBB`, `SUI`, `SBI`
- Logical AND: `ANA`, `ANI`
- Logical XOR: `XRA`, `XRI`
- Logical OR: `ORA`, `ORI`
- Compare: `CMP`, `CPI`
- Increment/Decrement: `INR`, `DCR`, `INX`, `DCX`
- Double-add: `DAD`
- Rotate and shift: `RLC`, `RRC`, `RAL`, `RAR`, `CMA`
- Decimal adjust: `DAA`
- Flag operations: `STC`, `CMC`

#### Branch Control (16+ opcodes)
- Unconditional jump: `JMP`
- Conditional jumps: `JC`, `JNC`, `JZ`, `JNZ`, `JP`, `JM`, `JPE`, `JPO`
- Unconditional call: `CALL`
- Conditional calls: `CC`, `CNC`, `CZ`, `CNZ`, `CP`, `CM`, `CPE`, `CPO`
- Return: `RET` (unconditional and conditional variants)
- Restart: `RST`
- PC HL load: `PCHL`
- No operation: `NOP`

#### Stack & Machine Control (18+ opcodes)
- Push: `PUSH B`, `PUSH D`, `PUSH H`, `PUSH PSW`
- Pop: `POP B`, `POP D`, `POP H`, `POP PSW`
- Stack operations: `XTHL`, `SPHL`
- I/O operations: `IN d8`, `OUT d8`
- Interrupt control: `EI` (Enable Interrupts), `DI` (Disable Interrupts)
- Halt: `HLT`

### Status

- **Total Opcodes**: 244 implemented, 12 reserved for future use
- **Test Program**: Runs with CPUTEST.COM diagnostic ROM
- **CP/M Compatibility**: Supports CP/M BIOS console I/O calls

## Building

### Requirements

- CMake 3.16 or later
- C11-compliant compiler (GCC, Clang, or MSVC)
- Standard C library

### Build Steps

```bash
# Clone the repository
git clone <repository-url>
cd i8080-emulator

# Create build directory and configure
cmake -S . -B build

# Build the project
cmake --build build -j

# The executable will be at: ./build/i8080_emulator
```

### Build Options

The project uses strict compiler flags for C89/C99/C11 compatibility:
- `-Wall -Wextra -Wpedantic` for GCC/Clang

## Running

```bash
# From the build directory, copy the test ROM and run
cp build/CPUTEST.COM .
./build/i8080_emulator
```

The emulator runs in a continuous loop, executing CPU cycles. It implements CP/M BIOS console operations:

- **BDOS Call 2**: Output single character in register E
- **BDOS Call 9**: Output string from memory (until '$' delimiter) from address in DE register pair

## Project Structure

```
i8080-emulator/
├── CMakeLists.txt              # Build configuration
├── README.md                   # This file
├── build/                      # Build output directory
└── src/
    ├── main.c                  # Entry point and ROM loader
    └── cpu/
        ├── State8080.h         # CPU state structure and declarations
        ├── State8080.c         # CPU initialization and utilities
        ├── arithmetic_group.c  # Arithmetic operations (ADD, SUB, etc.)
        ├── logical_group.c     # Logical operations (AND, OR, XOR, etc.)
        ├── datatransfer_group.c # Data transfer operations (MOV, LXI, etc.)
        ├── branch_group.c      # Branch/jump operations (JMP, CALL, RET, etc.)
        └── machine_control_group.c # Stack, I/O, and control operations
```

## API Reference

### Initialization

```c
void Init8080(State8080* state);
```
Initializes the CPU state, clears memory, and binds all opcode handlers to the instruction set.

### Execution

```c
uint16_t EmulateCycle(State8080* state);
```
Executes a single CPU cycle:
1. Fetches the opcode from memory at PC
2. Increments PC
3. Calls the opcode handler
4. Returns the number of machine cycles consumed

### Utility Functions

```c
uint8_t* GetRegisterPointer(State8080* state, uint8_t reg_code);
uint16_t* GetRegisterPair(State8080* state, uint8_t rp);
void UpdateZeroAndSignFlags(State8080* state, uint8_t result);
bool GetConditionStatus(State8080* state, uint8_t val);
```

## Usage Example

```c
#include "cpu/State8080.h"

int main() {
    // Allocate and initialize CPU
    State8080* cpu = (State8080*)malloc(sizeof(State8080));
    Init8080(cpu);
    
    // Load ROM into memory at offset 0x0100
    LoadROM(cpu, "CPUTEST.COM", 0x0100);
    
    // Set program counter to ROM start
    cpu->pc = 0x0100;
    
    // Execute cycles
    while (1) {
        EmulateCycle(cpu);
    }
    
    free(cpu);
    return 0;
}
```

## Implementation Notes

- **Opcode Dispatch**: The emulator uses a dispatch table with function pointers for efficient opcode execution
- **Register Pairs**: BC, DE, and HL are implemented as unions for both 16-bit and 8-bit access
- **Flags**: The flag register stores Sign, Zero, Parity, Auxiliary Carry, and Carry flags
- **Memory**: Linear 64KB address space supporting both code and data
- **I/O**: Extensible I/O port system with 256 configurable input and output handlers

## Compiler Support

- **GCC**: 9.x and later
- **Clang**: 10.x and later
- **MSVC**: Visual Studio 2019 and later

## Future Enhancements

- Cycle-accurate timing
- Interrupt system implementation
- Extended I/O device simulation
- Debugger with breakpoint support
- Disassembler for machine code analysis
- Performance optimizations and JIT compilation

## References

- Intel 8080 User's Manual
- Intel 8080/8085 Assembly Language Programming
- CP/M Operating System Documentation

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

---

**Last Updated**: 2026-06-16  
**Project Status**: Active Development - Core instruction set fully implemented
