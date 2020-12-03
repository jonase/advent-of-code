#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TERRAIN_HEIGHT 323
#define TERRAIN_WIDTH 31

int hit_count(char** terrain, int slope_x, int slope_y) {
    int pos_x = 0, pos_y = 0, tree_hits = 0;

    for (int pos_y = 0; pos_y < TERRAIN_HEIGHT; pos_y += slope_y, pos_x += slope_x) {
        if (terrain[pos_y][pos_x % TERRAIN_WIDTH] == '#'){
            tree_hits++;
        }
    }

    return tree_hits;
}

void read_terrain(char** terrain, FILE* file) {
    char* line = NULL;
    size_t len = 0;
    int row = 0;
    while(getline(&line, &len, file) != EOF) {
        terrain[row++] = strdup(line);
    }
    free(line);
}

int main(void) {
    char* terrain[TERRAIN_HEIGHT];

    read_terrain(terrain, stdin);

    int slope_1_1 = hit_count(terrain, 1, 1);
    int slope_3_1 = hit_count(terrain, 3, 1);
    int slope_5_1 = hit_count(terrain, 5, 1);
    int slope_7_1 = hit_count(terrain, 7, 1);
    int slope_1_2 = hit_count(terrain, 1, 2);

    printf("Solution 1: %10d\n", slope_3_1);
    printf("Solution 2: %10ld\n",
        (long) slope_1_1 * slope_3_1 * slope_5_1 * slope_7_1 * slope_1_2
    );
}
