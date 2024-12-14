// Implementation of the processor in the VM.
// Created by Jackson Eshbaugh on 30.10.2024.

#ifndef CONTROLLER_H
#define CONTROLLER_H

typedef enum {
 R0 = 0,
 R1 = 1,
 R2 = 2,
 R3 = 3,
 AC = 4,
 SP = 5,
 BP = 6,
 PC = 7,
 IR = 8
} Register;

/**
 * Initializes the controller by setting the registers to their correct default values.
 *
 * @param sp the initial stack pointer (the base pointer will be calculated as sp + 0x02)
 * @param pc the initial program counter
 */
void controllerInit(short sp, short pc);

/**
 * Performs one fetch operation.
 */
void fetch();

/**
 * Decodes the fetched operation, then executes it.
 * This is implemented with the help of helper functions for each possible operation.
 * fetch() should be called before execute().
 */
void execute();

/**
 * Determines if a halt was reached
 * @return 1 if halt reached, 0 otherwise
 */
int haltReached();

/**
 * Determines if an error has occurred
 * @return 1 if error occurred, 0 otherwise
 */
int errorOccurred();

/**
 * Gets the contents of a specified register.
 * @param reg the register to get the contents of
 * @return the contents of the register reg
 */
unsigned short getRegister(Register reg);

#endif //CONTROLLER_H
