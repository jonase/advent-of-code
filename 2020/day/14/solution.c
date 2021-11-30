#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef enum InstructionType {
    ASSIGNMENT,
    MASK
} InstructionType;

typedef struct Assignment {
    uint64_t memory_address;
    uint64_t value;
} Assignment;

typedef struct Instruction {
    InstructionType instruction_type;
    union {
        Assignment assignment;
        char* mask;
    };
} Instruction;


typedef struct Memory Memory;
Memory* memory_init();
void memory_set(Memory*, uint64_t address, uint64_t value);
uint64_t memory_sum(Memory*);
void memory_free(Memory*);

void parse_instruction(char*, Instruction*);
void execute_instruction_v1(Instruction, char** mask, Memory* memory);
void execute_instruction_v2(Instruction, char** mask, Memory* memory);

int main(int argc, char** argv) {
    FILE* input = fopen(argv[1], "r");

    char* mask_v1 = NULL;
    Memory* memory_v1 = memory_init();

    char* mask_v2 = NULL;
    Memory* memory_v2 = memory_init();

    size_t len = 0;
    char* line = NULL;
    while(getline(&line, &len, input) != EOF) {
        Instruction instruction;
        parse_instruction(line, &instruction);
        execute_instruction_v1(instruction, &mask_v1, memory_v1);
        execute_instruction_v2(instruction, &mask_v2, memory_v2);
    }

    printf("Solution 1: %llu\n", memory_sum(memory_v1));
    printf("Solution 2: %llu\n", memory_sum(memory_v2));

    memory_free(memory_v1);
    memory_free(memory_v2);

    fclose(input);
}

void parse_instruction(char* line, Instruction* instruction) {
    if (line[1] == 'e') {
        // mem[nnn] = mmm
        instruction->instruction_type = ASSIGNMENT;
        uint64_t address = strtol(line + 4, NULL, 10);
        char* eq = strchr(line, '=');
        uint64_t value = strtol(eq+1, NULL, 10);
        instruction->assignment = (Assignment) { address, value };
    } else {
        // mask = nnn
        instruction->instruction_type = MASK;
        instruction->mask = strndup(line + 7, 36);
    }
}

void execute_instruction_v1(Instruction instruction, char** mask, Memory* memory) {
    uint64_t value = 0;
    switch (instruction.instruction_type) {
        case MASK:
            *mask = instruction.mask;
            break;
        case ASSIGNMENT:
            value = instruction.assignment.value;
            for (int i = 0; i < 36; i++) {
                char ch = (*mask)[i];
                switch (ch) {
                    case '1':
                        value |= 1ul << (35 - i);
                        break;
                    case '0':
                        value &= ~(1ul << (35 - i));
                        break;
                }
            }

            memory_set(memory, instruction.assignment.memory_address, value);
            break;
        default: assert("Unknown Instruction" && 0);
    }
}


void execute_instruction_v2(Instruction instruction, char** mask, Memory* memory) {
    uint64_t* addresses = NULL;
    int address_count = 1;
    int cycle;
    switch (instruction.instruction_type) {
        case MASK:
            *mask = instruction.mask;
            break;
        case ASSIGNMENT:
            for (int i = 0; i < 36; i++) {
                if ((*mask)[i] == 'X') {
                    address_count *= 2;
                }
            }

            cycle = address_count;
            addresses = (uint64_t*) malloc(sizeof(uint64_t)*address_count);
            for (int i = 0; i < address_count; i++) {
                addresses[i] = instruction.assignment.memory_address;
            }

            for (int i = 0; i < 36; i++) {
                char ch = (*mask)[i];
                switch (ch) {
                    case '1':
                        for (int k = 0; k < address_count; k++) {
                            addresses[k] |= 1ul << (35 - i);
                        }
                        break;
                    case '0':
                        break;
                    case 'X':
                        cycle = cycle / 2;
                        for (int k = 0; k < address_count; k++) {
                            if ((k / cycle) & 1) {
                                addresses[k] |= 1ul << (35 - i);
                            } else {
                                addresses[k] &= ~(1ul << (35 - i));
                            }
                        }
                        break;
                }
            }

            for (int i = 0; i < address_count; i++) {
                memory_set(memory, addresses[i], instruction.assignment.value);
            }

            free(addresses);
            break;
        default: assert("Unknown Instruction" && 0);
    }
}

typedef struct MemoryNode  {
    uint64_t address;
    uint64_t value;
    struct MemoryNode* next;
} MemoryNode;

MemoryNode* memory_node_init(uint64_t address, uint64_t value) {
    MemoryNode* item = (MemoryNode*) malloc(sizeof(MemoryNode));
    item->address = address;
    item->value = value;
    item->next = NULL;
    return item;
}

void memory_node_free(MemoryNode* node) {
    if (node == NULL) {
        return;
    } else {
        memory_node_free(node->next);
        free(node);
    }
}

void memory_node_insert(MemoryNode* memory_node, uint64_t address, uint64_t value) {
    MemoryNode* current = memory_node;

    if (memory_node->address == address) {
        memory_node->value = value;
        return;
    }

    while(current->next != NULL) {
        current = current->next;
        if (current->address == address) {
            current->value = value;
            return;
        }
    }
    current->next = memory_node_init(address, value);
}


#define MEMORY_BUCKETS 4153

struct Memory {
    MemoryNode *buckets[MEMORY_BUCKETS];
};

Memory* memory_init() {
    Memory* memory = (Memory*) malloc(sizeof(Memory));
    for (int i = 0; i < MEMORY_BUCKETS; i++) {
        memory->buckets[i] = NULL;
    }
    return memory;
}

void memory_free(Memory* memory) {
    for (int i = 0; i < MEMORY_BUCKETS; i++) {
        if (memory->buckets[i] != NULL) {
            memory_node_free(memory->buckets[i]);
        }
    }
    free(memory);
}

int hash(uint64_t address) {
    return address % MEMORY_BUCKETS;
}

void memory_set(Memory* memory, uint64_t address, uint64_t value) {
    int bucket_idx = hash(address) % MEMORY_BUCKETS;
    if (memory->buckets[bucket_idx] == NULL) {
        MemoryNode* item = memory_node_init(address, value);
        memory->buckets[bucket_idx] = item;
    } else {
        memory_node_insert(memory->buckets[bucket_idx], address, value);
    }
}

uint64_t memory_sum(Memory* memory) {
    uint64_t sum = 0;
    for (int i = 0; i < MEMORY_BUCKETS; i++) {
        if (memory->buckets[i] != NULL) {
            MemoryNode* item = memory->buckets[i];
            while (item != NULL) {
                sum += item->value;
                item = item->next;
            }
        }
    }
    return sum;
}
