# SSAM Virtual Machine
For a lab assignment for CS 203 (Computer Organization) at Lafayette College, I was tasked with developing a virtual machine that can run SSAM (Super Simple Assembly, developed by my professor) machine code. Tasks included developing VRAM (`memory.c` and `memory.h`), a VCPU (`controller.c` and `controller.h`), and a UI (`sim.c` and `sim.h`).

I learned a bunch about how lower levels of computing work from this class—and this lab tied plenty of concepts together.

## Using the VM
To use the VM, you'll need to provide three command-line arguments:
- the binary file itself
- the initial base pointer location, in hex (the stack pointer will be calculated using the base pointer)
- the initial program counter, in hex

To call the program,

```zsh
cd cmake-build-debug
./vm <source-file.bin> <0xinitial-base-pointer> <0xinitial-program-counter>
```

To run each program respectively:
- `./vm example1.bin 0x0100 0x0600`
- `./vm inclass_fib.bin 0x0100 0x0600`
- `./vm hw4_a.bin 0x0100 0x0400`
- `./vm hw4_b.bin 0x0100 0x0400`
- `./vm hw5_a.bin 0x0100 0x0400`
- `./vm hw5_b.bin 0x0100 0x0400`

## Included Programs
I've provided a few sample programs that you can run. Most programs have an associated `.s` source assembly file. I will not provide the assembler itself as it is the intellectual property of my professor, but I hope you can get a rough understanding of what these programs do from their source files.