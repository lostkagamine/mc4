#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include "libmc4/emu.h"

uint8_t memory[MAX_MEM] = { 0 };

void memReadIntercept(uint16_t address, uint8_t* data) {
    *data = memory[address];
}

void memWriteIntercept(uint16_t address, uint8_t data) {
    memory[address] = data;
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
    Bus bus;
    MC4_Context ctx;

    MC4_initBus(&bus);

    MC4_setBusReadIntercept(&bus, memReadIntercept);
    MC4_setBusWriteIntercept(&bus, memWriteIntercept);

    MC4_setBus(&ctx, &bus);
    MC4_setCPU(&ctx, &cpu);

    size_t read = fread(&memory, 1, MAX_MEM, binary);
    if (read < MAX_MEM) {
        fprintf(stderr, "File too small or error!\n");
        fclose(binary);

        return 1;
    }

    MC4_reset(&ctx);
    // here we go!
    MC4_execute(&ctx);

    fclose(binary);

    return 0;
}