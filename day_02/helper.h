#ifndef AOC_HELPERS
#define AOC_HELPERS

#include <stddef.h>

char **read_all_lines(const char *filename, int *line_count);
void free_array_of_array(void** array, size_t count);

#endif // AOC_HELPERS