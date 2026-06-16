#include "State8080.h"

uint8_t Opcode_JMP_ADDR(State8080* state) {
  uint8_t lower_address = state->memory[state->pc];
  uint8_t higher_address = state->memory[state->pc + 1];

  uint16_t address = (uint16_t)lower_address | (uint16_t)(higher_address << 8u);

  state->pc = address;

  return 10;
}

uint8_t Opcode_JUMP_CONDITIONAL_ADDR(State8080* state) {
  uint8_t condition_addr = (state->memory[state->pc - 1] >> 3u) & 0x07;

  bool condition = GetConditionStatus(state, condition_addr);

  if (condition) {
    uint8_t lower_address = state->memory[state->pc];
    uint8_t higher_address = state->memory[state->pc + 1];

    uint16_t address =
        (uint16_t)lower_address | (uint16_t)(higher_address << 8u);

    state->pc = address;
  } else {
    state->pc += 2;
  }

  return 10;
}

uint8_t Opcode_CALL_ADDR(State8080* state) {
  uint8_t low_address = (state->memory[state->pc]);
  uint8_t high_address = (state->memory[state->pc + 1]);

  uint16_t return_address = state->pc + 2;

  state->memory[state->sp - 1] = (return_address >> 8u) & 0xFF;
  state->memory[state->sp - 2] = return_address & 0xFF;

  state->sp -= 2;

  state->pc = (uint16_t)(low_address) | (uint16_t)(high_address << 8u);

  return 17;
}

uint8_t Opcode_CALL_CONDITIONAL_ADDR(State8080* state) {
  uint8_t condition_address = (state->memory[state->pc - 1] >> 3u) & 0x07;

  bool condition = GetConditionStatus(state, condition_address);

  if (condition) {
    uint8_t low_address = (state->memory[state->pc]);
    uint8_t high_address = (state->memory[state->pc + 1]);

    uint16_t return_address = state->pc + 2;

    state->memory[state->sp - 1] = (return_address >> 8u) & 0xFF;
    state->memory[state->sp - 2] = return_address & 0xFF;

    state->sp -= 2;

    state->pc = (uint16_t)(low_address) | (uint16_t)(high_address << 8u);

    return 17;
  }

  state->pc += 2;

  return 11;
}

uint8_t Opcode_RET(State8080* state) {
  uint8_t sp_low = state->memory[state->sp];
  uint8_t sp_high = state->memory[state->sp + 1];

  state->pc = (uint16_t)sp_low | ((uint16_t)sp_high << 8u);

  state->sp += 2;
  return 10;
}

uint8_t Opcode_RET_CONDITIONAL(State8080* state) {
  uint8_t condition_addr = (state->memory[state->pc - 1] >> 3u) & 0x07;

  uint8_t condition = GetConditionStatus(state, condition_addr);

  if (condition) {
    uint8_t sp_low = state->memory[state->sp];
    uint8_t sp_high = state->memory[state->sp + 1];

    state->pc = (uint16_t)sp_low | ((uint16_t)sp_high << 8u);

    state->sp += 2;
    return 11;
  }

  return 5;
}

uint8_t Opcode_RST(State8080* state) {
  uint8_t pc_high = (state->pc >> 8u) & 0xFF;
  uint8_t pc_low = (state->pc) & 0xFF;
  uint8_t nnn = (state->memory[state->pc - 1] >> 3u) & 0x07;

  state->memory[state->sp - 1] = pc_high;
  state->memory[state->sp - 2] = pc_low;

  uint16_t reset_addr = nnn;
  state->sp -= 2;
  state->pc = reset_addr << 3u;

  return 11;
}


uint8_t Opcode_PCHL(State8080* state){

  state->pc = state->reg_hl.hl;

  return 5;
}