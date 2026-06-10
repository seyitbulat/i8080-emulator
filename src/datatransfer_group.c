#include "State8080.h"




/*
    MVU , data (Move Immediate) Instuctions
*/

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



/*
    MVI M, data (Move to memory immediate)
*/

// 0x36
uint8_t Opcode_MVI_M(State8080 *state){
    uint8_t data = (uint8_t) state->memory[state->pc];

    uint16_t address = state->reg_hl.lh;

    state->memory[address] = data;

    state->pc += 1;
    return 10;
}

/*
    MOV r1, r2 (Move register)
*/
uint8_t Opcode_MOV_R1_R2(State8080 *state){
    
    uint8_t reg_1 = state->memory[state->pc] & 0x07;
    uint8_t reg_2 = state->memory[state->pc] & 0x38;

    // not implemented
}