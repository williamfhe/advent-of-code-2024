#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>

// #define INPUT_FILE_PATH "./res/demo_input.txt"
#define INPUT_FILE_PATH "./res/puzzle_input.txt"

#define MUL_REGEX_PATTERN "mul\\(([[:digit:]]+),([[:digit:]]+)\\)"

bool read_number_from_regex(const char *program, const regmatch_t match, const int offset, int *number)
{

    int size = match.rm_eo - match.rm_so;

    // printf("rm_eo=%d rm_so=%d\n", match.rm_eo, match.rm_so);
    char number_str[size + 1];

    strncpy(number_str, program + offset + match.rm_so, match.rm_eo - match.rm_so);
    number_str[size] = '\0';
    if (sscanf(number_str, "%d", number) != 1)
    {
        perror("Could not parse number");
        return false;
    }

    return true;
}

int compute_program(const char *program)
{

    regex_t regex;
    if (regcomp(&regex, MUL_REGEX_PATTERN, REG_EXTENDED) != 0)
    {
        fprintf(stderr, "Failed to compile regex\n");
        return 0;
    }

    int match_count = regex.re_nsub + 1;
    regmatch_t matches[match_count];

    int total = 0;
    int first_number, second_number;
    int offset = 0;
    while (regexec(&regex, program + offset, match_count, matches, 0) == 0)
    {
        if (!read_number_from_regex(program, (const regmatch_t)matches[1], offset, &first_number))
        {
            return -1;
        }
        if (!read_number_from_regex(program, (const regmatch_t)matches[2], offset, &second_number))
        {
            return -1;
        }

        offset += matches[2].rm_eo;
        total += first_number * second_number;
        // printf("First=%d Second=%d total=%d offset=%d\n", first_number, second_number, total, offset);
        // printf("mul(%d,%d)\n", first_number, second_number);
    }

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
        perror("Could not read all lines");
        ret = 1;
        goto cleanup;
    }

    int total = 0;
    for (int i = 0; i < line_count; i++)
    {
        total += compute_program((const char *)lines[i]);
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
