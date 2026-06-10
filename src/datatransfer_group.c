#include "State8080.h"

// 00 DD |D 110

// A 111 -> 0x3E
uint8_t Opcode_MVI_A(State8080 *state){
    uint16_t parameter = state->memory[state->pc];

    uint8_t data = (uint8_t) parameter;

    state->reg_a = data;

    state->pc += 1;
    return 7;
}
// B 000 -> 0x06
uint8_t Opcode_MVI_B(State8080 *state){
    uint16_t parameter = state->memory[state->pc];

    uint8_t data = (uint8_t) parameter;

    state->reg_bc.h = data;

    state->pc += 1;
    return 7;
}

// C 001 -> 0x0E
uint8_t Opcode_MVI_C(State8080 *state){
    uint16_t parameter = state->memory[state->pc];

    uint8_t data = (uint8_t) parameter;

    state->reg_bc.l = data;

    state->pc += 1;
    return 7;
}

// D 010 -> 0x1E
uint8_t Opcode_MVI_D(State8080 *state){
    uint16_t parameter = state->memory[state->pc];

    uint8_t data = (uint8_t) parameter;

    state->reg_de.h = data;

    state->pc += 1;
    return 7;
}

// E 011 -> 0x1E
uint8_t Opcode_MVI_E(State8080 *state){
    uint16_t parameter = state->memory[state->pc];

    uint8_t data = (uint8_t) parameter;

    state->reg_de.l = data;

    state->pc += 1;
    return 7;
}


// H 100 -> 0x26
uint8_t Opcode_MVI_H(State8080 *state){
    uint16_t parameter = state->memory[state->pc];

    uint8_t data = (uint8_t) parameter;

    state->reg_hl.l = data;

    state->pc += 1;
    return 7;
}

// L 101 -> 0x2E
uint8_t Opcode_MVI_L(State8080 *state){
    uint16_t parameter = state->memory[state->pc];

    uint8_t data = (uint8_t) parameter;

    state->reg_hl.h = data;

    state->pc += 1;
    return 7;
}