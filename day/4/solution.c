#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <regex.h>
#include <assert.h>

#define NUMBER_OF_PASSPORTS 257

typedef struct passport {
    int byr; // Birth Year
    int iyr; // Issue Year
    int eyr; // Expiration Year
    char* hgt; // Height
    char* hcl; // Hair Color
    char* ecl; // Eye Color
    char* pid; // Passport ID
    int cid; // Country ID
} passport;

bool is_required_fields_present(passport p) {
    return p.byr != 0 && p.iyr != 0 &&
        p.eyr != 0 && p.hgt != NULL &&
        p.hcl != NULL && p.ecl != NULL &&
        p.pid != NULL;
}

bool rematch(char* s, char* re) {
    regex_t regex;
    assert(!regcomp(&regex, re, REG_EXTENDED));
    bool res = regexec(&regex, s, 0, NULL, 0) != REG_NOMATCH;
    regfree(&regex);
    return res;
}

bool is_valid_hair_color(char* s) {
    return rematch(s, "^#[0-9a-z]{6}$");
}

bool is_valid_eye_color(char* s) {
    return rematch(s, "^amb|blu|brn|gry|grn|hzl|oth$");
}

bool is_valid_pid(char* s) {
    return rematch(s, "^[0-9]{9}$");
}

bool streq(char* s1, char* s2) {
    return strcmp(s1, s2) == 0;
}

bool is_valid_height(char* s) {
    char* unit;
    int height = strtol(s, &unit, 10);
    if (streq(unit, "cm")) {
        return height >= 150 && height <= 193;
    } else if (streq(unit, "in")) {
        return height >= 59 && height <= 76;
    } else {
        return false;
    }
}

bool is_valid_passport(passport p) {
    return p.byr >= 1920 && p.byr <= 2002 &&
        p.iyr >= 2010 && p.iyr <= 2020 &&
        p.eyr >= 2020 && p.eyr <= 2030 &&
        is_valid_height(p.hgt) &&
        is_valid_hair_color(p.hcl) &&
        is_valid_eye_color(p.ecl) &&
        is_valid_pid(p.pid);
}

int parse_int(char* s) {
    return (int) strtol(s, NULL, 10);
}

void parse_passports(FILE* file, passport* passports) {
    char* line = NULL;
    size_t len = 0;
    int n = 0;
    int read = 0;

    while (read != EOF) {
        passport p = {};
        char* key;
        char* value;
        while((read = getline(&line, &len, file)) && read != EOF && read != 1) {
            key = strtok(line, ":");
            while (key != NULL) {
                value = strtok(NULL, " \n");

                if (streq(key, "byr")) p.byr = parse_int(value);
                else if (streq(key, "eyr")) p.eyr = parse_int(value);
                else if (streq(key, "iyr")) p.iyr = parse_int(value);
                else if (streq(key, "hgt")) p.hgt = strdup(value);
                else if (streq(key, "hcl")) p.hcl = strdup(value);
                else if (streq(key, "ecl")) p.ecl = strdup(value);
                else if (streq(key, "pid")) p.pid = strdup(value);
                else if (streq(key, "cid")) p.cid = parse_int(value);

                key = strtok(NULL, ":");
            }
        }
        passports[n] = p;
        n++;
    }
    free(line);
}

int main(int argc, char** argv) {
    FILE* input = fopen(argv[1], "r");
    passport passports[NUMBER_OF_PASSPORTS];
    parse_passports(input, passports);
    fclose(input);

    int required_passport_count = 0;
    int valid_passport_count = 0;

    for (int i = 0; i < NUMBER_OF_PASSPORTS; i++) {
        if (is_required_fields_present(passports[i])) {
            required_passport_count++;
            if (is_valid_passport(passports[i])) {
                valid_passport_count++;
            }
        }
    }

    printf("Solution 1: %d\n", required_passport_count);
    printf("Solution 2: %d\n", valid_passport_count);
}
