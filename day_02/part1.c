#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

// #define INPUT_FILE_PATH "./res/demo_input.txt"
#define INPUT_FILE_PATH "./res/puzzle_input.txt"

#define DEFAULT_LINE_INT_ARRAY_CAPACITY 10

typedef struct
{
    int *values;
    int count;
} ParsedLine;

void free_parsed_line(ParsedLine *parsed_line)
{
    if (parsed_line == NULL || parsed_line->values == NULL)
    {
        return;
    }
    free(parsed_line->values);
    parsed_line->values = NULL;
    parsed_line->count = 0;
}

ParsedLine parse_line(const char *line)
{
    ParsedLine parsed_line = {NULL, 0};
    parsed_line.values = malloc(DEFAULT_LINE_INT_ARRAY_CAPACITY * sizeof(int));
    if (parsed_line.values == NULL)
    {
        perror("Could not allocate memory to store parsed values");
        return parsed_line;
    }

    int array_capacity = DEFAULT_LINE_INT_ARRAY_CAPACITY;
    int count = 0;

    char *line_copy = strdup(line);
    if (line_copy == NULL)
    {
        perror("Could not duplicate line");
        free(parsed_line.values);
        parsed_line.values = NULL;
        return parsed_line;
    }

    char *saveptr = NULL;
    char *token = strtok_r(line_copy, " ", &saveptr);
    while (token != NULL)
    {
        int parsed_number;
        if (sscanf(token, "%d", &parsed_number) != 1)
        {
            token = strtok_r(NULL, " ", &saveptr);
            continue;
        }

        if (count >= array_capacity)
        {
            array_capacity += DEFAULT_LINE_INT_ARRAY_CAPACITY;
            int *new_values = realloc(parsed_line.values, array_capacity * sizeof(int));
            if (new_values == NULL)
            {
                perror("Could not reallocate memory for parsed values");
                free(line_copy);
                free_parsed_line(&parsed_line);
                return parsed_line;
            }
            parsed_line.values = new_values;
        }

        parsed_line.values[count] = parsed_number;
        count++;
        token = strtok_r(NULL, " ", &saveptr);
    }

    free(line_copy);
    parsed_line.count = count;

    return parsed_line;
}

ParsedLine *parse_lines(const char **lines, int line_count)
{
    ParsedLine *parsed_lines = malloc(line_count * sizeof(ParsedLine));
    if (parsed_lines == NULL)
    {
        perror("Could not allocate memory for parsed lines");
        return NULL;
    }

    for (int i = 0; i < line_count; i++)
    {
        parsed_lines[i] = parse_line(lines[i]);
        if (parsed_lines[i].values == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free_parsed_line(&parsed_lines[j]);
            }
            free(parsed_lines);
            return NULL;
        }
    }

    return parsed_lines;
}

bool is_report_safe(const int *report, const int count)
{
    // check if values are strictly increasing

    if (report == NULL || count == 0)
    {
        perror("got a null report or count=0");
        return false;
    }

    // only one value
    if (count == 1)
    {
        return true;
    }

    bool decreasing = true;
    bool increasing = true;
    int diff;
    for (int i = 0; i < count - 1; i++)
    {
        diff = abs(report[i] - report[i+1]);
        if (!(diff >= 1 && diff <= 3)) {
            return false;
        }

        if (report[i] < report[i + 1])
        {
            decreasing = false;
        }

        if (report[i] > report[i + 1])
        {
            increasing = false;
        }

        // values are neither increasing nor decreasing
        if (!(decreasing || increasing))
        {
            return false;
        }
    }
    
    // return true if at least one is true
    return decreasing || increasing;
}

int main()
{
    int ret = 0;
    int line_count;
    char **lines = NULL;

    lines = read_all_lines(INPUT_FILE_PATH, &line_count);
    if (lines == NULL)
    {
        perror("Could not read all lines");
        ret = 1;
        goto cleanup;
    }

    ParsedLine *parsed_lines = NULL;
    parsed_lines = parse_lines((const char **)lines, line_count);
    if (parsed_lines == NULL)
    {
        perror("Could not parse lines");
        ret = 1;
        goto cleanup;
    }

    int total_safe = 0;
    for (int i = 0; i < line_count; i++)
    {
        ParsedLine parsed_line = parsed_lines[i];
        if (is_report_safe((const int *)parsed_line.values, (const int)parsed_line.count))
        {
            total_safe++;
        }
    }

    printf("%d\n", total_safe);

cleanup:
    if (lines != NULL)
    {
        free_array_of_array((void **)lines, line_count);
    }

    if (parsed_lines != NULL)
    {
        for (int i = 0; i < line_count; i++)
        {
            free_parsed_line(&parsed_lines[i]);
        }
        free(parsed_lines);
    }

    return ret;
}
