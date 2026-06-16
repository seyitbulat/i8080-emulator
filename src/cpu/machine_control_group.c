#include "State8080.h"

uint8_t Opcode_PUSH_RP(State8080* state) {
  uint8_t rp_address = (state->memory[state->pc - 1] >> 4u) & 0x03;

  if (rp_address == 3) {
    state->memory[state->sp - 1] = state->reg_a;
    state->memory[state->sp - 2] = state->reg_flag;
  } else {
    uint16_t* rp_val = GetRegisterPair(state, rp_address);
    state->memory[state->sp - 1] = (uint8_t)(*rp_val >> 8u) & 0xFF;
    state->memory[state->sp - 2] = (uint8_t)(*rp_val & 0xFF);
  }

  state->sp -= 2;
  return 11;
}

uint8_t Opcode_PUSH_PSW(State8080* state) {
  state->memory[state->sp - 1] = state->reg_a;

  state->memory[state->sp - 2] = state->reg_flag;
  state->sp -= 2;

  return 11;
}

uint8_t Opcode_POP_RP(State8080* state) {
  uint8_t rp_address = (state->memory[state->pc - 1] >> 4u) & 0x03;

  if (rp_address == 3) {
    state->reg_flag = state->memory[state->sp];
    state->reg_a = state->memory[state->sp + 1];
  } else {
    uint16_t* rp_val = GetRegisterPair(state, rp_address);
    *rp_val = (uint16_t)state->memory[state->sp] |
              (state->memory[state->sp + 1] << 8u);
  }

  state->sp += 2;
  return 10;
}

uint8_t Opcode_POP_PSW(State8080* state) {
  state->reg_flag = state->memory[state->sp];
  state->reg_a = state->memory[state->sp + 1];

  state->sp += 2;

  return 10;
}

uint8_t Opcode_XTHL(State8080* state) {
  uint8_t hl_l = state->reg_hl.l;
  state->reg_hl.l = state->memory[state->sp];
  state->memory[state->sp] = hl_l;

  uint8_t hl_h = state->reg_hl.h;
  state->reg_hl.h = state->memory[state->sp + 1];
  state->memory[state->sp + 1] = hl_h;

  return 18;
}

uint8_t Opcode_SPHL(State8080* state) {
  state->sp = state->reg_hl.hl;

  return 5;
}

uint8_t Opcode_IN_PORT(State8080* state) {
  uint8_t port_addr = state->memory[state->pc];

  InputHandler port = state->in_devices[port_addr];

  if (port) {
    state->reg_a = port();
  } else {
    state->reg_a = 0xFF;
  }

  state->pc += 1;

  return 10;
}

uint8_t Opcode_OUT_PORT(State8080* state) {
  uint8_t port_addr = state->memory[state->pc];

  OutputHandler port = state->out_devices[port_addr];

  if (port) {
    port(state->reg_a);
  }

  state->pc += 1;

  return 10;
}


uint8_t Opcode_EI(State8080* state){
    state->internal_state |= 0x01;
    return 4;
}

uint8_t Opcode_DI(State8080* state){
    state->internal_state &= ~0x01;
    return 4;
}

uint8_t Opcode_HLT(State8080* state){
    state->internal_state |= 0x02;
    return 4;
}