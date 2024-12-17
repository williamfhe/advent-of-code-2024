#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **read_all_lines(const char *filename, int *line_count)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Failed to open file");
        return NULL;
    }

    char **lines = NULL;
    char *line = NULL;
    size_t len = 0;
    int read;
    *line_count = 0;

    while ((read = getline(&line, &len, file)) != -1)
    {
        // Remove newline character if present
        if (read > 0 && line[read - 1] == '\n')
        {
            line[read - 1] = '\0';
        }

        // Reallocate memory for the lines array
        char **temp = realloc(lines, (*line_count + 1) * sizeof(char *));
        if (temp == NULL)
        {
            perror("Memory allocation failed");
            free(lines);
            fclose(file);
            free(line);
            return NULL;
        }
        lines = temp;

        // Allocate memory for the new line and copy it
        lines[*line_count] = malloc((strlen(line) + 1) * sizeof(char));
        if (lines[*line_count] == NULL)
        {
            perror("Memory allocation failed");
            for (int i = 0; i < *line_count; i++)
            {
                free(lines[i]);
            }
            free(lines);
            fclose(file);
            free(line);
            return NULL;
        }
        strcpy(lines[*line_count], line);
        (*line_count)++;
    }

    free(line);
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