#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// #define INPUT_FILE_PATH "./res/demo_input.txt"
#define INPUT_FILE_PATH "./res/puzzle_input.txt"
#define XMAS_WORD "XMAS"

bool has_xmas_in_direction(const char **lines, const int width, const int height, const int x, const int y, const int x_direction, const int y_direction)
{
    const int xmas_len = strlen(XMAS_WORD);
    // direction will reach out of bounds
    if (y + y_direction * (xmas_len - 1) < 0 || y + y_direction * (xmas_len - 1) >= height || x + x_direction * (xmas_len - 1) < 0 || x + x_direction * (xmas_len - 1) >= width)
        return false;

    for (int i = 0; i < xmas_len; i++)
    {
        if (lines[y + i * y_direction][x + i * x_direction] != XMAS_WORD[i])
            return false;
    }

    return true;
}

int count_xmas_starting_at(const char **lines, const int width, const int height, const int x, const int y)
{
    int xmas_count = 0;
    int x_direction, y_direction;

    // check all directions
    for (int i = 0; i < 9; i++)
    {
        if (i == 4)
            continue;

        // offset of direction
        y_direction = (i / 3) - 1;
        x_direction = (i % 3) - 1;

        if (has_xmas_in_direction(lines, width, height, x, y, (const int)x_direction, (const int)y_direction))
            xmas_count++;
    }
    return xmas_count;
}

int count_xmas(const char **lines, const int width, const int height)
{
    int total = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (lines[y][x] != 'X')
                continue;
            // printf("Start total=%d y=%d x=%d char=%d\n", total, y, x, lines[y][x]);
            total += count_xmas_starting_at(lines, width, height, x, y);
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
