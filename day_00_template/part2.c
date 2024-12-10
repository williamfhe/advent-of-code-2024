#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE_PATH "./res/demo_input.txt"
// #define INPUT_FILE_PATH "./res/puzzle_input.txt"

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
   
    printf("Read %d lines\n", line_count);

cleanup:
    if (lines != NULL)
        free_read_lines(lines, line_count);
    
    return ret;
}
