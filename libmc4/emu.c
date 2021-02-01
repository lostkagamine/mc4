#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#include "emu.h"
#include "instructions.h"

void fetch(CPU* cpu, Memory* mem, uint8_t* data) {
    *data = mem->Data[cpu->PC];
    cpu->PC++;
}

void resetCPU(CPU* cpu, Memory* mem) {
    cpu->PC = 0xFFF0;
    cpu->SP = 0x0;
    cpu->A = 0x0;
    cpu->B = 0x0;
    cpu->FLAGS = 0x0;

    uint8_t newPC_low;
    uint8_t newPC_high;

    fetch(cpu, mem, &newPC_low);
    fetch(cpu, mem, &newPC_high);

    cpu->PC = WORD(newPC_low, newPC_high);
}

void initMemory(Memory* mem) {
    for (unsigned int i = 0; i < MAX_MEM; i++) {
        mem->Data[i] = 0x0;
    }
}

static void setFlag(CPU* cpu, int flag, unsigned int value) {
    if (value == 1)
        cpu->FLAGS |= flag;
    else
        cpu->FLAGS &= ~(flag);
}

static char getFlag(CPU* cpu, int flag) {
    return (cpu->FLAGS & flag) > 0;
}

int doOne(CPU* cpu, Memory* mem) {
    uint8_t ins;
    fetch(cpu, mem, &ins);

    switch (ins) {
    case INS_NOOP:
        // Don't do anything.
        break;
    case INS_LDA_LIT:
    {
        uint8_t dat;

        fetch(cpu, mem, &dat);

        cpu->A = dat;

        break;
    };
    case INS_LDA_PTR:
    {
        uint8_t high;
        uint8_t low;

        fetch(cpu, mem, &high);
        fetch(cpu, mem, &low);

        uint16_t addr = WORD(high, low);
        cpu->A = mem->Data[addr];

        break;
    };
    case INS_LDB_LIT:
    {
        uint8_t dat;

        fetch(cpu, mem, &dat);

        cpu->B = dat;

        break;
    };
    case INS_LDB_PTR:
    {
        uint8_t high;
        uint8_t low;

        fetch(cpu, mem, &high);
        fetch(cpu, mem, &low);

        uint16_t addr = WORD(high, low);
        cpu->B = mem->Data[addr];

        break;
    };
    case INS_JMP_LIT:
    {
        uint8_t high;
        uint8_t low;

        fetch(cpu, mem, &high);
        fetch(cpu, mem, &low);

        uint16_t res = WORD(high, low);
        cpu->PC += res;

        break;
    };
    case INS_JMP_PTR:
    {
        uint8_t high;
        uint8_t low;

        fetch(cpu, mem, &high);
        fetch(cpu, mem, &low);

        uint16_t addr = WORD(high, low);
        cpu->PC = mem->Data[addr];

        break;
    };
    case INS_JMP_REG:
    {
        uint16_t result = WORD(cpu->A, cpu->B);
        cpu->PC = result;
        break;
    };
    case INS_HALT:
    {
        return 0;
    };

    default:
    {
        printf("Unknown instruction %x\n", ins);
        break;
    };
    };

    return 1;
}

void run(CPU* cpu, Memory* mem, uint32_t tics, uint32_t runForever) {
    while (tics > 0 || runForever != 0) {
        if (doOne(cpu, mem) == 0) break;
        tics--;
    }
}