#include <math.h>

#include "day9.h"

struct Vector2u {
    u64 x;
    u64 y;
};

static int position_count;
static Vector2u positions[500];

u64 day_9_part_1(FILE *file) {
    u64 area_max = 0;

    u64 x;
    u64 y;
    while (fscanf(file, "%I64u,%I64u\n", &x, &y) == 2) {
        assert(position_count < array_count(positions));

        Vector2u *position = &positions[position_count++];
        position->x = x;
        position->y = y;
    }

    for (int i = 0; i < position_count - 1; i++) {
        Vector2u a = positions[i];

        for (int j = i + 1; j < position_count; j++) {
            Vector2u b = positions[j];

            if (!((a.x == b.x) &&
                  (a.y == b.y))) {
                u64 dx = (u64)llabs(b.x - a.x);
                u64 dy = (u64)llabs(b.y - a.y);

                u64 area = (dx + 1)*(dy + 1);
                if (area > area_max) {
                    area_max = area;
                }
            }
        }
    }

    return area_max;
}

u64 day_9_part_2(FILE *file) {
    u64 result = 0;

    return result;
}
