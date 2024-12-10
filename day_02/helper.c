#include "helper.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINE_LENGTH 1024

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

void free_array_of_array(void** array, size_t count) {
    if (array == NULL) {
        return;
    }

    for (size_t i = 0; i < count; i++) {
        if (array[i] != NULL) {
            free(array[i]);
            array[i] = NULL;
        }
    }
    free(array);
}