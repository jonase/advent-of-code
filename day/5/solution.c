#include <stdio.h>
#include <stdlib.h>

#define BOARDING_PASS_COUNT 789

int seat_row(char* boarding_pass) {
    int bottom = 0, top = 127, mid = top / 2;
    for (int i = 0; i < 7; i++) {
        if (boarding_pass[i] == 'F') top = mid;
        else bottom = mid + 1;
        mid = (top + bottom) / 2;
    }
    return mid;
}

int seat_column(char* boarding_pass) {
    int left = 0, right = 7, mid = right / 2;
    for (int i = 7; i < 10; i++) {
        if (boarding_pass[i] == 'L') right = mid;
        else left = mid + 1;
        mid = (right + left) / 2;
    }
    return mid;
}

int int_cmp(const void* a, const void* b) {
   return *(int*)a - *(int*)b;
}

int main(int argc, char** argv) {
    FILE* input = fopen(argv[1], "r");

    int max_seat_id = 0, n = 0;
    char boarding_pass[11];
    int seat_ids[BOARDING_PASS_COUNT];

    while(fscanf(input, "%s", boarding_pass) != EOF) {
        int row = seat_row(boarding_pass);
        int column = seat_column(boarding_pass);
        int seat_id = row * 8 + column;
        seat_ids[n++] = seat_id;
        if (seat_id > max_seat_id) {
            max_seat_id = seat_id;
        }
    }
    fclose(input);

    printf("Solution 1: %d\n", max_seat_id);

    qsort(seat_ids, BOARDING_PASS_COUNT, sizeof(int), int_cmp);

    for (int i = 0; i < BOARDING_PASS_COUNT - 1; i++) {
        int seat_id = seat_ids[i];
        if (seat_id + 1 != seat_ids[i + 1]) {
            printf("Solution 2: %d\n", seat_id + 1);
            break;
        }
    }
}
