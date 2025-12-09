#include <assert.h>
#include <memory.h>
#include <stdlib.h>

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

    Character_Grid manifold_diagram;
    character_grid_init(&manifold_diagram, file, 141, 142);

    int width  = manifold_diagram.width;
    int height = manifold_diagram.height;

    u64 *beam_counts = (u64 *)calloc(width*height, sizeof(beam_counts[0]));

    // Find S position and propagate initial beam.
    int sy = 0;
    int sx = 0;

    for (int x = 0; x < width; x++) {
        char c = character_grid_get_char(&manifold_diagram, x, 0);
        if (c == 'S') {
            sx = x;
            break;
        }
    }

    beam_counts[sy*width + sx] = 1;

    // Propagate beams.
    for (int y = sy + 1; y < height; y++) {
        for (int x = 0; x < width; x++) {
            u64 beams_above_count = beam_counts[(y - 1) * width + x];
            if (beams_above_count == 0) continue;

            char at = character_grid_get_char(&manifold_diagram, x, y);
            if (at == '^') {
                beam_counts[y*width + (x - 1)] += beams_above_count;
                beam_counts[y*width + (x + 1)] += beams_above_count;
            } else if (at == '.') {
                beam_counts[y*width + x] += beams_above_count;
            } else assert(0);
        }
    }

    // Sum propagated beams in bottommost row.
    for (int x = 0; x < width; x++) {
        result += beam_counts[(height - 1)*width + x];
    }

    return result;
}
