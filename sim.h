// Implements the user interface for the VM.
// Created by Jackson Eshbaugh on 06.11.2024.

#ifndef SIM_H
#define SIM_H

#include <stdio.h>

/**
 * Prints to the console the current state of the VM.
 */
void printState(int originalBP, int originalPC);

/**
 * Logs the current VM state to the given logFile. This gives the same information as
 * printState(), but directs it to the specified logFile.
 *
 * @param logFile the file to log the state to
 */
void logState(FILE *logFile, int originalBP, int originalPC);

/**
 * The entry point of the VM. Loads in the specified code, then accepts user input to
 * control the VM.
 *
 * @param argc
 * @param argv
 */
int main(int argc, char *argv[]);

#endif //SIM_H
