#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define SUM_WINDOW_SIZE 25
#define NUMBERS_COUNT 1000

long find_invalid_number(long* numbers);
long find_encryption_weakness(long* numbers, long invalid_number);

int main(int argc, char **argv) {
    FILE* input = fopen(argv[1], "r");

    long numbers[NUMBERS_COUNT];

    long n = 0;
    int i = 0;
    while(fscanf(input, "%ld", &n) != EOF) {
        numbers[i++] = n;
    }

    long invalid_number = find_invalid_number(numbers);
    printf("Solution 1: %ld\n", invalid_number);
    long encryption_weakness = find_encryption_weakness(numbers, invalid_number);
    printf("Solution 2: %ld\n", encryption_weakness);
}

long find_invalid_number(long* numbers) {
    for(int i = SUM_WINDOW_SIZE; i < NUMBERS_COUNT; i++) {
        bool found = false;
        for (int j = i - SUM_WINDOW_SIZE; j < i; j++) {
            for (int k = j+1; k < i; k++) {
                if (numbers[i] == numbers[j] + numbers[k]) {
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        if (!found) return numbers[i];
    }
    return -1;
}

long find_encryption_weakness(long* numbers, long invalid_number) {
    for (int i = 0; i < NUMBERS_COUNT; i++) {
        long sum = numbers[i];
        for (int j = i+1; j < NUMBERS_COUNT; j++) {
            sum += numbers[j];
            if (sum == invalid_number) {
                long min = LONG_MAX, max = LONG_MIN;
                for (int k = i; k <= j; k++) {
                    if (numbers[k] < min) min = numbers[k];
                    if (numbers[k] > max) max = numbers[k];
                }
                return min + max;
            } else if (sum > invalid_number) {
                break;
            }
        }
    }
    return -1;
}
