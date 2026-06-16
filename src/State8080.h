#pragma once

#include <stdbool.h>
#include <stdint.h>

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
typedef uint8_t (*InputHandler)();
typedef void (*OutputHandler)(uint8_t data);

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

  InputHandler in_devices[256];
  OutputHandler out_devices[256];

  uint8_t internal_state; // msb -> lsb 0000 0HI // H: HALT I: Interrupt Enabled
};

void Init8080(State8080* state);

uint16_t EmulateCycle(State8080* state);

uint8_t* GetRegisterPointer(State8080* state, uint8_t reg_code);
uint16_t* GetRegisterPair(State8080* state, uint8_t rp);
void UpdateZeroAndSignFlags(State8080* state, uint8_t result);
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
uint8_t Opcode_LHLD_ADDR(State8080* state);
uint8_t Opcode_XCHG(State8080* state);

uint8_t Opcode_ADD_R_M(State8080* state);
uint8_t Opcode_ADC_R_M(State8080* state);
uint8_t Opcode_Sub_R_M(State8080* state);
uint8_t Opcode_SBB_R_M(State8080* state);
uint8_t Opcode_ADI_DATA(State8080* state);
uint8_t Opcode_ACI_DATA(State8080* state);
uint8_t Opcode_SUI_DATA(State8080* state);
uint8_t Opcode_SBI_DATA(State8080* state);
uint8_t Opcode_INR_R_M(State8080* state);
uint8_t Opcode_DCR_R_M(State8080* state);
uint8_t Opcode_INX_RP(State8080* state);
uint8_t Opcode_DCX_RP(State8080* state);
uint8_t Opcode_DAD(State8080* state);
uint8_t Opcode_DAA(State8080* state);

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

// Branch group
uint8_t Opcode_JMP_ADDR(State8080* state);
uint8_t Opcode_JUMP_CONDITIONAL_ADDR(State8080* state);
uint8_t Opcode_CALL_ADDR(State8080* state);
uint8_t Opcode_CALL_CONDITIONAL_ADDR(State8080* state);
uint8_t Opcode_RET(State8080* state);
uint8_t Opcode_RET_CONDITIONAL(State8080* state);
uint8_t Opcode_RST(State8080* state);
uint8_t Opcode_PCHL(State8080* state);

// Machine control / stack / I/O
uint8_t Opcode_PUSH_RP(State8080* state);
uint8_t Opcode_PUSH_PSW(State8080* state);
uint8_t Opcode_POP_RP(State8080* state);
uint8_t Opcode_POP_PSW(State8080* state);
uint8_t Opcode_XTHL(State8080* state);
uint8_t Opcode_SPHL(State8080* state);
uint8_t Opcode_IN_PORT(State8080* state);
uint8_t Opcode_OUT_PORT(State8080* state);
uint8_t Opcode_EI(State8080* state);
uint8_t Opcode_DI(State8080* state);
uint8_t Opcode_HLT(State8080* state);