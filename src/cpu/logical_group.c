#include "State8080.h"

uint8_t Opcode_ANA_R_M(State8080* state) {
  uint8_t reg = state->memory[state->pc - 1] & 0x07;

  uint8_t* reg_val = GetRegisterPointer(state, reg);

  state->reg_a &= *reg_val;

  state->reg_flag &= ~0x01;
  state->reg_flag |= 0x10;

  UpdateZeroAndSignFlags(state, state->reg_a);
  if (reg == 6) {
    return 7;
  }
  return 4;
}

uint8_t Opcode_ANI_DATA(State8080* state) {
  uint8_t data = state->memory[state->pc];

  state->reg_a &= data;

  state->reg_flag &= ~0x01;
  state->reg_flag |= 0x10;

  UpdateZeroAndSignFlags(state, state->reg_a);

  state->pc += 1;
  return 7;
}

uint8_t Opcode_XRA_R_M(State8080* state) {
  uint8_t reg = state->memory[state->pc - 1] & 0x07;

  uint8_t* reg_val = GetRegisterPointer(state, reg);

  state->reg_a ^= *reg_val;
  state->reg_flag &= ~0x01;
  state->reg_flag &= ~0x10;

  UpdateZeroAndSignFlags(state, state->reg_a);
  if (reg == 6) {
    return 7;
  }
  return 4;
}

uint8_t Opcode_XRI_DATA(State8080* state) {
  uint8_t data = state->memory[state->pc];
  state->reg_a ^= data;

  state->reg_flag &= ~0x01;
  state->reg_flag &= ~0x10;

  UpdateZeroAndSignFlags(state, state->reg_a);

  state->pc += 1;

  return 7;
}

uint8_t Opcode_ORA_R_M(State8080* state) {
  uint8_t reg = state->memory[state->pc - 1] & 0x07;

  uint8_t* reg_val = GetRegisterPointer(state, reg);

  state->reg_a |= *reg_val;
  state->reg_flag &= ~0x01;
  state->reg_flag &= ~0x10;

  UpdateZeroAndSignFlags(state, state->reg_a);
  if (reg == 6) {
    return 7;
  }
  return 4;
}

uint8_t Opcode_ORI_DATA(State8080* state) {
  uint8_t data = state->memory[state->pc];
  state->reg_a |= data;

  state->reg_flag &= ~0x01;
  state->reg_flag &= ~0x10;

  UpdateZeroAndSignFlags(state, state->reg_a);

  state->pc += 1;

  return 7;
}

uint8_t Opcode_CPI_DATA(State8080* state) {
  uint8_t data = state->memory[state->pc];
  uint8_t sub = state->reg_a - data;

  if (state->reg_a < data) {
    state->reg_flag |= 0x01;
  } else {
    state->reg_flag &= ~0x01;
  }

  if ((data & 0x0F) > (state->reg_a & 0x0F)) {
    state->reg_flag |= 0x10;
  } else {
    state->reg_flag &= ~0x10;
  }

  UpdateZeroAndSignFlags(state, sub);

  state->pc += 1;
  return 7;
}

uint8_t Opcode_CMP_R_M(State8080* state){
    uint8_t reg = state->memory[state->pc-1] & 0x07;
    uint8_t *reg_val = GetRegisterPointer(state, reg);

    uint8_t sub = state->reg_a - *reg_val;
    if(state->reg_a < *reg_val){
        state->reg_flag |= 0x01;
    }else{
        state->reg_flag &= ~0x01;
    }

    if((*reg_val & 0x0F) > (state->reg_a & 0x0F)){
        state->reg_flag |= 0x10;
    }else{
        state->reg_flag &= ~0x10;
    }

    UpdateZeroAndSignFlags(state, sub);

    if(reg == 6){
        return 7;
    }
    return 4;
}

uint8_t Opcode_CMA(State8080* state){
    state->reg_a = ~state->reg_a;

    return 4;
}

uint8_t Opcode_CMC(State8080* state){
    state->reg_flag ^= 0x01;

    return 4;
}


uint8_t Opcode_STC(State8080* state){
    state->reg_flag |= 0x01;

    return 4;
}

uint8_t Opcode_RLC(State8080* state){
    uint8_t A7 = state->reg_a & 0x80;

    state->reg_a <<= 1;

    state->reg_a |= (A7 >> 7u);

    state->reg_flag &= ~0x01;
    state->reg_flag |= (A7 >> 7u);

    return 4;
}

uint8_t Opcode_RRC(State8080* state){
    uint8_t A0 = state->reg_a & 0x01;

    state->reg_a >>= 1;
    state->reg_a |= (A0 << 7u);

    state->reg_flag &= ~0x01;
    state->reg_flag |= A0;

    return 4;
}


uint8_t Opcode_RAL(State8080* state){
    uint8_t A7 = state->reg_a & 0x80;

    state->reg_a <<= 1;

    state->reg_a |= (0x01 & (state->reg_flag & 0x01));

    state->reg_flag &= ~0x01;
    state->reg_flag |= (A7 >> 7u);

    return 4;
}

uint8_t Opcode_RAR(State8080* state){
    uint8_t A0 = state->reg_a & 0x01;

    state->reg_a >>= 1;

    state->reg_a |= ((state->reg_flag & 0x01)) << 7u;

    state->reg_flag &= ~0x01;
    state->reg_flag |= A0;

    return 4;
}