#include "State8080.h"

/*
  1101  0100
  SZ0A  0P0C
*/

/*
    ADD r
    ADD M
*/
uint8_t Opcode_ADD_R_M(State8080* state) {
  uint8_t reg = (state->memory[state->pc - 1]) & 0x07u;
  uint8_t* reg_value = GetRegisterPointer(state, reg);

  uint8_t val = *reg_value;

  if (((state->reg_a & 0x0F) + (val & 0x0F)) > 0x0F) {
    state->reg_flag |= 0x10;
  } else {
    state->reg_flag &= ~0x10;
  }

  uint16_t sum = (uint16_t)state->reg_a + (uint16_t)val;

  if (sum > 0xFF)
    state->reg_flag |= 0x01;
  else
    state->reg_flag &= ~0x01;

  state->reg_a = (uint8_t)sum;
  UpdateZeroAndSignFlags(state, state->reg_a);

  if (reg == 6) {
    return 7;
  }
  return 4;
}

/*
  ADC r
  ADC M
*/

uint8_t Opcode_ADC_R_M(State8080* state) {
  uint8_t reg = (state->memory[state->pc - 1]) & 0x07u;
  uint8_t* reg_value = GetRegisterPointer(state, reg);

  uint8_t carry = state->reg_flag & 0x01;
  uint8_t val = *reg_value;

  if (((state->reg_a & 0x0F) + (val & 0x0F)) + carry > 0x0F) {
    state->reg_flag |= 0x10;
  } else {
    state->reg_flag &= ~0x10;
  }

  uint16_t sum = (uint16_t)state->reg_a + (uint16_t)val + carry;

  if (sum > 0xFF)
    state->reg_flag |= 0x01;
  else
    state->reg_flag &= ~0x01;

  state->reg_a = (uint8_t)sum;
  UpdateZeroAndSignFlags(state, state->reg_a);

  if (reg == 6) {
    return 7;
  }
  return 4;
}

/*
    SUB r
    SUB M
*/

uint8_t Opcode_Sub_R_M(State8080* state) {
  uint8_t reg = (state->memory[state->pc - 1]) & 0x07u;
  uint8_t* reg_value = GetRegisterPointer(state, reg);

  uint8_t val = *reg_value;

  if (state->reg_a < val) {
    state->reg_flag |= 0x01;  // Carry bit
  } else {
    state->reg_flag &= ~0x01;
  }

  if ((state->reg_a & 0x0F) < (val & 0x0F)) {
    state->reg_flag |= 0x10;  // Auxiliary Carry
  } else {
    state->reg_flag &= ~0x10;
  }

  state->reg_a -= val;
  UpdateZeroAndSignFlags(state, state->reg_a);

  if (reg == 6) {
    return 7;
  }
  return 4;
}

uint8_t Opcode_SBI_R_M(State8080* state) {
  uint8_t reg = (state->memory[state->pc - 1]) & 0x07u;
  uint8_t* reg_value = GetRegisterPointer(state, reg);
  uint8_t carry = state->reg_flag & 0x01;

  uint8_t val = *reg_value;

  uint8_t sub = state->reg_a - val - carry;

  if (state->reg_a < val) {
    state->reg_flag |= 0x01;  // Carry bit
  } else {
    state->reg_flag &= ~0x01;
  }

  if ((state->reg_a & 0x0F) < (val & 0x0F)) {
    state->reg_flag |= 0x10;  // Auxiliary Carry
  } else {
    state->reg_flag &= ~0x10;
  }

  state->reg_a = sub;
  UpdateZeroAndSignFlags(state, state->reg_a);

  if (reg == 6) {
    return 7;
  }
  return 4;
}