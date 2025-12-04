#include <stdio.h>

#include "day1.h"
#include "day2.h"
#include "day3.h"
#include "types.h"

int main(int argc, char **argv) {
    FILE *file = fopen("data/input.txt", "rb");
    if (!file) return 1;

    u64 solution = 0;

#if DAY_1_PART_1
    solution = day_1_part_1(file);
#elif DAY_1_PART_2
    solution = day_1_part_2(file);
#elif DAY_2_PART_1
    solution = day_2_part_1(file);
#elif DAY_2_PART_2
    solution = day_2_part_2(file);
#elif DAY_3_PART_1
    solution = day_3_part_1(file);
#elif DAY_3_PART_2
    solution = day_3_part_2(file);
#endif

    printf("Solution: %I64u\n", solution);

    return 0;
}
