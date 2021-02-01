#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include "../libmc4/emu.h"

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

    int read = fread(memory->Data, 1, MAX_MEM, binary);
    if (read < MAX_MEM) {
        fprintf(stderr, "File too small or error!\n");
        free(memory);
        fclose(binary);

        return 1;
    }

    resetCPU(&cpu, memory);
    run(&cpu, memory, 0, 1);

    free(memory);
    fclose(binary);

    return 0;
}