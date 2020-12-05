#include <stdio.h>
#include <stdlib.h>

#define BOARDING_PASS_COUNT 789

int seat_id(char* boarding_pass) {
    int sid = 0;
    for (int i = 0; i < 10; i++) {
        char ch = boarding_pass[i];
        if (ch == 'B' || ch == 'R') sid++;
        sid <<= 1;
    }
    return sid >> 1;
}

int int_cmp(const void* a, const void* b) {
   return *(int*)a - *(int*)b;
}

int main(int argc, char** argv) {
    FILE* input = fopen(argv[1], "r");

    int max_sid = 0, n = 0;
    char boarding_pass[11];
    int seat_ids[BOARDING_PASS_COUNT];

    while(fscanf(input, "%s", boarding_pass) != EOF) {
        int sid = seat_id(boarding_pass);
        seat_ids[n++] = sid;
        if (sid > max_sid) {
            max_sid = sid;
        }
    }
    fclose(input);

    printf("Solution 1: %d\n", max_sid);

    qsort(seat_ids, BOARDING_PASS_COUNT, sizeof(int), int_cmp);

    for (int i = 0; i < BOARDING_PASS_COUNT - 1; i++) {
        int sid = seat_ids[i];
        if (sid + 1 != seat_ids[i + 1]) {
            printf("Solution 2: %d\n", sid + 1);
            break;
        }
    }
}
