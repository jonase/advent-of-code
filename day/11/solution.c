#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define ROWS 91
#define COLS 91

typedef struct Position {
    int row;
    int col;
} Position;

typedef enum Dir {
    NORTH, SOUTH, EAST, WEST,
    NORTH_WEST, NORTH_EAST,
    SOUTH_WEST, SOUTH_EAST
} Dir;

void gridcpy(char* dest, char* src) {
    memcpy(dest, src, ROWS * COLS);
}

bool grideq(char* grid1, char* grid2) {
    if (grid1 == grid2) return true;
    for (int i = 0; i < ROWS*COLS; i++) {
        if (grid1[i] != grid2[i]) return false;
    }
    return true;
}

bool out_of_bounds(Position pos) {
    return pos.row < 0 || pos.row > ROWS - 1 || pos.col < 0 || pos.col > COLS - 1;
}

char get(char* grid, Position pos) {
    if (out_of_bounds(pos)) {
        return '_';
    } else {
        return *(grid + pos.row * COLS + pos.col);
    }
}

void set(char* grid, Position pos, char c) {
    if (!out_of_bounds(pos)) {
        *(grid + pos.row * COLS + pos.col) = c;
    }
}

Position move(Position pos, Dir dir) {
    int row = pos.row;
    int col = pos.col;
    switch (dir) {
        case NORTH: row--; break;
        case SOUTH: row++; break;
        case EAST: col++; break;
        case WEST: col--; break;
        case NORTH_EAST: row--; col++; break;
        case NORTH_WEST: row--; col--; break;
        case SOUTH_EAST: row++; col++; break;
        case SOUTH_WEST: row++; col--; break;
        default: assert(0);
    }

    Position new_pos = { row, col };
    return new_pos;
}

char neighbour(char* grid, Position pos, Dir dir) {
    return get(grid, move(pos, dir));
}

int occupied_count(char* grid, Position pos) {
    return
        (neighbour(grid, pos, NORTH_WEST) == '#') +
        (neighbour(grid, pos, NORTH)      == '#') +
        (neighbour(grid, pos, NORTH_EAST) == '#') +
        (neighbour(grid, pos, EAST)       == '#') +
        (neighbour(grid, pos, SOUTH_EAST) == '#') +
        (neighbour(grid, pos, SOUTH)      == '#') +
        (neighbour(grid, pos, SOUTH_WEST) == '#') +
        (neighbour(grid, pos, WEST)       == '#');
}

char first_in_sight(char* grid, Position pos, Dir dir) {
    char grid_item = get(grid, pos);
    if (grid_item == '_' || grid_item == 'L' || grid_item == '#') {
        return grid_item;
    } else {
        return first_in_sight(grid, move(pos, dir), dir);
    }

}

int occupied_in_sight_count(char* grid, Position pos) {
    return
        (first_in_sight(grid, move(pos, NORTH_WEST), NORTH_WEST) == '#') +
        (first_in_sight(grid, move(pos, NORTH     ), NORTH)      == '#') +
        (first_in_sight(grid, move(pos, NORTH_EAST), NORTH_EAST) == '#') +
        (first_in_sight(grid, move(pos, EAST      ), EAST)       == '#') +
        (first_in_sight(grid, move(pos, SOUTH_EAST), SOUTH_EAST) == '#') +
        (first_in_sight(grid, move(pos, SOUTH     ), SOUTH)      == '#') +
        (first_in_sight(grid, move(pos, SOUTH_WEST), SOUTH_WEST) == '#') +
        (first_in_sight(grid, move(pos, WEST      ), WEST)       == '#');

}

void iterate_part_1(char* grid, char* next_grid) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            Position pos = { r, c };
            int n = occupied_count(grid, pos);
            if (get(grid, pos) == 'L' && n == 0) set(next_grid, pos, '#');
            if (get(grid, pos) == '#' && n >= 4) set(next_grid, pos, 'L');
        }
    }
}

void iterate_part_2(char* grid, char* next_grid) {
    for(int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            Position pos = { r, c };
            int n = occupied_in_sight_count(grid, pos);
            if (get(grid, pos) == 'L' && n == 0) set(next_grid, pos, '#');
            if (get(grid, pos) == '#' && n >= 5) set(next_grid, pos, 'L');
        }
    }
}

int count_occupied_seats(char* grid) {
    int count = 0;
    for (int i = 0; i < ROWS*COLS; i++) {
        if (grid[i] == '#') count++;
    }
    return count;
}

int main(int argc, char** argv) {
    FILE* input = fopen(argv[1], "r");

    char* og_grid = malloc(sizeof(char) * ROWS * COLS);
    char* grid = malloc(sizeof(char) * ROWS * COLS);
    char* next_grid = malloc(sizeof(char) * ROWS * COLS);

    size_t len = 0;
    char* line = NULL;
    int row = 0;
    while(getline(&line, &len, input) != EOF) {
        memcpy(grid + (row++) * COLS, line, COLS);

    };
    free(line);
    fclose(input);

    gridcpy(og_grid, grid);
    gridcpy(next_grid, grid);

    while (true) {
        iterate_part_1(grid, next_grid);
        if (grideq(next_grid, grid)) break;
        gridcpy(grid, next_grid);
    }
    printf("Solution 1: %d\n", count_occupied_seats(next_grid));

    gridcpy(grid, og_grid);
    gridcpy(next_grid, og_grid);

    while (true) {
        iterate_part_2(grid, next_grid);
        if (grideq(next_grid, grid)) break;
        gridcpy(grid, next_grid);
    }
    printf("Solution 2: %d\n", count_occupied_seats(next_grid));
 }
