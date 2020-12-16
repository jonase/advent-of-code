#include <stdlib.h>
#include <stdio.h>

int play(int* init_nums, int init_count, int iterations) {
    int* nums = (int*) malloc(sizeof(int) * iterations);

    for (int i = 0; i < iterations; i++) {
        nums[i] = -1;
    }

    for (int i = 0; i < init_count; i++) {
        nums[init_nums[i]] = i;
    }

    int last_spoken = init_nums[init_count-1];

    for (int i = init_count; i < iterations; i++) {
        int previously_spoken_at = nums[last_spoken];
        int next_to_speek;
        if (previously_spoken_at == -1) {
            next_to_speek = 0;
        } else {
            next_to_speek = i - previously_spoken_at - 1;
        }
        nums[last_spoken] = i - 1;
        last_spoken = next_to_speek;
    }

    free(nums);
    return last_spoken;
}

int main(int argc, char** argv) {
    const int init_count = 6;
    int init_nums[init_count] = { 1, 12, 0, 20, 8, 16 };
    printf("Solution 1: %d\n", play(init_nums, init_count, 2020));
    printf("Solution 2: %d\n", play(init_nums, init_count, 30000000));
}

