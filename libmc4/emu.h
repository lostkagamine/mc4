#ifndef __EMU_H__
#define __EMU_H__

#include<stdint.h>

#define MAX_MEM (1024*64)
#define WORD(h, l) (((h) << 8) + (l))

#include "bus.h"

typedef struct CPU {
    uint16_t PC;
    uint16_t SP;

    uint8_t A;
    uint8_t B;

    uint8_t FLAGS;
} CPU;

typedef struct MC4_Context {
    CPU* cpu;
    Bus* bus;
} MC4_Context;

enum Flags {
    Z = (0 << 0),   // Zero       set if A == 0
    O = (0 << 1),   // Overflow   set if ???
    S = (0 << 2)    // Sign       set if A & 0x80 > 0
};

extern void MC4_setCPU(MC4_Context* ctx, CPU* cpu);
extern void MC4_setBus(MC4_Context* ctx, Bus* bus);
extern void MC4_fetch(MC4_Context* ctx, uint8_t* data);
extern void MC4_reset(MC4_Context* ctx);
extern int MC4_runCycle(MC4_Context* ctx);
extern void MC4_execute(MC4_Context* ctx);

#endif