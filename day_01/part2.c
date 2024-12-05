#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
// #define INPUT_FILE_PATH "./res/demo_input.txt"
#define INPUT_FILE_PATH "./res/puzzle_input.txt"

char **read_all_lines(const char *filename, int *line_count)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Failed to open file");
        return NULL;
    }

    char **lines = NULL;
    char buffer[MAX_LINE_LENGTH];
    *line_count = 0;

    while (fgets(buffer, MAX_LINE_LENGTH, file))
    {
        // Remove newline character if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
        }

        // Reallocate memory for the lines array
        char **temp = realloc(lines, (*line_count + 1) * sizeof(char *));
        if (temp == NULL)
        {
            perror("Memory allocation failed");
            free(lines);
            fclose(file);
            return NULL;
        }
        lines = temp;

        // Allocate memory for the new line and copy it
        lines[*line_count] = malloc((strlen(buffer) + 1) * sizeof(char));
        if (lines[*line_count] == NULL)
        {
            perror("Memory allocation failed");
            for (int i = 0; i < *line_count; i++)
            {
                free(lines[i]);
            }
            free(lines);
            fclose(file);
            return NULL;
        }
        strcpy(lines[*line_count], buffer);
        (*line_count)++;
    }

    fclose(file);
    return lines;
}

void free_read_lines(char **lines, const int line_count)
{
    for (int i = 0; i < line_count; i++)
    {
        free(lines[i]);
    }
    free(lines);
}

void dummy_bubblesort(int *numbers, int length)
{
    if (length <= 1)
    {
        return;
    }

    int temp;
    for (int i = 0; i < length - 1; i++)
    {
        for (int j = 0; j < length - i - 1; j++)
        {
            if (numbers[j] > numbers[j + 1])
            {
                temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }
}

int parse_columns(const char **lines, const int line_count, int **left_column, int **right_column)
{
    int left, right;

    int err;
    for (int i = 0; i < line_count; i++)
    {
        const char *line = lines[i];
        if (sscanf(line, "%d %d", &left, &right) != 2)
        {
            // TODO: add line here
            perror("An error occured while parsing a line");
            return 1;
        }
        (*left_column)[i] = left;
        (*right_column)[i] = right;
        // printf("Parsed at line=%d left=%d right=%d\n", i, left, right);
    }

    return 0;
}

void display_columns(const int **left_col, const int **right_col, const int line_count)
{
    int left, right;
    for (int i = 0; i < line_count; i++)
    {
        left = (*left_col)[i];
        right = (*right_col)[i];
        printf("Line=%d left=%d right=%d\n", i, left, right);
    }
}

int calculate_score(const int *left_col, const int *right_col, const int line_count)
{
    int total = 0;
    int diff;
    for (int i = 0; i < line_count; i++)
    {
        diff = left_col[i] - right_col[i];
        if (diff < 0)
        {
            diff = -diff;
        }
        total += diff;
        // printf("Calculated at line=%d diff=%d total=%d\n", i, diff, total);
    }
    return total;
}

int main()
{
    int ret = 0;
    int line_count;
    int score;
    char **lines = NULL;
    int *left_col = NULL;
    int *right_col = NULL;

    lines = read_all_lines(INPUT_FILE_PATH, &line_count);
    if (lines == NULL)
    {
        perror("Could not read all lines");
        ret = 1;
        goto cleanup;
    }
    // printf("Read %d lines\n", line_count);

    left_col = (int *)malloc(line_count * sizeof(int));
    if (left_col == NULL)
    {
        perror("Could not allocate memory for left column");
        ret = 1;
        goto cleanup;
    }

    right_col = (int *)malloc(line_count * sizeof(int));
    if (right_col == NULL)
    {
        perror("Could not allocate memory for left column");
        ret = 1;
        goto cleanup;
    }

    if (parse_columns((const char **)lines, line_count, &left_col, &right_col) != 0)
    {
        perror("An error occured while parsing columns");
        ret = 1;
        goto cleanup;
    }

    // sort all values in ascending order
    dummy_bubblesort(left_col, line_count);
    dummy_bubblesort(right_col, line_count);

    score = calculate_score((const int *)left_col, (const int *)right_col, line_count);

    printf("%d\n", score);

cleanup:
    // Free allocated memory
    if (left_col != NULL)
        free(left_col);
    if (right_col != NULL)
        free(right_col);
    if (lines != NULL)
        free_read_lines(lines, line_count);
    return ret;
}