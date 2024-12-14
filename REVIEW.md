# SSAM 3.1 VM
**Project Goal**: Create a virtual machine that runs SSAM machine code.

**VM Lifetime**:
- While user does not input exit command:
  - Take and decode input, run executed command.
  - To execute machine instructions, we use the fetch-execute cycle (call `fetch()`, then `execute()`).
  - `fetch()` simply pulls the next instruction into the instruction register (using tools from `memory.h`), then increments the program counter by 2.
  - `execute()` reads the contents of the IR, and decodes it, running the instruction. Functionally, this is nothing more than a `switch` statement.
  - Each instruction is modeled by its own function, which implements the RTN necessary to execute it.


**Improvements**:
- Unit testing
- Reducing magic numbers by using macros