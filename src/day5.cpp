#include <assert.h>
#include <stdlib.h>

#include "day5.h"

struct ID_Range {
    u64 minimum;
    u64 maximum;
};

u64 day_5_part_1(FILE *file) {
    u64 result = 0;

    FILE *ranges = fopen("data/input1.txt", "r"); // Ranges from input.txt
    FILE *ids    = fopen("data/input2.txt", "r"); // IDs from input.txt

    assert(ranges && ids);

    u64 id;
    while (fscanf(ids, "%I64u\n", &id) == 1) {
        fseek(ranges, 0, SEEK_SET);

        u64 minimum;
        u64 maximum;
        while (fscanf(ranges, "%I64u-%I64u\n", &minimum, &maximum) == 2) {
            if (minimum <= id && id <= maximum) {
                result++;
                break;
            }
        }
    }

    return result;
}

u64 day_5_part_2(FILE *file) {
    u64 result = 0;

    FILE *ranges = fopen("data/input1.txt", "r"); // Ranges from input.txt

    int id_range_count = 0;
    ID_Range *id_ranges = (ID_Range *)malloc(sizeof(*id_ranges)*256);

    u64 minimum;
    u64 maximum;
    while (fscanf(ranges, "%I64u-%I64u\n", &minimum, &maximum) == 2) {
        ID_Range *id_range = &id_ranges[id_range_count++];
        id_range->minimum = minimum;
        id_range->maximum = maximum;
    }

    // Bubble sort the ranges by their minimums.
    for (int i = 0; i < id_range_count; i++) {
        bool swapped = false;

        for (int j = 0; j < (id_range_count - 1); j++) {
            ID_Range *a = &id_ranges[j];
            ID_Range *b = &id_ranges[j + 1];

            if ((b->minimum < a->minimum) ||
                ((b->minimum == a->minimum) && (b->maximum < a->maximum))) {
                ID_Range swap = *b;
                *b = *a;
                *a = swap;

                swapped = true;
            }
        }

        if (!swapped) break;
    }

    u64 running_minimum = id_ranges[0].minimum;
    u64 running_maximum = id_ranges[0].maximum;

    for (int i = 1; i < id_range_count; i++) {
        ID_Range *range = &id_ranges[i];

        if ((range->minimum <= (running_maximum + 1)) &&
            (range->maximum > running_maximum)) {
            running_maximum = range->maximum;
        } else {
            result += (running_maximum - running_minimum) + 1;

            running_minimum = range->minimum;
            running_maximum = range->maximum;
        }
    }

    result += (running_maximum - running_minimum) + 1;

    return result;
}
