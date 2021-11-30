#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct Instruction {
    char action;
    int value;
} Instruction;

typedef struct Ship {
    int x, y, dx, dy;
} Ship;

void navigate_p1(Ship* ship, Instruction instruction);
void navigate_p2(Ship* ship, Instruction instruction);
int manhattan_distance(Ship ship);

int main(int argc, char** argv) {
    FILE* input = fopen(argv[1], "r");

    Ship ship_p1 = { 0, 0, 1, 0 };
    Ship ship_p2 = { 0, 0, 10, 1 };

    char action; int value;
    while(fscanf(input, "%c%d\n", &action, &value) != EOF) {
        navigate_p1(&ship_p1, (Instruction) { action, value });
        navigate_p2(&ship_p2, (Instruction) { action, value });
    };
    fclose(input);

    printf("Solution 1: %d\n", manhattan_distance(ship_p1));
    printf("Solution 2: %d\n", manhattan_distance(ship_p2));
}

int manhattan_distance(Ship ship) {
    return abs(ship.x) + abs(ship.y);
}

int* rotation_matrix(int degree) {
    switch (degree) {
        case 90:  return (int[4]) {  0, -1,  1,  0 };
        case 180: return (int[4]) { -1,  0,  0, -1 };
        case 270: return (int[4]) {  0,  1, -1,  0 };
        default: assert(0);
    }
}

void rotate(Ship* ship, int* rot) {
    int dx = ship->dx;
    int dy = ship->dy;
    ship->dx = rot[0] * dx + rot[1] * dy;
    ship->dy = rot[2] * dx + rot[3] * dy;
}

void forward(Ship* ship, int value) {
    ship->x += ship->dx * value;
    ship->y += ship->dy * value;
}

void navigate_lrf(Ship* ship, Instruction instruction) {
    switch (instruction.action) {
        case 'L': rotate(ship, rotation_matrix(instruction.value)); break;
        case 'R': rotate(ship, rotation_matrix(360 - instruction.value)); break;
        case 'F': forward(ship, instruction.value); break;
        default: assert(0);
    }
}

void navigate_p1(Ship* ship, Instruction instruction) {
    switch (instruction.action) {
        case 'N': ship->y += instruction.value; break;
        case 'S': ship->y -= instruction.value; break;
        case 'E': ship->x += instruction.value; break;
        case 'W': ship->x -= instruction.value; break;
        default: navigate_lrf(ship, instruction); break;
    }
}

void navigate_p2(Ship* ship, Instruction instruction) {
    switch (instruction.action) {
        case 'N': ship->dy += instruction.value; break;
        case 'S': ship->dy -= instruction.value; break;
        case 'E': ship->dx += instruction.value; break;
        case 'W': ship->dx -= instruction.value; break;
        default: navigate_lrf(ship, instruction); break;
    }
}
