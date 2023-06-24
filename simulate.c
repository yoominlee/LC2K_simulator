/* LC-2K Instruction-level simulator */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000 


typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int num);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; // 세번째 파라미터에서 최대 두번째 파라미터 자리까지 읽어 첫번째 파라미터 자리에 넣음
            state.numMemory++) {

        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

	/* TODO: */
    /* Initialize all registers and the program counter to 0 */
    state.pc = 0;
    for (int j = 0; j < NUMREGS; j++){
        state.reg[j] = 0;
    }
    /* Simulate the program until the program executes a halt */
    int notHalt = 1;
    int instructionCount = 0;

    // int loopcount = 0; // !! 나중에 지우기!!!

    while (notHalt){ // 0 false 1 true
        printState(&state);  // !! 나중에 주석 해제!!
        int instruction = state.mem[state.pc]; // while문 한번 돌때마다 memory에 있는 instruction 한 줄씩
        int opcode, regA, regB;
        opcode = (instruction >> 22) & 0b111; // bits 24-22: opcode
        regA = (instruction >> 19) & 0b111; // bits 21-19: regA
        regB = (instruction >> 16) & 0b111; // bits 18-16: regB
        
        int result, destReg, offsetField;
        switch (opcode){
        case 0: // ADD 000 - Add contents of regA with contents of regB, store results in destReg.
            // printf("case 0\n");
            result = state.reg[regA] + state.reg[regB];
            destReg = instruction & 0b111;
            state.reg[destReg] = result;
            break;
        case 1: // NOR 001 - Nor contents of regA with contents of regB, store results in destReg. This is a bitwise nor; each bit is treated independently.
            // printf("case 1\n");
            result = ~(state.reg[regA] | state.reg[regB]);
            destReg = instruction & 0b111;
            state.reg[destReg] = result;        
            break;
        case 2: // LW 010 - Load regB from memory. Memory address is formed by adding offsetField with the contents of regA
            // printf("case 2\n");
            offsetField = instruction & 0xFFFF;
            state.reg[regB] = state.mem[convertNum(offsetField + state.reg[regA])];
            break;
        case 3: // SW 011 - Store regB into memory. Memory address is formed by adding offsetField with the contents of regA.
            // printf("case 3\n");
            offsetField = instruction & 0xFFFF;
			state.mem[convertNum(offsetField + state.reg[regA])] = state.reg[regB];        
            break;
        case 4: // BEQ 100 - If the contents of regA and regB are the same, then branch to the address PC+1+offsetField, where PC is the address of this beq instruction.
            // printf("case 4\n");
            offsetField = instruction & 0xFFFF;
            if (state.reg[regA] == state.reg[regB]) {
				state.pc = state.pc + convertNum(offsetField);
            }
            break;
        case 5: // JALR 101 - First store PC+1 into regB, where PC is the address of this jalr instruction. Then branch to the address contained in regA. Note that this implies if regA and regB refer to the same register, the net effect will be jumping to PC+1.
            // printf("case 5\n");
            state.reg[regB] = state.pc + 1;
			state.pc = state.reg[regA] - 1;
            break;
        case 6: // HALT 110 - Increment the PC (as with all instructions), then halt the machine (let the simulator notice that the machine halted).
            // printf("case 6\n");
            notHalt = 0;
            printf("machine halted\n");
            break;
        case 7: // NOOP 111 - Do nothing.
            // printf("case 7\n");
            // loopcount++;
            break;
        
        default:
            break;
        }
        state.pc++;
        instructionCount++;
        // printf("instructionCount: %d\n",instructionCount); // !! 나중에 지우기!!!
        // printf("loopcount: %d\n",loopcount); // !! 나중에 지우기!!!

    }
    printf("total of %d instructions executed\n", instructionCount);
    printf("final state of machine:\n");
    printState(&state);


    return(0);
}

void printState(stateType *statePtr) // before executing each instruction, once before exiting the program
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);  // !! 나중에 주석 해제!!
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++) {
        printf("\t\treg[ %d ] %d \n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return (num);
}
