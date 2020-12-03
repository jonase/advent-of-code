#include <stdio.h>
#include <string.h>

int is_valid_part_1(char* password, char ch, int min, int max) {
    int count = 0;

    for (int i = 0; i < strlen(password); i++) {
        if (password[i] == ch) {
            count++;
        }
    }

    return count >= min && count <= max;
}

int is_valid_part_2(char* password, char ch, int pos1, int pos2) {
    char ch1 = password[pos1 - 1];
    char ch2 = password[pos2 - 1];
    return (ch == ch1) != (ch == ch2);
}

int main(void) {
    int valid_count_part_1 = 0;
    int valid_count_part_2 = 0;

    char password[32];
    char ch;
    int a, b;

    while(scanf("%d-%d %c: %s", &a, &b, &ch, password) != EOF) {
        if (is_valid_part_1(password, ch, a, b)) {
            valid_count_part_1++;
        }

        if (is_valid_part_2(password, ch, a, b)) {
            valid_count_part_2++;
        }
    }

    printf("Solution 1: %d\n", valid_count_part_1);
    printf("Solution 2: %d\n", valid_count_part_2);
}
