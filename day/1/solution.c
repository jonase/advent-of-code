#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUMBERS_COUNT 200

// O(n)
bool find_sum_indices(int* numbers, int* left, int* right, int target) {
    while(*left < *right) {
        int sum = numbers[*left] + numbers[*right];
        if (sum > target) (*right)--;
        else if (sum < target) (*left)++;
        else return true;
    }
    return false;
}

int solution_1(int* numbers) {
    int left = 0, right = NUMBERS_COUNT - 1;
    if(find_sum_indices(numbers, &left, &right, 2020)) {
        return numbers[left] * numbers[right];
    } else return -1;
}

// O(n^2)
int solution_2(int* numbers) {
    for(int i = 0; i < NUMBERS_COUNT - 2; i++) {
        int x = numbers[i];
        int target = 2020 - x, left = i + 1, right = NUMBERS_COUNT - 1;
        if (find_sum_indices(numbers, &left, &right, target)) {
            return x * numbers[left] * numbers[right];
        }
    }
    return -1;
}

void read_numbers(FILE* file, int* numbers) {
    char *line = NULL;
    size_t len = 0;
    int i = 0;
    while (getline(&line, &len, file) != EOF) {
        int number = (int) strtol(line, NULL, 10);
        numbers[i++] = number;
    }
    free(line);
}

int int_cmp(const void* a, const void* b) {
   return *(int*)a - *(int*)b;
}

int main(int argc, char** argv) {
    FILE* input = fopen(argv[1], "r");
    int numbers[NUMBERS_COUNT];

    read_numbers(input, numbers);
    fclose(input);

    qsort(numbers, NUMBERS_COUNT, sizeof(int), int_cmp);

    printf("Solution 1: %9d\n", solution_1(numbers));
    printf("Solution 2: %9d\n", solution_2(numbers));

    return 0;
}
