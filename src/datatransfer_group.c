#include "State8080.h"

/*
    MVI , data (Move Immediate) Instuctions
*/

// 00 DD |D 110
uint8_t Opcode_MVI_R(State8080* state) {
  uint8_t reg = (state->memory[state->pc - 1] >> 3u) & 0x07;

  uint8_t* p_reg = GetRegisterPointer(state, reg);

  uint8_t data = state->memory[state->pc];

  *p_reg = data;

  state->pc += 1;

  if (reg == 6) {
    return 10;
  }
  return 7;
}

/*
    MOV r1, r2 (Move register)
*/
uint8_t Opcode_MOV_R1_R2(State8080* state) {
  uint8_t reg_s = state->memory[state->pc - 1] & 0x07;
  uint8_t reg_d = (state->memory[state->pc - 1] >> 3u) & 0x07;

  uint8_t* p_reg_s = GetRegisterPointer(state, reg_s);
  uint8_t* p_reg_d = GetRegisterPointer(state, reg_d);

  *p_reg_d = *p_reg_s;

  if (reg_d == 6 || reg_s == 6) {
    return 7;
  }
  return 5;
}

/*
    LXI rp, data 16 (Load register pair immediate)
*/

/*
RP Register Pair
00 B-C
01 D-E
10 H-L
11 SP

*/
uint8_t Opcode_LXI_RP_DATA(State8080* state) {
  uint8_t rp = (state->memory[state->pc - 1] >> 4u) & 0x03;

  uint16_t* p_rp = GetRegisterPair(state, rp);

  uint8_t low_data = (state->memory[state->pc]);
  uint8_t high_data = (state->memory[state->pc + 1]);

  *p_rp = (high_data << 8u) | low_data;

  state->pc += 2;

  return 10;
}

/*
    STA addr (Store accumulator direct)
*/

uint8_t Opcode_STA_ADDR(State8080* state) {
  uint8_t low_address = (state->memory[state->pc]);
  uint8_t high_address = (state->memory[state->pc + 1]);

  uint16_t address = (high_address << 8u) | low_address;

  state->memory[address] = state->reg_a;

  state->pc += 2;

  return 13;
}

/*
    LDA addr (Load accumulator direct)
*/

uint8_t Opcode_LDA_ADDR(State8080* state) {
  uint8_t low_address = (state->memory[state->pc]);
  uint8_t high_address = (state->memory[state->pc + 1]);

  uint16_t address = (high_address << 8u) | low_address;

  state->reg_a = state->memory[address];

  state->pc += 2;

  return 13;
}

/*
    STAX rp (Store accumulator indirect)
*/

uint8_t Opcode_STAX_RP(State8080* state) {
  uint8_t opcode = state->memory[state->pc - 1];

  uint8_t rp = (opcode >> 4u) & 0x03u;

  uint16_t* p_rp = GetRegisterPair(state, rp);

  state->memory[*p_rp] = state->reg_a;

  return 7;
}

/*
    LDAX rp (Load accumulator indirect)
*/

uint8_t Opcode_LDAX_RP(State8080* state) {
  uint8_t opcode = state->memory[state->pc - 1];

  uint8_t rp = (opcode >> 4u) & 0x03u;

  uint16_t* p_rp = GetRegisterPair(state, rp);

  state->reg_a = state->memory[*p_rp];

  return 7;
}


/*
    SHLD addr (Store H and L direct)
*/

uint8_t Opcode_SHLD_ADDR(State8080* state){
    uint8_t low_address = state->memory[state->pc];
    uint8_t high_address = state->memory[state->pc + 1];

    uint16_t address = (high_address << 8u) | low_address;
    
    state->memory[address] = state->reg_hl.l;
    state->memory[address + 1] = state->reg_hl.h;

    state->pc += 2;

    return 16;
}


/*
  LHLD addr (Load H and L direct)
*/

uint8_t Opcode_LHLD_ADDR(State8080* state){
    uint8_t low_address = state->memory[state->pc];
    uint8_t high_address = state->memory[state->pc + 1];

    uint16_t address = (high_address << 8u) | low_address;

    uint8_t low_data = state->memory[address];
    uint8_t high_data = state->memory[address + 1];

    state->reg_hl.l = low_data;
    state->reg_hl.h = high_data;

    state->pc += 2;

    return 16;
}


/*
    XCHG (Exchange H and L with D and E)
*/

uint8_t Opcode_XCHG(State8080* state){

    uint16_t hl_data = state->reg_hl.hl;
    uint16_t de_data = state->reg_de.hl;

    state->reg_de.hl = hl_data;

    state->reg_hl.hl = de_data;

    return 4;
}