#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// #define INPUT_FILE_PATH "./res/demo_input.txt"
#define INPUT_FILE_PATH "./res/puzzle_input.txt"
#define XMAS_WORD "XMAS"

bool check_xmas_at(const char **lines, const int width, const int height, const int x, const int y)
{

    // check if out of border
    if (x + 2 >= width || y + 2 >= height)
        return false;

    // check middle char
    if (lines[y + 1][x + 1] != 'A')
        return false;


    // check top left to bottom right for SAM or MAS
    if (lines[y][x] == 'S')
    {
        // check SAM
        if (lines[y + 2][x + 2] != 'M')
            return false;
    } else if (lines[y][x] == 'M') {
        // check MAS
        if (lines[y + 2][x + 2] != 'S')
            return false;
    } else {
        return false;
    }

    // check diag top right to bottom left for SAM
    if (lines[y][x + 2] == 'S' && lines[y + 2][x] == 'M')
        return true;

    // check diag top right to bottom left for MAS
    if (lines[y][x + 2] == 'M' && lines[y + 2][x] == 'S')
        return true;

    return false;
}

int count_xmas(const char **lines, const int width, const int height)
{
    int total = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (lines[y][x] != 'S' && lines[y][x] != 'M')
                continue;
            // printf("Start total=%d y=%d x=%d char=%d\n", total, y, x, lines[y][x]);
            if (check_xmas_at(lines, width, height, x, y))
                total += 1;
            // printf("End total=%d y=%d x=%d char=%d\n", total, y, x, lines[y][x]);
        }
    }

    return total;
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

    if (line_count == 0)
    {
        fprintf(stderr, "Error, read line count is 0");
        ret = 1;
        goto cleanup;
    }

    // printf("Read %d lines\n", line_count);

    int width = strlen(lines[0]);
    if (width == 0)
    {
        fprintf(stderr, "Got a width of 0");
    }

    int total = count_xmas((const char **)lines, (const int)width, (const int)line_count);

    printf("%d\n", total);

cleanup:
    if (lines != NULL)
        free_read_lines(lines, line_count);

    return ret;
}
