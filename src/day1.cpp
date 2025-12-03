#include <assert.h>
#include <stdio.h>

#include "day1.h"

static inline int math_modi(int i, int m) {
    int result = i % m;
    if (result < 0) result += m;

    return result;
}

u64 day_1_part_1(FILE *file) {
    int dial_at = 50;
    int zero_count = 0;

    bool parsing = true;
    while (parsing) {
        char rotation_dir;
        int rotation_count;
        if (fscanf(file, "%c%d\n", &rotation_dir, &rotation_count) == 2) {
            if (rotation_dir == 'L') {
                dial_at -= rotation_count;
            } else if (rotation_dir == 'R') {
                dial_at += rotation_count;
            } else assert(0);

            dial_at = math_modi(dial_at, 100);
            if (dial_at == 0) {
                zero_count++;
            }
        } else {
            parsing = false;
        }
    }

    return zero_count;
}

u64 day_1_part_2(FILE *file) {
    int dial_at = 50;
    int zero_count = 0;

    bool parsing = true;
    while (parsing) {
        char rotation_dir;
        int rotation_count;
        if (fscanf(file, "%c%d\n", &rotation_dir, &rotation_count) == 2) {
            if (rotation_dir == 'L') {
                for (int i = 0; i < rotation_count; i++) {
                    dial_at--;
                    dial_at = math_modi(dial_at, 100);
                    if (dial_at == 0) {
                        zero_count++;
                    }
                }
            } else if (rotation_dir == 'R') {
                for (int i = 0; i < rotation_count; i++) {
                    dial_at++;
                    dial_at = math_modi(dial_at, 100);
                    if (dial_at == 0) {
                        zero_count++;
                    }
                }
            } else assert(0);
        } else {
            parsing = false;
        }
    }

    return zero_count;
}
