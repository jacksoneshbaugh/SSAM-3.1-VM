// Implementation of the processor in the VM.
// Created by Jackson Eshbaugh on 30.10.2024.

#include "controller.h"

#include "memory.h"
#define REG_COUNT 9

char flags = 0x0; // 0th bit is the haltReached flag; 1st is the error flag.
unsigned short R[REG_COUNT];

// flow operations

/**
 * Returns from a call.
 * RTN:
 * R[SP] <== R[BP]
 * R[BP] <== M[R[SP]]
 * R[SP] <== R[SP] - 0x02
 * R[PC] <== M[R[SP]]
 */
void ret() {
    R[SP] = R[BP];
    R[BP] = getWord(R[SP]);
    R[SP] = R[SP] - 0x02;
    R[PC] = getWord(R[SP]);
}

// transfer operations

/**
 * Loads the immediate value into the given register.
 * RTN:
 * R[reg] <== immediate
 * @param reg the register to load the immediate into
 * @param immediate the immediate to load into the register
 */
void lodi(Register reg, char immediate) {
    R[reg] = (short) immediate;
}

/**
 * Loads the value stored at address in memory into register reg.
 * RTN:
 * R[reg] <== M[address]
 * @param reg the register to load the value into
 * @param address the location of the value in memory
 */
void loda(Register reg, char address) {
    R[reg] = getWord(address);
}

/**
 * Loads the value stored in memory at the address given in regB into regA.
 * RTN:
 * R[regA] <== M[R[regB]]
 * @param regA the register to load the value into
 * @param regB the location to load from in memory
 */
void lodr(Register regA, Register regB) {
    R[regA] = getWord(regB);
}

/**
 * Loads the value from memory at location R[regB] + index into regA.
 * RTN:
 * R[regA] <== M[R[regB] + offset]
 * @param regA register to load the value into
 * @param regB register with base memory address to load at
 * @param offset index by which to offset R[regB] by when indexing
 */
void lodrd(Register regA, Register regB, char offset) {
    R[regA] = getWord(R[regB] + offset);
}

/**
 * Stores the contents of reg into memory at address.
 * RTN:
 * M[address] <== R[reg]
 * @param reg the register to store in memory
 * @param address the location to store R[reg] in memory
 */
void stoa(Register reg, char address) {
    setWord(address, R[reg]);
}

/**
 * Stores the value in regA in memory at address regB.
 * RTN:
 * M[R[regB]] <== R[regA]
 * @param regA the register holding the value to store
 * @param regB the register holding the address to store at
 */
void stor(Register regA, Register regB) {
    setWord(R[regB], R[regA]);
}

/**
 * Stores the value in regA in memory at address regB, with offset index.
 * RTN:
 * M[R[regB] + offset] <== R[regA]
 * @param regA the register holding the value to store at R[regB] + offset in memory
 * @param regB the register holding the address to store R[regA] at
 * @param offset the offset from the address R[regB] to store R[regA] at
 */
void stord(Register regA, Register regB, char offset) {
    setWord(R[regB] + offset, R[regA]);
}

// manipulate operations

/**
 * Negates the contents of reg, storing them in the accumulator.
 * RTN:
 * R[AC] <== -R[reg]
 * @param reg the register to negate
 */
void neg(Register reg) {
    // R[AC] = ~R[reg];
    // R[AC] += 1;
    R[AC] = -R[reg];
}

/**
 * Adds the values stored in regA and regB, storing the result in the accumulator.
 * RTN:
 * R[AC] <== R[regA] + R[regB]
 * @param regA an addend
 * @param regB another addend
 */
void addr(Register regA, Register regB) {
    R[AC] = R[regA] + R[regB];
}

/**
 * Adds the value stored in regA and an immediate value, storing the result in the accumulator.
 * RTN:
 * R[AC] <== R[reg] + immediate
 * @param reg an addend
 * @param immediate another addend
 */
void addi(Register reg, char immediate) {
    R[AC] = R[reg] + immediate;
}

/**
 * Subtracts the values stored in regA and regB, storing the result in the accumulator.
 * RTN:
 * R[AC] <== R[regA] - R[regB]
 * @param regA the value to subtract R[regB] from
 * @param regB the value to subtract from R[regA]
 */
void subr(Register regA, Register regB) {
    R[AC] = R[regA] - R[regB];
}

/**
 * Subtracts the immediate value from R[reg] and stores the result in the accumulator.
 * RTN:
 * R[AC] <== R[reg] - immediate
 * @param reg the register to subtract the immediate from
 * @param immediate the immediate to subtract from the register
 */
void subi(Register reg, char immediate) {
    R[AC] = R[reg] - immediate;
}

/**
 * Copies the value from regA into regB.
 * RTN:
 * R[regB] <== R[regA]
 * @param regA the value to copy into regB
 * @param regB the location to copy the value in regA to.
 */
void mov(Register regA, Register regB) {
    R[regA] = R[regB];
}

// jmp operations

/**
 * Jumps to the given address.
 * RTN:
 * R[PC] <== address
 * @param address the address to jump to
 */
void jmp(short address) {
    R[PC] = address;
}

/**
 * Jumps to the given address if R[AC] is zero.
 * RTN:
 * R[PC] <== address [IF R[AC] == 0]
 * @param address the address to jump to
 */
void jmpz(short address) {
    if (R[AC] == 0x0000) R[PC] = address;
}

/**
 * Jumps to the given address if R[AC] is negative.
 * RTN:
 * R[PC] <== address [IF R[AC] < 0]
 * @param address the address to jump to
 */
void jmpn(short address) {
    if ((short) R[AC] < 0) R[PC] = address;
}

/**
 * Calls a function at the given address (set up a new stack frame).
 * RTN:
 * M[R[SP]] <== R[PC]
 * R[SP] <== R[SP] + 0x02
 * R[PC] <== address
 * M[R[SP]] <== R[BP]
 * R[BP] <== R[SP]
 * R[SP] <== R[SP] + 0x02
 * @param address the address to call
 */
void call(short address) {
    setWord(R[SP], R[PC]);
    R[SP] = R[SP] + 0x02;
    R[PC] = address;
    setWord(R[SP], R[BP]);
    R[BP] = R[SP];
    R[SP] = R[SP] + 0x02;
}


void controllerInit(short sp, short pc) {
    R[R0] = 0x0000;
    R[R1] = 0x0000;
    R[R2] = 0x0000;
    R[R3] = 0x0000;
    R[AC] = 0x0000;
    R[SP] = sp;
    R[BP] = sp - 0x02;
    R[PC] = pc;
    R[IR] = 0x0000;
    flags = 0x0;
}

int errorOccurred() {
    return flags & 0x2;
}

int haltReached() {
    return flags & 0x1;
}

void fetch() {
    if (haltReached()) return; // Don't do any more work if a halt was reached

    // Copy the contents of memory at PC into IR.
    // R[IR] <== M[R[PC]]
    R[IR] = getWord(R[PC]);
    // Increment the PC
    R[PC] = R[PC] + 0x02;
}

void execute() {
    if (haltReached()) return; // Don't do any more work if a halt was reached

    int offset;

    // Decode the instruction in IR,
    // then execute that instruction
    switch (R[IR] & 0xc000) {
        case 0x0000:
            // flow operation
            switch (R[IR] & 0x1800) {
                case 0x0000:
                    // halt; set halt flag
                    flags |= 0x1;
                    break;
                case 0x0800:
                    // nop; do nothing
                    break;
                case 0x1000:
                    // ret
                    ret();
                    break;
                default:
                    // error; set error flag
                    flags |= 0x2;
                    break;
            }
            break;
        case 0x4000:
            switch (R[IR] & 0x3800) {
                // transfer operation
                case 0x0000:
                    lodi((R[IR] & 0x0700) >> 8, R[IR] & 0x00ff);
                break;
                case 0x0800:
                    loda((R[IR] & 0x0700) >> 8, R[IR] & 0x00ff);
                break;
                case 0x1000:
                    lodr((R[IR] & 0x0700) >> 8, (R[IR] & 0x00e0) >> 5);
                break;
                case 0x1800:
                    offset = R[IR] & 0x001f;
                    if (offset & 0x10) {
                        offset -= 32;
                    }
                    lodrd((R[IR] & 0x0700) >> 8, (R[IR] & 0x00e0) >> 5, offset);
                break;
                case 0x2000:
                    stoa((R[IR] & 0x0700) >> 8, R[IR] & 0x00ff);
                break;
                case 0x2800:
                    stor((R[IR] & 0x0700) >> 8, (R[IR] & 0x00e0) >> 5);
                break;
                case 0x3000:
                    offset = R[IR] & 0x001f;
                    if (offset & 0x10) {
                        offset -= 32;
                    }
                    stord((R[IR] & 0x0700) >> 8, (R[IR] & 0x00e0) >> 5, offset);
                break;
                default:
                    flags |= 0x2;
                break;
            }
            break;
        case 0x8000:

            // manipulate operation
            switch(R[IR] & 0x3800) {
                case 0x0000:
                    neg((R[IR] & 0x0700) >> 8);
                    break;
                case 0x0800:
                    addr((R[IR] & 0x0700) >> 8, (R[IR] & 0x00ff) >> 5);
                    break;
                case 0x1000:
                    addi((R[IR] & 0x0700) >> 8, R[IR] & 0x00ff);
                    break;
                case 0x1800:
                    subr((R[IR] & 0x0700) >> 8, (R[IR] & 0x00ff) >> 5);
                    break;
                case 0x2000:
                    subi((R[IR] & 0x0700) >> 8, R[IR] & 0x00ff);
                    break;
                case 0x3800:
                    mov((R[IR] & 0x0700) >> 8, (R[IR] & 0x00ff) >> 5);
                    break;
                default:
                    flags |= 0x2;
                    break;
            }
            break;
        case 0xc000:
            // jump operation
            switch (R[IR] & 0x3000) {
                case 0x0000:
                    // 1100
                    jmp(0x0fff & R[IR]);
                    break;

                case 0x1000:
                    // 1101
                    jmpz(0x0fff & R[IR]);
                    break;

                case 0x2000:
                    // 1110
                    jmpn(0x0fff & R[IR]);
                    break;

                case 0x3000:
                    // 1111
                    call(0x0fff & R[IR]);
                    break;

                default:
                    flags |= 0x2;
                    break;
            }
            break;
        default:
            // Operation not recognized; set error flag
            flags |= 0x2;
            break;
    }
}

unsigned short getRegister(Register reg) {
    return R[reg];
}