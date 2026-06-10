#pragma once

#include <stdint.h>

#define MEMORY_SIZE 65536

typedef union {
  struct {
    uint8_t l;
    uint8_t h;
  };
  uint16_t lh;
} RegisterCouple;

typedef uint8_t (*OpcodeHandler)(State8080* state);

typedef struct {
  uint8_t reg_a;

  RegisterCouple reg_bc;
  RegisterCouple reg_de;
  RegisterCouple reg_hl;

  uint8_t reg_flag;

  uint16_t pc;
  uint16_t sp;

  uint8_t memory[MEMORY_SIZE];

  OpcodeHandler instruction_set[256];
} State8080;

void Init8080(State8080* state);

uint16_t EmulateCycle(State8080* state);

// opcodes

uint8_t Opcode_NOP(State8080* state);
uint8_t Opcode_NotImplemented(State8080* state);