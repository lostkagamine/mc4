#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#include "emu.h"
#include "instructions.h"
#include "bus.h"

void MC4_fetch(MC4_Context* ctx, uint8_t* data) {
    MC4_readBus(ctx->bus, ctx->cpu->PC, data);
    ctx->cpu->PC++;
}

void MC4_reset(MC4_Context* ctx) {
    ctx->cpu->PC = 0xFFF0;
    ctx->cpu->SP = 0x0;
    ctx->cpu->A = 0x0;
    ctx->cpu->B = 0x0;
    ctx->cpu->FLAGS = 0x0;

    uint8_t newPC_low;
    uint8_t newPC_high;

    MC4_fetch(ctx, &newPC_low);
    MC4_fetch(ctx, &newPC_high);

    ctx->cpu->PC = WORD(newPC_low, newPC_high);
}

static void setFlag(CPU* cpu, int flag, unsigned int value) {
    if (value == 1)
        cpu->FLAGS |= flag;
    else
        cpu->FLAGS &= ~flag;
}

static char getFlag(CPU* cpu, int flag) {
    return (cpu->FLAGS & flag) > 0;
}

static uint16_t pointer(MC4_Context* ctx) {
    uint8_t high;
    uint8_t low;

    MC4_fetch(ctx, &high);
    MC4_fetch(ctx, &low);

    return WORD(high, low);
}

int MC4_runCycle(MC4_Context* ctx) {
    uint8_t ins;
    MC4_fetch(ctx, &ins);

    switch (ins) {
    case INS_NOOP:
        // Don't do anything.
        break;
    case INS_LDA_LIT:
    {
        uint8_t dat;

        MC4_fetch(ctx, &dat);

        ctx->cpu->A = dat;

        setFlag(ctx->cpu, Z, ctx->cpu->A == 0);

        break;
    };
    case INS_LDA_PTR:
    {
        uint16_t addr = pointer(ctx);

        uint8_t val;
        MC4_readBus(ctx->bus, addr, &val);
        ctx->cpu->A = val;

        setFlag(ctx->cpu, Z, ctx->cpu->A == 0);

        break;
    };
    case INS_LDB_LIT:
    {
        uint8_t dat;

        MC4_fetch(ctx, &dat);

        ctx->cpu->B = dat;

        break;
    };
    case INS_LDB_PTR:
    {
        uint16_t addr = pointer(ctx);

        uint8_t val;
        MC4_readBus(ctx->bus, addr, &val);
        ctx->cpu->B = val;

        break;
    };
    case INS_JMP_LIT:
    {
        uint16_t res = pointer(ctx);

        ctx->cpu->PC += res;

        break;
    };
    case INS_JMP_PTR:
    {
        uint16_t addr = pointer(ctx);

        uint8_t valLow;
        uint8_t valHigh;

        MC4_readBus(ctx->bus, addr, &valHigh);
        MC4_readBus(ctx->bus, addr+1, &valLow);

        ctx->cpu->PC = WORD(valHigh, valLow);

        break;
    };
    case INS_JMP_REG:
    {
        uint16_t result = WORD(ctx->cpu->A, ctx->cpu->B);
        ctx->cpu->PC = result;
        break;
    };
    case INS_JZS_LIT:
    {
        uint16_t res = pointer(ctx);

        if (getFlag(ctx->cpu, Z)) ctx->cpu->PC += res;

        break;
    };
    case INS_JZS_PTR:
    {
        uint16_t addr = pointer(ctx);

        uint8_t valLow;
        uint8_t valHigh;

        MC4_readBus(ctx->bus, addr, &valHigh);
        MC4_readBus(ctx->bus, addr + 1, &valLow);

        if (getFlag(ctx->cpu, Z)) ctx->cpu->PC = WORD(valHigh, valLow);

        break;
    };
    case INS_JZS_REG:
    {
        uint16_t result = WORD(ctx->cpu->A, ctx->cpu->B);
        if (getFlag(ctx->cpu, Z)) ctx->cpu->PC = result;
        break;
    };
    case INS_JNZ_LIT:
    {
        uint16_t res = pointer(ctx);

        if (!getFlag(ctx->cpu, Z)) ctx->cpu->PC += res;

        break;
    };
    case INS_JNZ_PTR:
    {
        uint16_t addr = pointer(ctx);

        uint8_t valLow;
        uint8_t valHigh;

        MC4_readBus(ctx->bus, addr, &valHigh);
        MC4_readBus(ctx->bus, addr + 1, &valLow);

        if (!getFlag(ctx->cpu, Z)) ctx->cpu->PC = WORD(valHigh, valLow);

        break;
    };
    case INS_JNZ_REG:
    {
        uint16_t result = WORD(ctx->cpu->A, ctx->cpu->B);
        if (!getFlag(ctx->cpu, Z)) ctx->cpu->PC = result;
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

void MC4_execute(MC4_Context* ctx) {
    while (1) {
        if (MC4_runCycle(ctx) == 0) break;
    }
}

void MC4_setCPU(MC4_Context* ctx, CPU* cpu) {
    ctx->cpu = cpu;
}

void MC4_setBus(MC4_Context* ctx, Bus* bus) {
    ctx->bus = bus;
}