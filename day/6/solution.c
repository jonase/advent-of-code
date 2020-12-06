#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int az_set;

az_set az_set_add(az_set set, char ch) {
    if (ch >= 'a' && ch <= 'z') {
        set = set | (1 << (ch - 'a'));
    }
    return set;
}

az_set az_set_init(char* s) {
    az_set set = 0;
    for (int i = 0; i < strlen(s); i++) {
        set = az_set_add(set, s[i]);
    }
    return set;
}

az_set az_set_intersection(az_set set1, az_set set2) {
    return set1 & set2;
}

az_set az_set_union(az_set set1, az_set set2) {
    return set1 | set2;
}

az_set az_set_add_all(az_set set, char* s) {
    return az_set_union(set, az_set_init(s));
}

int az_set_count(az_set set) {
    return __builtin_popcount(set);
}

int main(int argc, char** argv) {
    FILE* input = fopen(argv[1], "r");

    int any_total_count = 0;
    int every_total_count = 0;

    char* line = NULL;
    size_t len = 0;
    int read = 0;

    while (read != EOF) {
        read = getline(&line, &len, input);
        if (read == EOF) break;
        if (read == 1) continue;

        az_set any_yes_set = az_set_init(line);
        az_set every_yes_set = az_set_init(line);

        while((read = getline(&line, &len, input)) && read != EOF && read != 1) {
            any_yes_set = az_set_union(any_yes_set, az_set_init(line));
            every_yes_set = az_set_intersection(every_yes_set, az_set_init(line));
        }

        any_total_count += az_set_count(any_yes_set);
        every_total_count += az_set_count(every_yes_set);
    }
    free(line);
    fclose(input);

    printf("Solution 1: %d\n", any_total_count);
    printf("Solution 2: %d\n", every_total_count);
}
