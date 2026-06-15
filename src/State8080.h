#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MEMORY_SIZE 65536

typedef struct State8080 State8080;

typedef union {
  struct {
    uint8_t l;
    uint8_t h;
  };
  uint16_t hl;
} RegisterCouple;

typedef uint8_t (*OpcodeHandler)(State8080* state);

struct State8080 {
  uint8_t reg_a;

  RegisterCouple reg_bc;
  RegisterCouple reg_de;
  RegisterCouple reg_hl;

  uint8_t reg_flag;

  uint16_t pc;
  uint16_t sp;

  uint8_t memory[MEMORY_SIZE];

  OpcodeHandler instruction_set[256];
};

void Init8080(State8080* state);

uint16_t EmulateCycle(State8080* state);

uint8_t* GetRegisterPointer(State8080* state, uint8_t reg_code);
uint16_t* GetRegisterPair(State8080* state, uint8_t rp);
void UpdateZeroAndSignFlags(State8080* state, uint8_t result);
void GetRegister(State8080* state, uint8_t index);
bool GetConditionStatus(State8080* state, uint8_t val);

// opcodes
uint8_t Opcode_NOP(State8080* state);
uint8_t Opcode_NotImplemented(State8080* state);

uint8_t Opcode_MVI_R(State8080* state);
uint8_t Opcode_MOV_R1_R2(State8080* state);
uint8_t Opcode_LXI_RP_DATA(State8080* state);
uint8_t Opcode_STA_ADDR(State8080* state);
uint8_t Opcode_LDA_ADDR(State8080* state);
uint8_t Opcode_STAX_RP(State8080* state);
uint8_t Opcode_LDAX_RP(State8080* state);
uint8_t Opcode_SHLD_ADDR(State8080* state);
uint8_t Opcode_LHDL_ADDR(State8080* state);
uint8_t Opcode_XCHG(State8080* state);

uint8_t Opcode_ADD_R_M(State8080* state);
uint8_t Opcode_ADC_R_M(State8080* state);
uint8_t Opcode_Sub_R_M(State8080* state);
uint8_t Opcode_SBI_R_M(State8080* state);

uint8_t Opcode_ANA_R_M(State8080* state);
uint8_t Opcode_ANI_DATA(State8080* state);
uint8_t Opcode_XRA_R_M(State8080* state);
uint8_t Opcode_XRI_DATA(State8080* state);
uint8_t Opcode_ORA_R_M(State8080* state);
uint8_t Opcode_ORI_DATA(State8080* state);
uint8_t Opcode_CPI_DATA(State8080* state);
uint8_t Opcode_CMP_R_M(State8080* state);
uint8_t Opcode_CMA(State8080* state);
uint8_t Opcode_CMC(State8080* state);
uint8_t Opcode_STC(State8080* state);
uint8_t Opcode_RLC(State8080* state);
uint8_t Opcode_RRC(State8080* state);
uint8_t Opcode_RAL(State8080* state);
uint8_t Opcode_RAR(State8080* state);