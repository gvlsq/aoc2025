#include <stdio.h>

#include "day1.h"

int main(int argc, char **argv) {
    FILE *file = fopen("data/input.txt", "rb");
    if (!file) return 1;

    int solution = 0;

#if DAY_1_PART_1
    solution = day_1_part_1(file);
#elif DAY_1_PART_2
    solution = day_1_part_2(file);
#endif

    printf("Solution: %d\n", solution);

    return 0;
}
