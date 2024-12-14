#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>

// #define INPUT_FILE_PATH "./res/demo_input.txt"
#define INPUT_FILE_PATH "./res/puzzle_input.txt"

#define MUL_REGEX_PATTERN "mul\\(([[:digit:]]+),([[:digit:]]+)\\)|do\\(\\)|don't\\(\\)"

bool read_number_from_regex(const char *program, const regmatch_t match, const int offset, int *number)
{
    if (match.rm_so == -1 || match.rm_eo == -1)
    {
        fprintf(stderr, "Invalid regex match bounds\n");
        return false;
    }

    int size = match.rm_eo - match.rm_so;
    if (size <= 0)
    {
        fprintf(stderr, "Invalid match size\n");
        return false;
    }

    char number_str[size + 1];
    strncpy(number_str, program + offset + match.rm_so, size);
    number_str[size] = '\0';

    // printf("Got number_str=%s\n", number_str);
    if (sscanf(number_str, "%d", number) != 1)
    {
        fprintf(stderr, "Could not parse number: %s\n", number_str);
        return false;
    }

    return true;
}

int compute_program(const char *program, bool * do_mul)
{
    regex_t regex;
    if (regcomp(&regex, MUL_REGEX_PATTERN, REG_EXTENDED) != 0)
    {
        fprintf(stderr, "Failed to compile regex\n");
        return -1;
    }

    int match_count = regex.re_nsub + 1;
    regmatch_t matches[match_count];
    int total = 0;
    int first_number, second_number;
    int offset = 0;

    while (regexec(&regex, program + offset, match_count, matches, 0) == 0)
    {
        int found_size = matches[0].rm_eo - matches[0].rm_so;
        char found_match[found_size + 1];
        strncpy(found_match, program + offset + matches[0].rm_so, found_size);
        found_match[found_size] = '\0';

        // printf("Found: '%s'\n", found_match);

        if (strcmp(found_match, "do()") == 0)
        {
            *do_mul = true;
            offset += matches[0].rm_eo;
            continue;
        }
        else if (strcmp(found_match, "don't()") == 0)
        {
            *do_mul = false;
            offset += matches[0].rm_eo;
            continue;
        }

        if (!*do_mul)
        {
            offset += matches[0].rm_eo;
            continue;
        }

        if (!read_number_from_regex(program, matches[1], offset, &first_number))
        {
            regfree(&regex);
            return -1;
        }
        if (!read_number_from_regex(program, matches[2], offset, &second_number))
        {
            regfree(&regex);
            return -1;
        }

        total += first_number * second_number;
        // printf("mul(%d, %d) = %d\n", first_number, second_number, first_number * second_number);

        offset += matches[0].rm_eo;
    }

    regfree(&regex);
    return total;
}

int main()
{
    int ret = 0;
    int line_count;
    char **lines = NULL;
    char *input_line = NULL;

    lines = read_all_lines(INPUT_FILE_PATH, &line_count);
    if (lines == NULL)
    {
        fprintf(stderr, "Could not read all lines");
        ret = 1;
        goto cleanup;
    }

    int total = 0;
    bool do_mul = true;
    for (int i = 0; i < line_count; i++)
    {
        // TODO: handle if return value is -1
        total += compute_program((const char *)lines[i], &do_mul);
        // printf("Line=%d total=%d\n", i, total);
    }
    // printf("Read %d lines\n", line_count);
    // printf("Input '%s'\n", input_line);

    printf("%d\n", total);

cleanup:
    if (lines != NULL)
        free_read_lines(lines, line_count);

    return ret;
}
