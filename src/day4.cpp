#include <memory.h>
#include <stdlib.h>
#include <string.h>

#include "day4.h"

#pragma region Character Grid

struct Character_Grid {
    int width;
    int height;
    s8 *data;
};

static void character_grid_init(Character_Grid *grid, FILE *file) {
    const char *file_text = read_open_text_file(file);
    int file_text_length = strlen(file_text);

    grid->width  = 136;
    grid->height = 136;

    // Read characters into Character_Grid::data.
    size_t stride = sizeof(*grid->data)*grid->width;

    grid->data = (s8 *)malloc(stride*grid->height);

    fseek(file, 0, SEEK_SET);

    u8 *dest = (u8 *)grid->data;

    char tmp[136 + 1];
    while (fscanf(file, "%s\n", tmp) == 1) {
        s8 *row = (s8 *)dest;
        for (int i = 0; i < grid->width; i++) {
            row[i] = tmp[i];
        }

        dest += stride;
    }
}

static void character_grid_set_char(Character_Grid *grid,
                                    int x, int y,
                                    char c) {
    size_t stride = sizeof(*grid->data)*grid->width;

    *(s8 *)((u8 *)grid->data + y*stride + x) = c;
}

char character_grid_get_char(Character_Grid *grid,
                             int x, int y,
                             int xoffset = 0, int yoffset = 0) {
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

static u64 count_forkliftable_rolls(Character_Grid *character_grid) {
    u64 result = 0;

    for (int y = 0; y < character_grid->height; y++) {
        for (int x = 0; x < character_grid->width; x++) {
            bool is_roll = character_grid_get_char(character_grid, x, y) == '@';
            if (!is_roll) continue;

            char n = character_grid_get_char(character_grid, x, y, 0, +1);
            char e = character_grid_get_char(character_grid, x, y, +1, 0);
            char s = character_grid_get_char(character_grid, x, y, 0, -1);
            char w = character_grid_get_char(character_grid, x, y, -1, 0);

            char ne = character_grid_get_char(character_grid, x, y, +1, +1);
            char se = character_grid_get_char(character_grid, x, y, +1, -1);
            char sw = character_grid_get_char(character_grid, x, y, -1, -1);
            char nw = character_grid_get_char(character_grid, x, y, -1, +1);

            int adjacent_roll_count = 0;

            if (n == '@') adjacent_roll_count++;
            if (e == '@') adjacent_roll_count++;
            if (s == '@') adjacent_roll_count++;
            if (w == '@') adjacent_roll_count++;

            if (ne == '@') adjacent_roll_count++;
            if (se == '@') adjacent_roll_count++;
            if (sw == '@') adjacent_roll_count++;
            if (nw == '@') adjacent_roll_count++;

            if (adjacent_roll_count < 4) {
                result += 1;
            }
        }
    }

    return result;
}

static void remove_forkliftable_rolls(Character_Grid *src_character_grid,
                                      Character_Grid *dest_character_grid) {
    for (int y = 0; y < src_character_grid->height; y++) {
        for (int x = 0; x < src_character_grid->width; x++) {
            bool is_roll = character_grid_get_char(src_character_grid, x, y) == '@';
            if (!is_roll) continue;

            char n = character_grid_get_char(src_character_grid, x, y, 0, +1);
            char e = character_grid_get_char(src_character_grid, x, y, +1, 0);
            char s = character_grid_get_char(src_character_grid, x, y, 0, -1);
            char w = character_grid_get_char(src_character_grid, x, y, -1, 0);

            char ne = character_grid_get_char(src_character_grid, x, y, +1, +1);
            char se = character_grid_get_char(src_character_grid, x, y, +1, -1);
            char sw = character_grid_get_char(src_character_grid, x, y, -1, -1);
            char nw = character_grid_get_char(src_character_grid, x, y, -1, +1);

            int adjacent_roll_count = 0;

            if (n == '@') adjacent_roll_count++;
            if (e == '@') adjacent_roll_count++;
            if (s == '@') adjacent_roll_count++;
            if (w == '@') adjacent_roll_count++;

            if (ne == '@') adjacent_roll_count++;
            if (se == '@') adjacent_roll_count++;
            if (sw == '@') adjacent_roll_count++;
            if (nw == '@') adjacent_roll_count++;

            if (adjacent_roll_count < 4) {
                character_grid_set_char(dest_character_grid, x, y, '.');
            }
        }
    }
}

u64 day_4_part_1(FILE *file) {
    Character_Grid character_grid;
    character_grid_init(&character_grid, file);

    u64 result = count_forkliftable_rolls(&character_grid);

    return result;
}

u64 day_4_part_2(FILE *file) {
    u64 result = 0;

    Character_Grid character_grid;
    character_grid_init(&character_grid, file);
    
    Character_Grid tmp_grid;
    character_grid_init(&tmp_grid, file);

    for (;;) {
        memcpy(tmp_grid.data, character_grid.data, sizeof(s8)*character_grid.width*character_grid.height);

        int forkliftable_count = count_forkliftable_rolls(&character_grid);
        if (forkliftable_count <= 0) {
            break;
        }

        remove_forkliftable_rolls(&tmp_grid, &character_grid);

        result += forkliftable_count;
    }

    return result;
}
