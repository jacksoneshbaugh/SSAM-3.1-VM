// Implements the user interface for the VM.
// Created by Jackson Eshbaugh on 06.11.2024.

#include "sim.h"
#include "memory.h"
#include "controller.h"
#include <stdio.h>

#define BUFFER_SIZE 1024

/**
 * Takes a string of the form "0x<hex>" and converts it to its hex value.
 * @param str the string to convert to hex
 * @return the hex value taken from the string.
 */
short strToHex(char *str) {
    short hex = 0;
    str += 2; // Skip over "0x"
    while (*str) {
        char c = *str;
        if (c >= '0' && c <= '9') {
            hex = (hex << 4) + (c - '0');
        } else if (c >= 'A' && c <= 'F') {
            hex = (hex << 4) + (c - 'A' + 10);
        } else if (c >= 'a' && c <= 'f') {
            hex = (hex << 4) + (c - 'a' + 10);
        } else {
            return -1; // Error
        }
        str++;
    }
    return hex;
}

void printState(int originalBP, int originalPC) {
    // Print error and halt statuses
    if (errorOccurred() && haltReached()) printf("[ERROR]      [HALT]\n\n");
    else if (errorOccurred()) printf("[ERROR]\n\n");
    else if (haltReached()) printf("[HALT]\n\n");

    // Headers for table
    printf(" REGISTERS                MEMORY                PROGRAM MEMORY\n");
    printf("----------------------------------------------------------------------\n");

    short regCounter = 0;
    int stackAddr = originalBP;
    int progAddr = originalPC;

    for (int i = 0; i < getRegister(BP); i++) {
        // Print registers
        if (regCounter < 9) {
            const char* registerNames[] = {"R0", "R1", "R2", "R3", "AC", "SP", "BP", "PC", "IR"};
            printf("%-3s: 0x%04hx          ", registerNames[regCounter], getRegister(regCounter));
            regCounter++;
        } else {
            printf("                     "); // Empty space when there are no more registers to iterate through
        }

        // Print stack memory
        printf("0x%04hx: 0x%04hx", stackAddr, getWord(stackAddr));
        if (stackAddr == getRegister(SP)) printf("  [SP]       ");
        else if (stackAddr == getRegister(BP)) printf("  [BP]       ");
        else printf("             ");

        // Print program memory
        if (progAddr < originalPC + 40) {
            printf("0x%04hx: 0x%04hx", progAddr, getWord(progAddr));
            if (progAddr == getRegister(PC)) printf("  <== PC");
        }

        printf("\n"); // New row
        stackAddr += 2;
        progAddr += 2;
    }
}

void logState(FILE *logFile, int originalBP, int originalPC) {
    // Print error and halt statuses
    if (errorOccurred() && haltReached()) fprintf(logFile, "[ERROR]      [HALT]\n\n");
    else if (errorOccurred()) fprintf(logFile, "[ERROR]\n\n");
    else if (haltReached()) fprintf(logFile, "[HALT]\n\n");

    // Headers for table
    fprintf(logFile, " REGISTERS                MEMORY                PROGRAM MEMORY\n");
    fprintf(logFile, "----------------------------------------------------------------------\n");

    short regCounter = 0;
    int stackAddr = originalBP;
    int progAddr = originalPC;

    for (int i = 0; i < getRegister(BP); i++) {
        // Print registers
        if (regCounter < 9) {
            const char* registerNames[] = {"R0", "R1", "R2", "R3", "AC", "SP", "BP", "PC", "IR"};
            fprintf(logFile, "%-3s: 0x%04hx          ", registerNames[regCounter], getRegister(regCounter));
            regCounter++;
        } else {
            fprintf(logFile, "                     "); // Empty space when there are no more registers to iterate through
        }

        // Print stack memory
        fprintf(logFile, "0x%04hx: 0x%04hx", stackAddr, getWord(stackAddr));
        if (stackAddr == getRegister(SP)) fprintf(logFile, "  [SP]       ");
        else if (stackAddr == getRegister(BP)) fprintf(logFile, "  [BP]       ");
        else fprintf(logFile, "             ");

        // Print program memory
        if (progAddr < originalPC + 40) {
            fprintf(logFile, "0x%04hx: 0x%04hx", progAddr, getWord(progAddr));
            if (progAddr == getRegister(PC)) fprintf(logFile, "  <== PC");
        }

        fprintf(logFile, "\n"); // New row
        stackAddr += 2;
        progAddr += 2;
    }
}

int main(int argc, char *argv[]) {
    // Expected arguments:
    // - bin file
    // - sp
    // - pc

    if(argc < 4) {
        fprintf(stderr, "Usage: ./vm <file.bin> <stack pointer start> <program counter start>\n");
        return 0;
    }


    printf("Initializing...\n");

    int sp = strToHex(argv[2]), pc = strToHex(argv[3]);

    // initialize the VCPU
    controllerInit(sp, pc);
    printf("Stack Pointer: %hi / Base Pointer: %hi / Program Counter: %hi\n", getRegister(SP), getRegister(BP), getRegister(PC));

    // Load program code into memory (init VRAM)
    FILE *binary = fopen(argv[1], "rb");
    if(!binary) {
        fprintf(stderr, "Error: specified binary file \"%s\" could not be loaded.\n", argv[1]);
        return 0;
    }

    printf("Loading program \"%s\"\n", argv[1]);
    loadProgram(binary);
    fclose(binary);

    printf("Welcome to SSAM VM.\n\n");

    FILE *file;

    while(1) {
        // Accept a new command
        char buffer[BUFFER_SIZE];
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);

        int i = 0;
        while (buffer[i] != '\n') {
            // Loop through each command in the string
            switch(buffer[i]) {
                case 'Q':
                    // Open the file dump_log.txt and log the VM state to it.
                    file = fopen("dump_log.txt", "w");
                    if(!file) {
                        fprintf(stderr, "Error: dump_log.txt could not be opened.\n");
                        return 0;
                    }
                    logState(file, sp - 0x02, pc);
                    fclose(file);
                case 'q':
                    // Quit if 'q' and after dumping to dump_log.txt for 'Q'
                    return 0;
                case 'd':
                    // Print the state to the console
                    printState(sp - 0x02, pc);
                    break;
                case 'n':
                    // Run one fetch-execute cycle
                    fetch();
                    execute();
                    break;
                case 'N':
                    // Run one fetch-execute cycle, then print the VM state
                    fetch();
                    execute();
                    printState(sp - 0x02, pc);
                    break;
                case 'H':
                    // Run fetch-execute cycles until halt is reached.
                    while(!haltReached()) {
                        fetch();
                        execute();
                    }
                    break;
                default:
                    fprintf(stderr, "Error: Unrecognized command \"%c\". Check the README.md file for the list of commands.\n", buffer[i]);
                    break;
            }
            i++;
        }

    }

}