#include "State8080.h"

/*
    ADD r
    ADD M
*/
uint8_t Opcode_ADD_R(State8080* state) {
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
    SUB r
    SUB M
*/

uint8_t Opcode_Sub_R(State8080* state) {
  uint8_t reg = (state->memory[state->pc - 1]) & 0x07u;
  uint8_t* reg_value = GetRegisterPointer(state, reg);

  uint8_t val = *reg_value;

  // not implemented
}

/*
    ADC r
    ADC M
*/

uint8_t Opcode_ADC_R(State8080* state) {
  uint8_t reg = (state->memory[state->pc - 1]) & 0x07u;

  uint8_t* reg_value = GetRegisterPointer(state, reg);

  // Not implemented
}