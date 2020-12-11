#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define ROWS 91
#define COLS 91

typedef struct Loc {
    int row;
    int col;
} Loc;

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
char get(char* grid, Loc loc) {
    int row = loc.row;
    int col = loc.col;
    if (row < 0 || row > ROWS - 1 || col < 0 || col > COLS - 1) {
        return '_';
    } else {
        return *(grid + row * COLS + col);
    }
}

void set(char* grid, Loc loc, char c) {
    int row = loc.row;
    int col = loc.col;
    if (row < 0 || row > ROWS - 1 || col < 0 || col > COLS - 1) {
        return;
    } else {
        *(grid + row * COLS + col) = c;
    }
}

Loc move(Loc loc, Dir dir) {
    int row = loc.row;
    int col = loc.col;
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

    Loc new_loc = { row, col };
    return new_loc;
}

char neighbour(char* grid, Loc loc, Dir dir) {
    return get(grid, move(loc, dir));
}

int occupied_count(char* grid, Loc loc) {
    return
        (neighbour(grid, loc, NORTH_WEST) == '#') +
        (neighbour(grid, loc, NORTH)      == '#') +
        (neighbour(grid, loc, NORTH_EAST) == '#') +
        (neighbour(grid, loc, EAST)       == '#') +
        (neighbour(grid, loc, SOUTH_EAST) == '#') +
        (neighbour(grid, loc, SOUTH)      == '#') +
        (neighbour(grid, loc, SOUTH_WEST) == '#') +
        (neighbour(grid, loc, WEST)       == '#');
}

char first_in_sight(char* grid, Loc loc, Dir dir) {
    char grid_item = get(grid, loc);
    if (grid_item == '_' || grid_item == 'L' || grid_item == '#') {
        return grid_item;
    } else {
        return first_in_sight(grid, move(loc, dir), dir);
    }

}

int occupied_in_sight_count(char* grid, Loc loc) {
    return
        (first_in_sight(grid, move(loc, NORTH_WEST), NORTH_WEST) == '#') +
        (first_in_sight(grid, move(loc, NORTH     ), NORTH)      == '#') +
        (first_in_sight(grid, move(loc, NORTH_EAST), NORTH_EAST) == '#') +
        (first_in_sight(grid, move(loc, EAST      ), EAST)       == '#') +
        (first_in_sight(grid, move(loc, SOUTH_EAST), SOUTH_EAST) == '#') +
        (first_in_sight(grid, move(loc, SOUTH     ), SOUTH)      == '#') +
        (first_in_sight(grid, move(loc, SOUTH_WEST), SOUTH_WEST) == '#') +
        (first_in_sight(grid, move(loc, WEST      ), WEST)       == '#');

}

void iterate_part_1(char* grid, char* next_grid) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            Loc loc = { r, c };
            int n = occupied_count(grid, loc);
            if (get(grid, loc) == 'L' && n == 0) set(next_grid, loc, '#');
            if (get(grid, loc) == '#' && n >= 4) set(next_grid, loc, 'L');
        }
    }
}

void iterate_part_2(char* grid, char* next_grid) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            Loc loc = { r, c };
            int n = occupied_in_sight_count(grid, loc);
            if (get(grid, loc) == 'L' && n == 0) set(next_grid, loc, '#');
            if (get(grid, loc) == '#' && n >= 5) set(next_grid, loc, 'L');
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
