#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define PROGRAM_LENGTH 649

int accumulator = 0;

typedef enum Operator {
    ACC,
    JMP,
    NOP
} Operator;

typedef struct Instruction {
    Operator op;
    int arg;
    bool visited;
} Instruction;

void parse_program(FILE* input, Instruction** program);
bool run_program(Instruction* program);

int main(int argc, char** argv) {
    FILE* input = fopen(argv[1], "r");
    Instruction* program = malloc(sizeof(Instruction) * PROGRAM_LENGTH);
    parse_program(input, &program);
    fclose(input);

    run_program(program);
    printf("Solution 1: %d\n", accumulator);

    for (int i = 0; i < PROGRAM_LENGTH; i++) {
        if (program[i].op == NOP) program[i].op = JMP;
        else if (program[i].op == JMP) program[i].op = NOP;
        if (run_program(program)) break;
        if (program[i].op == NOP) program[i].op = JMP;
        else if (program[i].op == JMP) program[i].op = NOP;
    }
    printf("Solution 2: %d\n", accumulator);
}

bool streq(char* s1, char* s2) {
    return strcmp(s1, s2) == 0;
}

Operator parse_operator(char* s) {
    if (streq(s, "acc")) return ACC;
    else if (streq(s, "jmp")) return JMP;
    else if (streq(s, "nop")) return NOP;
    assert(0);
}

void parse_program(FILE* input, Instruction** program) {
    char* line = NULL;
    size_t len = 0;
    int read = 0;

    for (int i = 0; i < PROGRAM_LENGTH; i++) {
        read = getline(&line, &len, input);
        (*program)[i].op = parse_operator(strtok(line, " "));
        (*program)[i].arg  = strtol(strtok(NULL, "\n"), NULL, 10);
        (*program)[i].visited = false;
    }
}

void reset_program(Instruction* program) {
    accumulator = 0;
    for (int i = 0; i < PROGRAM_LENGTH; i++) {
        program[i].visited = false;
    }
}

bool run_program(Instruction* program) {
    reset_program(program);
    int pc = 0;
    while (true) {
        if (pc < 0 || pc >= PROGRAM_LENGTH || program[pc].visited) return false;
        Instruction *instruction = program + pc;
        instruction->visited = true;
        switch (instruction->op) {
            case NOP:
                pc++;
                break;
            case JMP:
                pc += instruction->arg;
                break;
            case ACC:
                accumulator += instruction->arg;
                pc++;
                break;
            default: assert(0);
        }
        if (pc == PROGRAM_LENGTH) return true;
    }
}
