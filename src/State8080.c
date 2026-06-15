#include "State8080.h"

void Init8080(State8080* state) {
  for (uint16_t i = 0; i < 256; i++) {
    state->instruction_set[i] = Opcode_NotImplemented;
  }

  // Data transfer group
  state->instruction_set[0x01] = Opcode_LXI_RP_DATA;
  state->instruction_set[0x11] = Opcode_LXI_RP_DATA;
  state->instruction_set[0x21] = Opcode_LXI_RP_DATA;
  state->instruction_set[0x31] = Opcode_LXI_RP_DATA;

  state->instruction_set[0x02] = Opcode_STAX_RP;
  state->instruction_set[0x12] = Opcode_STAX_RP;
  state->instruction_set[0x0A] = Opcode_LDAX_RP;
  state->instruction_set[0x1A] = Opcode_LDAX_RP;

  state->instruction_set[0x22] = Opcode_SHLD_ADDR;
  state->instruction_set[0x2A] = Opcode_LHDL_ADDR;
  state->instruction_set[0x32] = Opcode_STA_ADDR;
  state->instruction_set[0x3A] = Opcode_LDA_ADDR;
  state->instruction_set[0xEB] = Opcode_XCHG;

  for (uint8_t reg = 0; reg < 8; reg++) {
    state->instruction_set[0x06 + (reg << 3u)] = Opcode_MVI_R;
  }

  for (uint8_t opcode = 0x40; opcode <= 0x7F; opcode++) {
    if (opcode != 0x76) {
      state->instruction_set[opcode] = Opcode_MOV_R1_R2;
    }
  }

  // Arithmetic group
  for (uint8_t reg = 0; reg < 8; reg++) {
    state->instruction_set[0x80 + reg] = Opcode_ADD_R_M;
    state->instruction_set[0x88 + reg] = Opcode_ADC_R_M;
    state->instruction_set[0x90 + reg] = Opcode_Sub_R_M;
    state->instruction_set[0x98 + reg] = Opcode_SBI_R_M;

    state->instruction_set[0xA0 + reg] = Opcode_ANA_R_M;
    state->instruction_set[0xA8 + reg] = Opcode_XRA_R_M;
    state->instruction_set[0xB0 + reg] = Opcode_ORA_R_M;
    state->instruction_set[0xB8 + reg] = Opcode_CMP_R_M;
  }

  state->instruction_set[0xE6] = Opcode_ANI_DATA;
  state->instruction_set[0xEE] = Opcode_XRI_DATA;
  state->instruction_set[0xF6] = Opcode_ORI_DATA;
  state->instruction_set[0xFE] = Opcode_CPI_DATA;

  state->instruction_set[0x07] = Opcode_RLC;
  state->instruction_set[0x0F] = Opcode_RRC;
  state->instruction_set[0x17] = Opcode_RAL;
  state->instruction_set[0x1F] = Opcode_RAR;
  state->instruction_set[0x2F] = Opcode_CMA;
  state->instruction_set[0x37] = Opcode_STC;
  state->instruction_set[0x3F] = Opcode_CMC;

  state->instruction_set[0x00] = Opcode_NOP;
}

uint8_t* GetRegisterPointer(State8080* state, uint8_t reg_code) {
  switch (reg_code) {
    case 0:  // B Register
      return &state->reg_bc.h;
    case 1:  // C Register
      return &state->reg_bc.l;
    case 2:  // D Register
      return &state->reg_de.h;
      break;
    case 3:  // E Register
      return &state->reg_de.l;
    case 4:  // H Register
      return &state->reg_hl.h;
    case 5:  // L Register
      return &state->reg_hl.l;
    case 6:  // HL register memory
      return &state->memory[state->reg_hl.hl];
    case 7:  // A Register
      return &state->reg_a;
  }
}

uint16_t* GetRegisterPair(State8080* state, uint8_t rp) {
  switch (rp) {
    case 0:
      return &state->reg_bc.hl;
    case 1:
      return &state->reg_de.hl;
    case 2:
      return &state->reg_hl.hl;
    case 3:
      return &state->sp;
  }
}

void UpdateZeroAndSignFlags(State8080* state, uint8_t result) {
  if (result == 0) {
    state->reg_flag |= 0x40;  // set zero bit to 1
  } else {
    state->reg_flag &= 0xBF;  // set zero bit to 0
  }

  if (result & 0x80) {
    state->reg_flag |= 0x80;  // set sign bit to 1

  } else {
    state->reg_flag &= 0x7F;  // set sign bit to 0
  }

  uint8_t p = result ^ (result >> 4);
  p = p ^ (p >> 2);
  p = p ^ (p >> 1);

  if ((p & 1) == 0) {
    state->reg_flag |= 0x04;
  } else {
    state->reg_flag &= ~0x04;
  }
}

bool GetConditionStatus(State8080* state, uint8_t val) {
  switch (val) {
    case 0:
      return (state->reg_flag & 0x40) == 0;
    case 1:
      return (state->reg_flag & 0x40) != 0;
    case 2:
      return (state->reg_flag & 0x01) == 0;
    case 3:
      return (state->reg_flag & 0x01) != 0;
    case 4:
      return (state->reg_flag & 0x04) == 0;
    case 5:
      return (state->reg_flag & 0x04) != 0;
    case 6:
      return (state->reg_flag & 0x80) == 0;
    case 7:
      return (state->reg_flag & 0x80) != 0;
  }
}

uint16_t EmulateCycle(State8080* state) {
  uint8_t opcode = state->memory[state->pc];
  state->pc += 1;
  uint8_t time = state->instruction_set[opcode](state);
  return time;
}

uint8_t Opcode_NotImplemented(State8080* state) { return Opcode_NOP(state); }

uint8_t Opcode_NOP(State8080* state) { return 4; }
