#ifndef __EMU_H__
#define __EMU_H__

#include<stdint.h>

#define MAX_MEM (1024*64)

#define WORD(h, l) (((h) << 8) + (l))

typedef struct CPU {
    uint16_t PC;
    uint16_t SP;

    uint8_t A;
    uint8_t B;

    uint8_t FLAGS;
} CPU;

typedef struct Memory {
    uint8_t Data[MAX_MEM];
} Memory;

enum Flags {
    J = (0 << 0),
    Z = (0 << 1)
};

extern void fetch(CPU* cpu, Memory* mem, uint8_t* data);
extern void resetCPU(CPU* cpu, Memory* mem);
extern int doOne(CPU* cpu, Memory* mem);
extern void run(CPU* cpu, Memory* mem, uint32_t tics, uint32_t runForever);
extern void initMemory(Memory* mem);

#endif