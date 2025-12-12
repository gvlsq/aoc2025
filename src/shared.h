#ifndef AOC_2025_SHARED_H
#define AOC_2025_SHARED_H

#include <assert.h>
#include <float.h> // FLT_*, DBL_*
#include <stdint.h>

#define array_count(a) (sizeof((a)) / sizeof((a)[0]))

typedef int8_t s8;

typedef uint8_t u8;
typedef uint64_t u64;

struct Character_Grid {
    int width;
    int height;
    s8 *data;
};

const char *read_open_text_file(FILE *file);

// Character grid
void character_grid_init(Character_Grid *grid, FILE *file, int grid_width, int grid_height);

void character_grid_print(Character_Grid *grid);

void character_grid_set_char(Character_Grid *grid, int x, int y, char c);
char character_grid_get_char(Character_Grid *grid, int x, int y, int xoffset = 0, int yoffset = 0);

#endif
