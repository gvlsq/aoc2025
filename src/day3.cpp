#include <math.h>

#include "day3.h"

static const int BANK_LENGTH = 100;

int get_max_digit_in_range(const char *bank, int start, int end, int *index) {
    int max_digit = INT32_MIN;
    int max_digit_index = BANK_LENGTH;
    for (int i = start; i <= end; i++) {
        int digit = bank[i] - '0';

        if (digit > max_digit) {
            max_digit = digit;
            max_digit_index = i;
        }
    }

    *index = max_digit_index;

    return max_digit;
}

u64 day_3_part_1(FILE *file) {
    u64 output_joltage = 0;

    bool parsing = true;
    while (parsing) {
        char bank[BANK_LENGTH + 1];
        if (fscanf(file, "%s\n", &bank) == 1) {
            int max_first_battery = INT32_MIN;
            int max_first_battery_index = BANK_LENGTH;
            for (int i = 0; i < (BANK_LENGTH - 1); i++) {
                int digit = bank[i] - '0';

                if (digit > max_first_battery) {
                    max_first_battery_index = i;
                    max_first_battery = digit;
                }
            }

            int max_second_battery = INT32_MIN;
            for (int i = max_first_battery_index + 1; i < BANK_LENGTH; i++) {
                int digit = bank[i] - '0';

                if (digit > max_second_battery) {
                    max_second_battery = digit;
                }
            }

            u64 max_joltage = max_first_battery*10 + max_second_battery;
            output_joltage += max_joltage;
        } else {
            parsing = false;
        }
    }

    return output_joltage;
}

u64 day_3_part_2(FILE *file) {
    u64 output_joltage = 0;

    bool parsing = true;
    while (parsing) {
        char bank[BANK_LENGTH + 1];
        if (fscanf(file, "%s\n", &bank) == 1) {
            u64 max_joltage = 0;

            int length = 12;
            int last_index = 0;
            for (int i = length - 1; i >= 0; i--) {
                int index;
                int digit = get_max_digit_in_range(bank, last_index, (BANK_LENGTH - 1) - i, &index);

                last_index = index + 1;

                max_joltage += digit*(u64)pow((double)10, (double)i);
            }

            output_joltage += max_joltage;
        } else {
            parsing = false;
        }
    }

    return output_joltage;
}
