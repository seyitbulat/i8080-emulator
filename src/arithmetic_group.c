#include "State8080.h"

/*
1101  0101
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

uint8_t Opcode_SBB_R_M(State8080* state) {
  uint8_t reg = (state->memory[state->pc - 1]) & 0x07u;
  uint8_t* reg_value = GetRegisterPointer(state, reg);
  uint8_t carry = state->reg_flag & 0x01;

  uint8_t val = *reg_value;

  uint8_t sub = state->reg_a - val - carry;

  if (state->reg_a < val + carry) {
    state->reg_flag |= 0x01;  // Carry bit
  } else {
    state->reg_flag &= ~0x01;
  }

  if ((state->reg_a & 0x0F) < ((val & 0x0F) + carry)) {
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

uint8_t Opcode_ADI_DATA(State8080* state) {
  uint8_t data = state->memory[state->pc];

  uint8_t reg_a_value = state->reg_a;

  uint16_t sum = reg_a_value + data;

  if (sum > 0xFF) {
    state->reg_flag |= 0x01;
  } else {
    state->reg_flag &= ~0x01;
  }

  // for nibble

  if ((reg_a_value & 0x0F) + (data & 0x0F) > 0x0F) {
    state->reg_flag |= 0x10;
  } else {
    state->reg_flag &= ~0x10;
  }

  state->reg_a = sum;
  UpdateZeroAndSignFlags(state, state->reg_a);

  state->pc += 1;
  return 7;
}

uint8_t Opcode_ACI_DATA(State8080* state) {
  uint8_t data = state->memory[state->pc];

  uint8_t reg_a_value = state->reg_a;

  uint16_t sum = reg_a_value + data;

  uint8_t carry = state->reg_flag & 0x01;

  if (sum + carry > 0xFF) {
    state->reg_flag |= 0x01;
  } else {
    state->reg_flag &= ~0x01;
  }

  // for nibble

  if (((reg_a_value & 0x0F) + (data & 0x0F) + carry) > 0x0F) {
    state->reg_flag |= 0x10;
  } else {
    state->reg_flag &= ~0x10;
  }

  state->reg_a = sum + carry;
  UpdateZeroAndSignFlags(state, state->reg_a);

  state->pc += 1;
  return 7;
}

uint8_t Opcode_SUI_DATA(State8080* state) {
  uint8_t data = state->memory[state->pc];
  uint8_t reg_a_val = state->reg_a;

  uint8_t sub = reg_a_val - data;

  if (data > reg_a_val) {
    state->reg_flag |= 0x01;
  } else {
    state->reg_flag &= ~0x01;
  }

  if ((data & 0x0F) > (reg_a_val & 0x0F)) {
    state->reg_flag |= 0x10;
  } else {
    state->reg_flag &= ~0x10;
  }

  state->reg_a = sub;

  UpdateZeroAndSignFlags(state, state->reg_a);

  state->pc += 1;

  return 7;
}

uint8_t Opcode_SBI_DATA(State8080* state) {
  uint8_t data = state->memory[state->pc];

  uint8_t reg_a_val = state->reg_a;
  uint8_t carry = state->reg_flag & 0x01;

  if ((data + carry) > reg_a_val) {
    state->reg_flag |= 0x01;
  } else {
    state->reg_flag &= ~0x01;
  }

  if (((data & 0x0F) + carry) > (reg_a_val & 0x0F)) {
    state->reg_flag |= 0x10;
  } else {
    state->reg_flag &= ~0x10;
  }
  state->reg_a = reg_a_val - data - carry;
  UpdateZeroAndSignFlags(state, state->reg_a);

  state->pc += 1;

  return 7;
}

uint8_t Opcode_INR_R_M(State8080* state) {
  uint8_t reg_address = (state->memory[state->pc - 1] >> 3u) & 0x07;

  uint8_t* reg_val = GetRegisterPointer(state, reg_address);

  if ((*reg_val & 0x0F) + 1 > 0x0F) {
    state->reg_flag |= 0x10;
  } else {
    state->reg_flag &= ~0x10;
  }

  *reg_val += 1;

  UpdateZeroAndSignFlags(state, *reg_val);

  if (reg_address == 6) {
    return 10;
  }
  return 5;
}

uint8_t Opcode_DCR_R_M(State8080* state) {
  uint8_t reg_address = (state->memory[state->pc - 1] >> 3u) & 0x07;

  uint8_t* reg_val = GetRegisterPointer(state, reg_address);

  if ((*reg_val & 0x0F) == 0x00) {
    state->reg_flag |= 0x10;
  } else {
    state->reg_flag &= ~0x10;
  }

  *reg_val -= 1;
  UpdateZeroAndSignFlags(state, *reg_val);

  if (reg_address == 6) {
    return 10;
  } else {
    return 5;
  }
}

uint8_t Opcode_INX_RP(State8080* state) {
  uint8_t rp_address = (state->memory[state->pc - 1] >> 4u) & 0x03;
  uint16_t* rp_value = GetRegisterPair(state, rp_address);

  *rp_value += 1;

  return 5;
}

uint8_t Opcode_DCX_RP(State8080* state){
  uint8_t rp_address = (state->memory[state->pc - 1] >> 4u) & 0x03;
  uint16_t* rp_value = GetRegisterPair(state, rp_address);

  *rp_value -= 1;

  return 5;
}


uint8_t Opcode_DAA(State8080* state){
  uint8_t low_nibble = state->reg_a & 0x0F;
  uint8_t high_nibble = (state->reg_a >> 4u) & 0x0F;

  uint8_t ac_flag = state->reg_flag & 0x10; 
  uint8_t cy_flag = state->reg_flag & 0x01;

  if(low_nibble > 9 || ac_flag == 0x10){
    state->reg_a += 6;
  }

  if(high_nibble > 9 || cy_flag == 0b1){
    state->reg_a = ((high_nibble + 4) << 4u) | low_nibble;
  }

  return 4;

}