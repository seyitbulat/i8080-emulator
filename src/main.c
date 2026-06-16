#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu/State8080.h"

bool LoadROM(State8080* state, const char* filename, uint16_t offset) {
  FILE* fptr;

  fptr = fopen(filename, "rb");

  if (fptr == NULL) {
    printf("The file is not opened.");
    return false;
  }

  fread(&state->memory[offset], sizeof(uint8_t), MEMORY_SIZE - offset, fptr);
  fclose(fptr);

  return true;
}

int main() {
  State8080* cpu = (State8080*)malloc(sizeof(State8080));

  if(cpu == NULL){
    printf("Bellek ayrilamadi!\n");
    return 1;
  }

  Init8080(cpu);

  bool is_loaded = LoadROM(cpu, "CPUTEST.COM", 0x0100);

  if (!is_loaded) return 1;

  cpu->pc = 0x0100;

  while (1)
  {
    if(cpu->pc == 0x0005){
        if(cpu->reg_bc.l == 9){
            uint16_t str_addr = cpu->reg_de.hl;
            while(cpu->memory[str_addr] != '$'){
                printf("%c", cpu->memory[str_addr]);
                str_addr++;
            }

        }

        if(cpu->reg_bc.l == 2){
            printf("%c", cpu->reg_de.hl);
        }

        uint16_t ret_addr = (cpu->memory[cpu->sp]) | (cpu->memory[cpu->sp + 1] << 8u);

        cpu->pc = ret_addr;
        cpu->sp += 2;

    }

    EmulateCycle(cpu);
  }
  
}

