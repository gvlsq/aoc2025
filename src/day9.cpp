#include <math.h>
#include <stdlib.h>

#include "day9.h"

struct Vector2u {
    u64 x;
    u64 y;
};

struct Rank {
    u64 value;
    u64 rank;
};

static const u64 QUEUE_ELEMENT_COUNT = 500000;

static int position_count;
static Vector2u positions[500];
static Vector2u tmp_positions[500];

static u64 read_cursor;
static u64 write_cursor;

static int xrank_count;
static Rank xranks[1000];

static int yrank_count;
static Rank yranks[1000];

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

static void connect_horizontally(Character_Grid *character_grid,
                                 u64 y,
                                 u64 xmin, u64 xmax) {
    assert(xmin != xmax);

    if (xmin > xmax) {
        u64 swap = xmin;
        xmin = xmax;
        xmax = swap;
    }

    for (u64 x = xmin + 1; x < xmax; x++) {
        character_grid_set_char(character_grid, x, y, 'X');
    }
}

static void connect_vertically(Character_Grid *character_grid,
                               u64 x,
                               u64 ymin, u64 ymax) {
    assert(ymin != ymax);

    if (ymin > ymax) {
        u64 swap = ymin;
        ymin = ymax;
        ymax = swap;
    }

    for (u64 y = ymin + 1; y < ymax; y++) {
        character_grid_set_char(character_grid, x, y, 'X');
    }
}

static void enqueue_tile(Vector2u *queue, u64 x, u64 y) {
    assert(write_cursor < QUEUE_ELEMENT_COUNT);

    queue[write_cursor++] = {x, y};
}

static Vector2u dequeue_tile(Vector2u *queue) {
    assert(read_cursor < write_cursor);
    assert(read_cursor < QUEUE_ELEMENT_COUNT);

    Vector2u result = queue[read_cursor++];

    return result;
}

static bool rectangle_only_has_red_and_green_tiles(Character_Grid *character_grid,
                                                   Vector2u a, Vector2u b) {
    u64 xmin = a.x;
    u64 ymin = a.y;
    u64 xmax = b.x;
    u64 ymax = b.y;

    if (xmin > xmax) {
        u64 swap = xmin;
        xmin = xmax;
        xmax = swap;
    }

    if (ymin > ymax) {
        u64 swap = ymin;
        ymin = ymax;
        ymax = swap;
    }

    for (u64 y = ymin; y <= ymax; y++) {
        for (u64 x = xmin; x <= xmax; x++) {
            char c = character_grid_get_char(character_grid, x, y);
            if ((c != 'X') && (c != '#')) {
                return false;
            }
        }
    }

    return true;
}

int position_compare_x_callback(const void *p, const void *q) {
    Vector2u a = *(Vector2u *)p;
    Vector2u b = *(Vector2u *)q;

    if (a.x < b.x) return -1;
    if (a.x > b.x) return +1;

    return 0;
}

int position_compare_y_callback(const void *p, const void *q) {
    Vector2u a = *(Vector2u *)p;
    Vector2u b = *(Vector2u *)q;

    if (a.y < b.y) return -1;
    if (a.y > b.y) return +1;

    return 0;
}

static u64 get_x_value_from_rank(u64 rank) {
    for (int i = 0; i < xrank_count; i++) {
        if (xranks[i].rank == rank) {
            return xranks[i].value;
        }
    }

    assert(0);

    return 0;
}

static u64 get_y_value_from_rank(u64 rank) {
    for (int i = 0; i < yrank_count; i++) {
        if (yranks[i].rank == rank) {
            return yranks[i].value;
        }
    }

    assert(0);

    return 0;
}

u64 day_9_part_2(FILE *file) {
    u64 result = 0;

    Character_Grid character_grid;
    character_grid_init(&character_grid, 0, 500, 500);

    u64 x;
    u64 y;
    while (fscanf(file, "%I64u,%I64u\n", &x, &y) == 2) {
        assert(position_count < array_count(positions));

        Vector2u *position = &positions[position_count];
        position->x = x;
        position->y = y;

        Vector2u *tmp_position = &tmp_positions[position_count];
        tmp_position->x = x;
        tmp_position->y = y;

        position_count++;
    }

    // Map X-Y coordinates to ranks instead of positions.
    qsort(tmp_positions, position_count, sizeof(tmp_positions[0]), position_compare_x_callback);

    int running_rank = 1;
    for (int i = 0; i < position_count; i++) {
        bool new_rank = true;
        if ((i > 0) &&
            (tmp_positions[i].x == tmp_positions[i - 1].x)) {
            new_rank = false;
        }

        if (new_rank) {
            Rank *rank = &xranks[xrank_count++];
            rank->value = tmp_positions[i].x;
            rank->rank  = running_rank++;
        }
    }

    for (int i = 0; i < position_count; i++) {
        Vector2u *position = &positions[i];

        bool found = false;

        for (int j = 0; j < xrank_count; j++) {
            Rank *rank = &xranks[j];
            if (rank->value == position->x) {
                position->x = rank->rank;
                
                found = true;
                break;
            }
        }

        assert(found);
    }

    qsort(tmp_positions, position_count, sizeof(tmp_positions[0]), position_compare_y_callback);

    running_rank = 1;
    for (int i = 0; i < position_count; i++) {
        bool new_rank = true;
        if ((i > 0) &&
            (tmp_positions[i].y == tmp_positions[i - 1].y)) {
            new_rank = false;
        }

        if (new_rank) {
            Rank *rank = &yranks[yrank_count++];
            rank->value = tmp_positions[i].y;
            rank->rank = running_rank++;
        }
    }

    for (int i = 0; i < position_count; i++) {
        Vector2u *position = &positions[i];

        bool found = false;

        for (int j = 0; j < yrank_count; j++) {
            Rank *rank = &yranks[j];
            if (rank->value == position->y) {
                position->y = rank->rank;

                found = true;
                break;
            }
        }

        assert(found);
    }

    //
    for (int i = 0; i < position_count; i++) {
        // Place red tile on grid.
        character_grid_set_char(&character_grid, positions[i].x, positions[i].y, '#');

        // Connect red tiles to tiles before and after them using green tiles.
        Vector2u positions_to_connect[3] = {
            (i == 0 ? positions[position_count - 1] : positions[i - 1]),
            positions[i],
            ((i == (position_count - 1)) ? positions[0] : positions[i + 1]),
        };

        for (int j = 0; j < array_count(positions_to_connect) - 1; j++) {
            Vector2u a = positions_to_connect[j];
            Vector2u b = positions_to_connect[j + 1];

            if (a.x == b.x) {
                connect_vertically(&character_grid, a.x, a.y, b.y);
            } else if (a.y == b.y) {
                connect_horizontally(&character_grid, a.y, a.x, b.x);
            } else assert(0);
        }
    }

    // Fill polygon with green tiles.
    Vector2u *queue = (Vector2u *)malloc(sizeof(*queue)*QUEUE_ELEMENT_COUNT);

    enqueue_tile(queue, 0, 0);

    while (read_cursor < write_cursor) {
        Vector2u tile = dequeue_tile(queue);

        char up    = character_grid_get_char(&character_grid, tile.x, tile.y - 1);
        char down  = character_grid_get_char(&character_grid, tile.x, tile.y + 1);
        char left  = character_grid_get_char(&character_grid, tile.x - 1, tile.y);
        char right = character_grid_get_char(&character_grid, tile.x + 1, tile.y);

        if (up == '.') {
            if (character_grid_set_char(&character_grid, tile.x, tile.y - 1, 'A')) {
                enqueue_tile(queue, tile.x, tile.y - 1);
            }
        }

        if (down == '.') {
            if (character_grid_set_char(&character_grid, tile.x, tile.y + 1, 'A')) {
                enqueue_tile(queue, tile.x, tile.y + 1);
            }
        }

        if (left == '.') {
            if (character_grid_set_char(&character_grid, tile.x - 1, tile.y, 'A')) {
                enqueue_tile(queue, tile.x - 1, tile.y);
            }
        }

        if (right == '.') {
            if (character_grid_set_char(&character_grid, tile.x + 1, tile.y, 'A')) {
                enqueue_tile(queue, tile.x + 1, tile.y);
            }
        }
    }

    for (u64 y = 0; y < character_grid.height; y++) {
        for (u64 x = 0; x < character_grid.width; x++) {
            if (character_grid_get_char(&character_grid, x, y) == '.') {
                character_grid_set_char(&character_grid, x, y, 'X');
            }
        }
    }

    //
    u64 area_max = 0;

    for (int i = 0; i < position_count - 1; i++) {
        Vector2u a = positions[i];

        for (int j = i + 1; j < position_count; j++) {
            Vector2u b = positions[j];

            if (!((a.x == b.x) && (a.y == b.y))) {
                if (rectangle_only_has_red_and_green_tiles(&character_grid, a, b)) {
                    u64 dx = (u64)llabs(get_x_value_from_rank(b.x) - get_x_value_from_rank(a.x));
                    u64 dy = (u64)llabs(get_y_value_from_rank(b.y) - get_y_value_from_rank(a.y));

                    u64 area = (dx + 1)*(dy + 1);
                    if (area > area_max) {
                        area_max = area;
                    }
                }
            }
        }
    }

    return area_max;
}
