#include <stdlib.h>
#include <stdio.h>

#define ADAPTERS_COUNT (90 + 2)

void read_input(FILE*, int**);
int count_diff(int*, int);
long count_combinations(int*);

int main(int argc, char **argv) {
    FILE* input = fopen(argv[1], "r");

    int* numbers = malloc(sizeof(int) * ADAPTERS_COUNT);

    read_input(input, &numbers);

    printf("Solution 1: %d\n", count_diff(numbers, 1) * count_diff(numbers, 3));
    printf("Solution 2: %ld\n", count_combinations(numbers));

    free(numbers);
    fclose(input);
}

int int_cmp(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

void read_input(FILE* file, int** numbers) {
    (*numbers)[0] = 0;

    int n = 0, i = 1;
    while(fscanf(file, "%d", &n) != EOF) {
        (*numbers)[i++] = n;
    }

    qsort(*numbers, ADAPTERS_COUNT-1, sizeof(int), &int_cmp);

    n = (*numbers)[i-1];
    (*numbers)[i] = n + 3;
}

int count_diff (int* numbers, int n) {
    int count = 0;
    for (int i = 0; i < ADAPTERS_COUNT - 1; i++) {
        int a = numbers[i];
        int b = numbers[i+1];
        if (a + n == b) {
            count++;
        }
    }
    return count;
}

long count_combinations(int* numbers) {
    long prev_window[3] = { 1, 1, numbers[2] - numbers[0] <= 3 ? 2 : 1 };

    for (int i = 3; i < ADAPTERS_COUNT; i++) {
        long a = prev_window[0];
        long b = prev_window[1];
        long c = prev_window[2];
        long d = numbers[i];

        long n =
            (d - numbers[i - 3] <= 3 ? 1 : 0) * a +
            (d - numbers[i - 2] <= 3 ? 1 : 0) * b +
            (d - numbers[i - 1] <= 3 ? 1 : 0) * c;

        prev_window[0] = b;
        prev_window[1] = c;
        prev_window[2] = n;
    }
    return prev_window[2];
}
