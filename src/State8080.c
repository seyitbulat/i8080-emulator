#include "State8080.h"

void Init8080(State8080* state) {
  for (uint16_t i = 0; i < 256; i++) {
    state->instruction_set[i] = Opcode_NOP;
  }
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
    case 7:  // A Register
      return &state->reg_a;
  }
}

uint16_t EmulateCycle(State8080* state) {
  uint8_t opcode = state->memory[state->pc];
  state->pc += 1;
  uint8_t time = state->instruction_set[opcode](state);
  return time;
}

uint8_t Opcode_NOP(State8080* state) { return 4; }
