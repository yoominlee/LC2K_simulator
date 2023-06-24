# LC2K_simulator
A program that can simulate any legal LC-2K machine-code program.

## SIMULATOR
Input : the machine-code file which is created with assembler.   
Output : txt file that shows the status of each program counter(PC).   
- initialize all registers and the program counter to 0 at the beginning.
- simulates the program until the program executes a halt
- printState() : prints the current state of the machine (program counter, registers, memory).   
  called before executing each instruction and once just before exiting the program
  
### simulate.c
Main file that simulates with .mc file.

### spec.mc
Example of simulator input

### spec.out.correct
Example of simulator output   
(correct answer for spec.mc)

### testcases directory
Test cases to validate any LC-2K simulator.   
- 5 paired(input-output) case.   
- Only includes normal cases. not the error case.

input: mc file(output of assembler)  
output: txt file

## To run it
```
simulate program.mc > output
```

This directs all printf()s to the file “output”
