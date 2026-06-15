#include "State8080.h"

uint8_t Op_JMP_ADDR(State8080* state) {
  uint8_t lower_address = state->memory[state->pc];
  uint8_t higher_address = state->memory[state->pc + 1];

  uint16_t address = (uint16_t)lower_address | (uint16_t)(higher_address << 8u);

  state->pc = address;

  return 10;
}

uint8_t Op_JUMP_CONDITIONAL_ADDR(State8080* state) {
  uint8_t condition_addr = (state->memory[state->pc - 1] >> 3u) & 0x07;

  bool condition = GetConditionStatus(state, condition_addr);

  if (condition) {
    uint8_t lower_address = state->memory[state->pc];
    uint8_t higher_address = state->memory[state->pc + 1];

    uint16_t address =
        (uint16_t)lower_address | (uint16_t)(higher_address << 8u);

    state->pc = address;
  }else{
    state->pc += 2;
  }

  return 10;
}


uint8_t Op_CALL_ADDR(State8080* state){
    uint8_t low_address = (state->memory[state->pc]);
    uint8_t high_address = (state->memory[state->pc + 1]);

    // not implemented

}