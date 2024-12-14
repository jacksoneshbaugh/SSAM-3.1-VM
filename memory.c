// Contains the memory array, and R/W functionality for it.
// Also contains functionality to load a file into memory.
// Created by Jackson Eshbaugh on 28.10.2024.

#include "memory.h"

#define MEMORY_SIZE 0xFFFF

unsigned char memory[MEMORY_SIZE];

unsigned char getByte(unsigned short address) {
    return memory[address];
}

unsigned short getWord(unsigned short address) {
    return memory[address] << 8 | memory[address + 1];
}

void setByte(unsigned short address, unsigned char value) {
    memory[address] = value;
}

void setWord(unsigned short address, unsigned short value) {
    // Break short into two chars
    unsigned char top = (value >> 8) & 0xFF;
    unsigned char bottom = value & 0xFF;
    memory[address] = top;
    memory[address + 1] = bottom;
}

void loadProgram(FILE *fileHandler) {
    int inputDataSize = 0;

    while(!feof(fileHandler)) {
        // Continue progressing through the input data, reading one byte at a time
        // until reaching the end of the file.
        fread((memory + inputDataSize), 1, 1, fileHandler);
        inputDataSize++;
    }
    fclose(fileHandler);

    if(ferror(fileHandler)) {
        fprintf(stderr, "Error reading file at size %d.\n", inputDataSize);
    }
}