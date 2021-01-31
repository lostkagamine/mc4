#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#define MAX_MEM (1024*64)

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
    J = (0 << 0)
};

void resetCPU(CPU* cpu, Memory* mem) {
    cpu->PC = 0xFFF0;
    cpu->SP = 0x0;
    cpu->A = 0x0;
    cpu->B = 0x0;
    cpu->FLAGS = 0x0;
}

void initMemory(Memory* mem) {
    for (unsigned int i = 0; i < MAX_MEM; i++) {
        mem->Data[i] = 0x0;
    }
}

void fetch(CPU* cpu, Memory* mem, uint8_t* data) {
    *data = mem->Data[cpu->PC];
    cpu->PC++;
}

#define INS_NOOP    (0x00)
#define INS_HALT    (0xF0)
#define INS_LDA_LIT (0x01)
#define INS_LDA_PTR (0x02)
#define INS_LDB_LIT (0x11)
#define INS_LDB_PTR (0x12)
#define INS_CMP_LIT (0xA0)
#define INS_CMP_PTR (0xA1)
#define INS_JMP_LIT (0xB0)
#define INS_JMP_PTR (0xB1)
#define INS_JMP_REG (0xB2)

#define WORD(h, l) (((h) << 8) + (l))

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

int main(const int argc, char* argv[]) {
    char* filename;

    if (argc < 2) {
        filename = "./data.bin";
    }
    else {
        filename = argv[1];
    }

    FILE* binary = fopen(filename, "rb");

    if (binary == NULL) {
        fprintf(stderr, "No file!?\n");
        return 1;
    }

    CPU cpu;
    Memory* memory = malloc(sizeof(Memory));

    initMemory(memory);
    resetCPU(&cpu, memory);

    int read = fread(memory->Data, 1, MAX_MEM, binary);
    if (read < MAX_MEM) {
        fprintf(stderr, "File too small or error!\n");
        free(memory);
        fclose(binary);

        return 1;
    }

    run(&cpu, memory, 0, 1);

    free(memory);
    fclose(binary);

    return 0;
}