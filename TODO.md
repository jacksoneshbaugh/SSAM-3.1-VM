# TODO

- [x] `memory.c` — implementation of the virtual memory (VRAM) [as a `char` array] and getters and setters for storing/retrieving information to/from VRAM.
  - [x] `void loadProgram(FILE *fileHandler)` — loads the contents of the binary file into the VRAM.
  - [x] `unsigned char getByte(short address)` — retrieves a specific character from the memory
  - [x] `unsigned short getWord(short address)` — retrieves a specific word which begins at address from memory (word size is 2 bytes)
  - [x] `void setByte(short address, char value)` — sets a specific character in the memory to be a given value
- [x] `controller.c` — implementation of the virtual CPU. Contains registers, processor logic, and flags: `haltReached` and `error`. Global variables are used to store the correct initialization values for registers `SP`, `BP`, and `PC`.
  - [x] `void controllerInit(short sp, short bp, short pc)` — initializes the registers to their correct default values.
  - [x] `void fetch()` — runs one fetch operation.
  - [x] `void execute()` – decodes the fetched operation, then executes it (`fetch()` should be called before `execute()`). This is implemented with the help of helper functions for each possible operation.
- [x] `sim.c` — implementation of the simulation at a high level: user control, printing state, logfiles, etc.).
  - [x] `void printState(int originalBP, int originalPC)` — prints, to the console, the current state of the VM.
  - [x] `void logState(FILE *logFile, int originalBP, int originalPC)` — logs the current VM state to a given logfile. This is the same information as is printed by `printState()`, just to a specified log file instead.
  - [x] `void main(int argc, char *argv)` — The entry point for the VM. The desired binary file is passed in as a command line argument, along with the starting positions for the stack pointer and program counter, like this: `./vm "./binary.bin" 0x012 0x610`. Then, `main` implements the user interface, requesting input from the user and using the functionality exposed by `controller.c` to execute these commands. These can be provided in sequence. For example, `nnndq` would be a valid input. Will need to loop through the input char-by-char and process that way.
    - Command Dictionary:
      - `q` — quits the program.
      - `Q` — quits the program, dumping the current VM state to a log file `dump_log.txt`.
      - `d` — displays the current state of the VM (to the console)
      - `n` — runs one fetch-execute cycle.
      - `N` — runs one fetch-execute cycle, then displays VM state in the console. Equivalent to `nd`.
      - `H` — runs the loaded program until a halt command is reached.
  
- [x] Compile other HW solutions and run them.
- [x] `README.md` — write some documentation for the VM.