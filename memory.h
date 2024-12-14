// Contains the memory array, and R/W functionality for it.
// Also contains functionality to load a file into memory.
// Created by Jackson Eshbaugh on 28.10.2024.

#ifndef MEMORY_H
#define MEMORY_H
#include <stdio.h>

/**
 * Memory[address] <== byte
 * Sets the byte at the given address to the value given by byte.
 * @param address the address to update
 * @param byte the value to write at address
 */
void setByte(unsigned short address, unsigned char byte);

/**
 * Memory[address] <== word
 * Sets the word at the given address to the value given by word.
 * @param address the address to update
 * @param word the value to write at address
 */
void setWord(unsigned short address, unsigned short word);

/**
 * (ret) <== Memory[address]
 * Fetches the byte at the given address.
 * @param address the address to read memory at
 * @return the byte located at address in memory
 */
unsigned char getByte(unsigned short address);

/**
 * Fetches the word at the given address.
 * @param address the address to read memory at
 * @return the word located at address in memory
 */
unsigned short getWord(unsigned short address);

/**
 * Loads the program into memory from the provided file.
 * @param fileHandler the file to load bytes into memory from.
 */
void loadProgram(FILE *fileHandler);

#endif //MEMORY_H
