#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define NUMBER_OF_RULES 594

typedef struct rule_item {
    char* bag_color;
    int count;
} rule_item;

typedef struct rule {
    char* bag_color;
    int rule_items_count;
    rule_item* rule_items;
} rule;

size_t count_rule_items(char* rule_rhs) {
    if (strcmp(rule_rhs, "no other bags") == 0) {
        return 0;
    }

    size_t sep_count = 0;
    for (int i = 0; i < strlen(rule_rhs); i++) {
        if (rule_rhs[i] == ',') sep_count++;
    }
    return sep_count + 1;
}

void parse_rule_items(char* rule_rhs, rule_item** rule_items, size_t rule_items_count) {
    regex_t rhs_regex;
    size_t nmatch = 3;
    regmatch_t rhs_match[nmatch];

    int res;
    res = regcomp(&rhs_regex, "([0-9]+) (.+?) bag", REG_EXTENDED|REG_ENHANCED);
    if (res != 0) {
        char error_message[256];
        regerror(res, &rhs_regex, error_message, 256);
        fprintf(stderr, "%s", error_message);
        exit(EXIT_FAILURE);
    }

    int offset = 0;
    for(int i = 0; i < rule_items_count; i++) {
        res = regexec(&rhs_regex, rule_rhs + offset, nmatch, rhs_match, 0);

        if (res == REG_NOMATCH) {
            return;
        }
        if (res != 0) {
            char error_message[256];
            regerror(res, &rhs_regex, error_message, 256);
            fprintf(stderr, "%s", error_message);
            exit(EXIT_FAILURE);
        }

        assert(rhs_match[1].rm_so != -1 && rhs_match[1].rm_eo != -1);
        assert(rhs_match[2].rm_so != -1 && rhs_match[2].rm_eo != -1);

        int bag_count_start = rhs_match[1].rm_so;
        int bag_count_len = rhs_match[1].rm_eo - rhs_match[1].rm_so;
        char* bag_count = strndup(rule_rhs + bag_count_start + offset, bag_count_len);

        int bag_color_start = rhs_match[2].rm_so;
        int bag_color_len = rhs_match[2].rm_eo - rhs_match[2].rm_so;
        char* bag_color = strndup(rule_rhs + bag_color_start + offset, bag_color_len);

        rule_item* rb = (rule_item*) malloc(sizeof(rule_item));

        rb->bag_color = bag_color;
        rb->count = strtol(bag_count, NULL, 10);

        (*rule_items)[i] = *rb;
        offset += rhs_match[2].rm_eo;
    }
}

void parse_rule(char* line, rule* rule) {
    regex_t rule_regex;
    size_t nmatch = 3;
    regmatch_t rule_match[nmatch];
    assert(!regcomp(&rule_regex, "^(.+) bags contain (.+)\\.", REG_EXTENDED));
    int res = regexec(&rule_regex, line, nmatch, rule_match, 0);

    assert(res == 0);
    assert(rule_match[1].rm_so != -1 && rule_match[1].rm_eo != 1);
    assert(rule_match[2].rm_so != -1 && rule_match[2].rm_eo != 1);

    int bag_start = rule_match[1].rm_so;
    int bag_len = rule_match[1].rm_eo - rule_match[1].rm_so;
    char* rule_lhs = strndup(line + bag_start, bag_len);


    int rhs_start = rule_match[2].rm_so;
    int rhs_len = rule_match[2].rm_eo - rule_match[2].rm_so;
    char* rule_rhs = strndup(line+rhs_start, rhs_len);

    regfree(&rule_regex);

    size_t rule_items_count = count_rule_items(rule_rhs);
    rule_item* rule_items = (rule_item*) malloc(sizeof(rule_item) * rule_items_count);

    parse_rule_items(rule_rhs, &rule_items, rule_items_count);

    rule->bag_color = rule_lhs;
    rule->rule_items_count = rule_items_count;
    rule->rule_items = rule_items;
}

// TODO bsearch?
rule find_rule(rule* rs, char* bag_color) {
    for (int i = 0; i < NUMBER_OF_RULES; i++) {
        if (strcmp(rs[i].bag_color, bag_color) == 0) {
            return rs[i];
        }
    }
    assert(0);
}

bool is_reachable_from(char* bag_color, rule r, rule* rs) {
    for (int i = 0; i < r.rule_items_count; i++) {
        rule_item rule_item = r.rule_items[i];
        if (strcmp(rule_item.bag_color, bag_color) == 0 ||
            is_reachable_from(bag_color, find_rule(rs, rule_item.bag_color), rs)) {
            return true;
        }
    }
    return false;
}

int count_outmost_bags(rule_item ri, rule* rules) {
    int count = 0;
    for (int i = 0; i < NUMBER_OF_RULES; i++) {
        if (is_reachable_from(ri.bag_color, rules[i], rules)) {
            count++;
        }
    }
    return count;
}

int count_inner_bags(rule_item ri, rule* rs) {
    rule rule = find_rule(rs, ri.bag_color);

    if (rule.rule_items_count == 0) {
        return 0;
    }

    int sum = 0;
    for (int i = 0; i < rule.rule_items_count; i++) {
        rule_item rule_item = rule.rule_items[i];
        sum += count_inner_bags(rule_item, rs) * rule_item.count + rule_item.count;
    }

    return sum;
}

int main(int argc, char** argv) {
    FILE* input = fopen(argv[1], "r");

    rule rules[NUMBER_OF_RULES];

    char* line = NULL;
    size_t len = 0;
    int read = 0;

    for (int i = 0; i < NUMBER_OF_RULES; i++) {
        read = getline(&line, &len, input);
        rule* r = (rule*) malloc(sizeof(rule));
        parse_rule(line, r);
        rules[i] = *r;
    }

    rule_item shiny_gold = { "shiny gold" };
    printf("Solution 1: %5d\n", count_outmost_bags(shiny_gold, rules));
    printf("Solution 2: %5d\n", count_inner_bags(shiny_gold, rules));
    fclose(input);
}
