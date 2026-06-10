#include "State8080.h"

void Init8080(State8080* state) {
    for(uint16_t i = 0; i < 256; i++){
        state->instruction_set[i] = Opcode_NOP;
    }

}

uint16_t EmulateCycle(State8080* state) {
  uint8_t opcode = state->memory[state->pc];
  state->pc += 1;
  uint8_t time = state->instruction_set[opcode](state);
  return time;
}

uint8_t Opcode_NOP(State8080* state) { return 4; }