#include "helper.h"
#include "uthash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define INPUT_FILE_PATH "./res/demo_input.txt"
#define INPUT_FILE_PATH "./res/puzzle_input.txt"

typedef struct
{
    int key;
    int value;
    UT_hash_handle hh;
} IntToIntMap;

// Function to add a key-value pair to the hash table
void add_entry(IntToIntMap **map, int key, int value)
{
    IntToIntMap *entry = NULL;

    // Check if key already exists
    HASH_FIND_INT(*map, &key, entry);
    if (entry == NULL)
    {
        // Key not found, create a new entry
        entry = (IntToIntMap *)malloc(sizeof(IntToIntMap));
        entry->key = key;
        entry->value = value;
        HASH_ADD_INT(*map, key, entry); // Add the entry to the hash table
    }
    else
    {
        // Key found, update value
        entry->value = value;
    }
}

// Function to find a value by key
IntToIntMap *find_entry(IntToIntMap *map, int key)
{
    IntToIntMap *entry = NULL;
    HASH_FIND_INT(map, &key, entry);
    return entry;
}

// Function to delete all entries and free memory
void delete_all(IntToIntMap **map)
{
    IntToIntMap *entry, *tmp;
    HASH_ITER(hh, *map, entry, tmp)
    {
        HASH_DEL(*map, entry);
        free(entry);
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
            perror("An error occured while parsing a line");
            return 1;
        }
        (*left_column)[i] = left;
        (*right_column)[i] = right;
        // printf("Parsed at line=%d left=%d right=%d\n", i, left, right);
    }

    return 0;
}

void count_column(const int *column, const int line_count, IntToIntMap **counter)
{
    int number;
    int number_count;
    IntToIntMap *entry = NULL;
    for (int i = 0; i < line_count; i++)
    {
        number = column[i];
        entry = find_entry(*counter, number);
        if (entry == NULL)
        {
            number_count = 1;
        }
        else
        {
            number_count = entry->value + 1;
        }

        entry = NULL;
        add_entry(counter, number, number_count);
        // printf("number=%d count=%d\n", number, number_count);
    }
}

int compute_score(const int *left_col, const int line_count, IntToIntMap **counter)
{
    int left_value;
    int total = 0;
    IntToIntMap *entry = NULL;
    for (int i = 0; i < line_count; i++)
    {
        left_value = left_col[i];
        entry = find_entry(*counter, left_value);
        if (entry != NULL)
            total += left_value * entry->value;
    }
    return total;
}

int main()
{
    IntToIntMap *number_counter = NULL;
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

    count_column((const int *)right_col, line_count, &number_counter);
    score = compute_score((const int *)left_col, line_count, &number_counter);

    printf("%d\n", score);

cleanup:
    // Free allocated memory
    if (number_counter != NULL)
        delete_all(&number_counter);
    if (left_col != NULL)
        free(left_col);
    if (right_col != NULL)
        free(right_col);
    if (lines != NULL)
        free_read_lines(lines, line_count);
    return ret;
}
