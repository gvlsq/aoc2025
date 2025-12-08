#include "day7.h"

u64 day_7_part_1(FILE *file) {
    u64 result = 0;

    Character_Grid manifold_diagram;
    character_grid_init(&manifold_diagram, file, 141, 142);

    for (int y = 1; y < manifold_diagram.height; y++) {
        for (int x = 0; x < manifold_diagram.width; x++) {
            char at    = character_grid_get_char(&manifold_diagram, x, y);
            char above = character_grid_get_char(&manifold_diagram, x, y - 1);

            if (above == 'S') {
                character_grid_set_char(&manifold_diagram, x, y, '|');
            } else if ((above == '|') && (at != '^')) {
                character_grid_set_char(&manifold_diagram, x, y, '|');
            } else if ((above == '|') && (at == '^')) {
                character_grid_set_char(&manifold_diagram, x - 1, y, '|');
                character_grid_set_char(&manifold_diagram, x + 1, y, '|');
            }
        }
    }

    for (int y = 1; y < manifold_diagram.height; y++) {
        for (int x = 0; x < manifold_diagram.width; x++) {
            char at    = character_grid_get_char(&manifold_diagram, x, y);
            char above = character_grid_get_char(&manifold_diagram, x, y - 1);

            if ((at == '^') && (above == '|')) {
                result++;
            }
        }
    }

    return result;
}

u64 day_7_part_2(FILE *file) {
    u64 result = 0;

    return result;
}
