#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared.h"

#pragma warning(push, 1)
#pragma warning(disable: 6386)
const char *read_open_text_file(FILE *file) {
    fseek(file, 0, SEEK_END);
    int file_size = (int)ftell(file);
    fseek(file, 0, SEEK_SET);

    void *file_memory = malloc(file_size + 1);
    if (!file_memory) {
        fprintf(stderr, "Failed to allocate memory for text file\n");
        fclose(file);
        return 0;
    }

    fread(file_memory, file_size, 1, file);
    ((char *)file_memory)[file_size] = '\0';

    return (const char *)file_memory;
}
#pragma warning(pop)

#pragma region Character Grid

void character_grid_init(Character_Grid *grid, FILE *file, int grid_width, int grid_height) {
    grid->width  = grid_width;
    grid->height = grid_height;

    // Read characters into Character_Grid::data.
    size_t stride = sizeof(*grid->data)*grid->width;

    grid->data = (s8 *)malloc(stride*grid->height);

    if (file) {
        fseek(file, 0, SEEK_SET);

        u8 *dest = (u8 *)grid->data;

        char tmp[141 + 1];
        assert(sizeof(tmp) > grid_width);

        while (fscanf(file, "%s\n", tmp) == 1) {
            s8 *row = (s8 *)dest;
            for (int i = 0; i < grid->width; i++) {
                row[i] = tmp[i];
            }

            dest += stride;
        }
    } else {
        for (u64 y = 0; y < grid_height; y++) {
            for (u64 x = 0; x < grid_width; x++) {
                grid->data[grid_width*y + x] = '.';
            }
        }
    }
}

void character_grid_deinit(Character_Grid *grid) {
    free(grid->data);
}

void character_grid_print(Character_Grid *grid) {
    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            printf("%c", character_grid_get_char(grid, x, y));
        }
        printf("\n");
    }
}

bool character_grid_set_char(Character_Grid *grid,
                             int x, int y,
                             char c) {
    if (x < 0)            return false;
    if (x >= grid->width) return false;

    if (y < 0)             return false;
    if (y >= grid->height) return false;

    size_t stride = sizeof(*grid->data)*grid->width;

    *(s8 *)((u8 *)grid->data + y*stride + x) = c;

    return true;
}

char character_grid_get_char(Character_Grid *grid,
                             int x, int y,
                             int xoffset, int yoffset) {
    char result = '.';

    x += xoffset;

    if (x < 0)            return result;
    if (x >= grid->width) return result;

    y += yoffset;

    if (y < 0)             return result;
    if (y >= grid->height) return result;

    size_t stride = sizeof(*grid->data)*grid->width;

    result = *(s8 *)((u8 *)grid->data + y*stride + x);

    return result;
}

#pragma endregion
